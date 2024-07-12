#include "../headers/map.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

Map::Map(int height, int width, sf::RenderWindow& window, const std::string& obstacleTextureFile, const std::string& holeTextureFile)
    : grid(height, std::vector<int>(width, EMPTY)), window(window), currentWaterFrame(0), animationTimer(0.0f), animationSpeed(0.0001f) {

    // Load obstacle texture
    if (!obstacleTexture.loadFromFile(obstacleTextureFile)) {
        std::cerr << "Error loading obstacle texture from " << obstacleTextureFile << std::endl;
    } else {
        obstacle.setTexture(obstacleTexture);
        this->cellSize = window.getSize().x / width;
        float scaleFactor = static_cast<float>(cellSize) / obstacleTexture.getSize().x;
        obstacle.setScale(scaleFactor, scaleFactor);
        obstacle.setOrigin(cellSize / 2, cellSize / 2);
    }

    // Load sand texture
    if (!sandTexture.loadFromFile("assets/sand.jpg")) {
        std::cerr << "Error loading sand texture from assets/sand.jpg" << std::endl;
    } else {
        sand.setTexture(sandTexture);
        float sandScale = static_cast<float>(cellSize) / sandTexture.getSize().x;
        sand.setScale(sandScale, sandScale);
        sand.setOrigin(cellSize / 2, cellSize / 2);
    }

    // Load hole texture
    if (!holeTexture.loadFromFile(holeTextureFile)) {
        std::cerr << "Error loading hole texture from " << holeTextureFile << std::endl;
    } else {
        hole.setTexture(holeTexture);
        float holeScaleFactor = static_cast<float>(cellSize) / holeTexture.getSize().x;
        hole.setScale(holeScaleFactor / 2, holeScaleFactor / 2);
        hole.setOrigin(cellSize / 2, cellSize / 2);
    }

    // Load flag texture
    if (!flagTexture.loadFromFile("assets/flag.png")) {
        std::cerr << "Error loading flag texture from assets/flag.png" << std::endl;
    } else {
        flag.setTexture(flagTexture);
        float flagScaleFactor = 0.2f;
        flag.setScale(flagScaleFactor, flagScaleFactor);
    }

    // Load water shader
    if (!waterShader.loadFromFile("assets/water.frag", sf::Shader::Fragment)) {
        std::cerr << "Error loading water shader from assets/water.frag" << std::endl;
    } else {
        std::cout << "Water shader loaded successfully" << std::endl;
    }

    // Load a base texture for water (e.g., a plain blue texture)
    if (!waterTexture.loadFromFile("assets/water_base.png")) {
        std::cerr << "Error loading water base texture from assets/water_base.jpg" << std::endl;
    } else {
        waterSprite.setTexture(waterTexture);
        waterSprite.setScale(static_cast<float>(cellSize) / waterTexture.getSize().x, static_cast<float>(cellSize) / waterTexture.getSize().y);
        waterSprite.setOrigin(cellSize / 2, cellSize / 2);
    }
}

void Map::loadMap(const std::vector<std::vector<int>>& mapData) {
    grid = mapData;
}

void Map::loadMapFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file " + filePath);
    }

    std::string line;
    grid.clear();

    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char& ch : line) {
            if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4') {
                row.push_back(ch - '0');
            }
        }
        grid.push_back(row);
    }
    file.close();
}

void Map::draw() {
    animationTimer += animationSpeed;
    float time = static_cast<float>(animationTimer);

    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] == OBSTACLE) {
                obstacle.setPosition(x * cellSize, y * cellSize);
                window.draw(obstacle);
            } else if (grid[y][x] == SAND) {
                sand.setPosition(x * cellSize, y * cellSize);
                window.draw(sand);
            } else if (grid[y][x] == HOLE) {
                hole.setPosition(x * cellSize, y * cellSize);
                flag.setPosition(x * cellSize + 10, y * cellSize - 40);
                window.draw(hole);
                window.draw(flag);
            } else if (grid[y][x] == WATER) {
                waterShader.setUniform("time", time);
                waterShader.setUniform("resolution", sf::Vector2f(window.getSize()));
                waterShader.setUniform("texture", sf::Shader::CurrentTexture);
                waterSprite.setPosition(x * cellSize, y * cellSize);
                window.draw(waterSprite, &waterShader);
            }
        }
    }
}

bool Map::isObstacle(int x, int y) {
    int gridX = x / cellSize;
    int gridY = y / cellSize;
    if (gridX < 0 || gridX >= grid[0].size() || gridY < 0 || gridY >= grid.size()) {
        return false;
    }
    return grid[gridY][gridX] == OBSTACLE;
}

bool Map::isSand(int x, int y) {
    int gridX = x / cellSize;
    int gridY = y / cellSize;
    if (gridX < 0 || gridX >= grid[0].size() || gridY < 0 || gridY >= grid.size()) {
        return false;
    }
    return grid[gridY][gridX] == SAND;
}

bool Map::isHole(int x, int y) {
    sf::Vector2f pos = hole.getPosition();
    if (pos.x > x - 20 && pos.x < x + 10 && pos.y > y - 20 && pos.y < y + 10) {
        return true;
    }
    return false;
}

sf::Vector2f Map::getHoleCenter() {
    sf::Vector2f pos = hole.getPosition();
    pos.x += holeTexture.getSize().x / 2;
    pos.y += holeTexture.getSize().y / 2;
    return pos;
}

sf::Vector2i Map::getCellCoords(sf::Vector2f position) {
    int x = static_cast<int>(position.x) / cellSize;
    int y = static_cast<int>(position.y) / cellSize;
    return sf::Vector2i(x, y);
}
