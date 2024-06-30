#include "../headers/map.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

Map::Map(int height, int width, sf::RenderWindow& window, const std::string& textureFile)
    : grid(height, std::vector<int>(width, EMPTY)), window(window) {

    if (!obstacleTexture.loadFromFile(textureFile)) {
      std::cerr << "Error loading texture";
    }
    
    obstacle.setTexture(obstacleTexture);
    this->cellSize = window.getSize().x / width;
    float scaleFactor = static_cast<float> (cellSize) / obstacleTexture.getSize().x; 
    obstacle.setScale(scaleFactor, scaleFactor);
    obstacle.setOrigin(cellSize, cellSize);
}

void Map::loadMap(const std::vector<std::vector<int>>& mapData) {
    grid = mapData;
}

void Map::loadMapFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file");
    }

    std::string line;
    grid.clear();

    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char& ch : line) {
            if (ch == '0' || ch == '1') { 
                row.push_back(ch - '0'); 
            }
        }
        grid.push_back(row);
    }
    file.close();
}

void Map::draw() {
    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] == OBSTACLE) {
                obstacle.setPosition(x * cellSize, y * cellSize);
                window.draw(obstacle);
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

sf::Vector2i Map::getCellCoords(sf::Vector2f position) {
    int x = static_cast<int>(position.x) / cellSize;
    int y = static_cast<int>(position.y) / cellSize;
    return sf::Vector2i(x, y);
}
