#include "../headers/game.hpp"
#include <cmath>

GolfGame::GolfGame() : ball(390, 500, 10) {}

void GolfGame::draw(sf::RenderWindow& window) {
    ball.draw(window);
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

void GolfGame::update(float deltaTime) {
    ball.update(deltaTime);
}
