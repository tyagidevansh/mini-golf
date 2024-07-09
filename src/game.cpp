#include "../headers/game.hpp"
#include <cmath>
#include <iostream>
#include <SFML/System.hpp>

GolfGame::GolfGame(sf::RenderWindow& window) : ball(390, 500, 10, "assets/golfBall.png", "assets/arrow.png"), map(18, 32, window, "assets/obstacle.png", "assets/hole.png") {
    loadLevel("levels/level1.txt");

    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error opening font file";
    }

    strokeText.setFont(font);
    strokeText.setCharacterSize(32);
    strokeText.setFillColor(sf::Color::White);
    strokeText.setPosition(10, 10);

    levelUpText.setFont(font);
    levelUpText.setCharacterSize(48);
    levelUpText.setFillColor(sf::Color::White);
    levelUpText.setPosition(400, 300);
}

void GolfGame::draw(sf::RenderWindow& window) {
    map.draw();
    ball.draw(window);

    strokeText.setString("Strokes: " + std::to_string(strokeCount));
    window.draw(strokeText);
}

void GolfGame::handlePress(sf::Event& event) {
    validClick = false;
    if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f curPos = ball.getPos();
        if (event.mouseButton.x <= curPos.x + 30 && event.mouseButton.x >= curPos.x - 30 
            && event.mouseButton.y <= curPos.y + 30 && event.mouseButton.y >= curPos.y - 30) {
            initialPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            validClick = true;
            isMousePressed = true; 
        }
    }
}

void GolfGame::handleRelease(sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left and validClick) {
        finalPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        calculateVelocity();
        ball.move(velMagnitude, velDirection);
        strokeCount++;
        isMousePressed = false; 
    }
}

void GolfGame::handleMouseMoved(sf::Event& event) {
    if (isMousePressed && validClick) {
        sf::Vector2f currentPos(event.mouseMove.x, event.mouseMove.y);
        velDirection = initialPos - currentPos;
        velMagnitude = std::sqrt(velDirection.x * velDirection.x + velDirection.y * velDirection.y);
        if (velMagnitude != 0) {
            velDirection /= velMagnitude;
        }
        ball.updatePowerIndicator(velMagnitude, velDirection); 
    }
}

void GolfGame::update(float deltaTime, sf::RenderWindow& window) {
    ball.update(deltaTime, window, map);
    if (ball.getHoleStatus()) {
        handleLevelUp(window);
        ball.setHoleStatus();
    }

    if (!isMousePressed) {
        ball.updatePowerIndicator(0, sf::Vector2f(0,0));
    }
}

void GolfGame::loadLevel(const std::string& filePath) {
    map.loadMapFromFile(filePath);
    ball.reset();
    strokeCount = 0;
}

void GolfGame::calculateVelocity() {
    velDirection = initialPos - finalPos;
    velMagnitude = std::sqrt(velDirection.x * velDirection.x + velDirection.y * velDirection.y);
    if (velMagnitude != 0) {
        velDirection /= velMagnitude;
    }
    //std::cout << velMagnitude << std::endl;
}

void GolfGame::handleLevelUp(sf::RenderWindow& window) {
    displayLevelUpText(window);
    sf::sleep(sf::seconds(1));
    curLevel++;
    if (curLevel <= 4) {
        loadLevel("levels/level" + std::to_string(curLevel) + ".txt");
    }
    else {
        exit(0);
    }
    ball.updatePowerIndicator(0, sf::Vector2f(0,0));
}

void GolfGame::displayLevelUpText(sf::RenderWindow& window) {
    levelUpText.setString("Hole complete!");
    window.draw(levelUpText);
    window.display();
}