#include "../headers/game.hpp"
#include <cmath>
#include <iostream>
#include <SFML/System.hpp>

GolfGame::GolfGame(sf::RenderWindow& window) : ball(390, 500, 10, "assets/golfBall.png"), map(18, 32, window, "assets/obstacle.png", "assets/hole.png"), strokeCount(0), levelUp(false) {
    loadLevel("levels/level1.txt");

    // Load the font
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // Initialize stroke text
    strokeText.setFont(font);
    strokeText.setCharacterSize(24);
    strokeText.setFillColor(sf::Color::White);
    strokeText.setPosition(10, 10);

    // Initialize level-up text
    levelUpText.setFont(font);
    levelUpText.setCharacterSize(36);
    levelUpText.setFillColor(sf::Color::Green);
    levelUpText.setString("Level Up!");
    levelUpText.setPosition(400, 300);
}

void GolfGame::draw(sf::RenderWindow& window) {
    map.draw();
    ball.draw(window);

    // Update and draw stroke text
    strokeText.setString("Strokes: " + std::to_string(strokeCount));
    window.draw(strokeText);

    // Draw level-up text if needed
    if (levelUp) {
        window.draw(levelUpText);
    }
}

void GolfGame::handlePress(sf::Event& event) {
    validClick = false;
    if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f curPos = ball.getPos();
        if (event.mouseButton.x <= curPos.x + 30 && event.mouseButton.x >= curPos.x - 30 
            && event.mouseButton.y <= curPos.y + 30 && event.mouseButton.y >= curPos.y - 30) {
            initialPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            validClick = true;
        }
    }
}

void GolfGame::handleRelease(sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left and validClick) {
        finalPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        calculateVelocity();
        ball.move(velMagnitude, velDirection);
        strokeCount++;
    }
}

void GolfGame::calculateVelocity() {
    velDirection =initialPos - finalPos;
    velMagnitude = std::sqrt(velDirection.x * velDirection.x + velDirection.y * velDirection.y);
    if (velMagnitude != 0) {
        velDirection /= velMagnitude;
    }
    velDirection = velDirection; 
}

void GolfGame::update(float deltaTime, const sf::RenderWindow& window) {
    ball.update(deltaTime, window, map);

    if (ball.getHoleStatus()) {
        levelUp = true;
        ball.setHoleStatus();

        // Wait for 1 second before handling level up
        sf::sleep(sf::seconds(1));
        handleLevelUp();
        ball.reset();
    }
}

void GolfGame::loadLevel(const std::string& filePath) {
    map.loadMapFromFile(filePath);
}

void GolfGame::handleLevelUp() {
    std::string filePath = "levels/level";
    curLevel++;
    filePath = filePath + std::to_string(curLevel) + ".txt";
    std::cout << filePath << "\n";
    loadLevel(filePath);
    ball.setPos(390, 500);
    strokeCount = 0; // Reset stroke count for new level
    levelUp = false; // Reset level up flag
}
