#include "../headers/ball.hpp"
#include <iostream>
#include <SFML/System.hpp>

Ball::Ball(int x, int y, int radius, const std::string& textureFile) : velMagnitude(0), friction(1.0f), scaling(false) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error loading texture from file: " << textureFile << std::endl;
    }
    initialPos = sf::Vector2f(x, y);
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setOrigin(radius, radius);
    sprite.setScale(2.0f * radius / texture.getSize().x, 2.0f * radius / texture.getSize().y); 
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Ball::setPos(int x, int y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Ball::getPos() {
    return sprite.getPosition();
}

void Ball::move(float velMagnitude, sf::Vector2f velDirection) {
    this->velMagnitude = velMagnitude * 3.0f; 
    this->velDirection = velDirection; 
}

void Ball::update(float deltaTime, const sf::RenderWindow& window, Map& map) {
    float ballRadius = sprite.getGlobalBounds().width / 2.0f;

    if (velMagnitude > 0) {
        sf::Vector2f pos = sprite.getPosition();
        sf::Vector2f newPos = pos + velDirection * velMagnitude * deltaTime;

        velMagnitude -= friction * velMagnitude * deltaTime;
        if (velMagnitude < 0) velMagnitude = 0; 

        sf::Vector2u windowSize = window.getSize();

        if (newPos.x < ballRadius) {
            newPos.x = ballRadius;
            velDirection.x = -velDirection.x;
        }
        if (newPos.x > windowSize.x - ballRadius) { 
            newPos.x = windowSize.x - ballRadius;
            velDirection.x = -velDirection.x;   
        }
        if (newPos.y < ballRadius) {
            newPos.y = ballRadius;
            velDirection.y = -velDirection.y;
        }
        if (newPos.y > windowSize.y - ballRadius) {
            newPos.y = windowSize.y - ballRadius;
            velDirection.y = -velDirection.y;
        }

        if (map.isObstacle(newPos.x - ballRadius, pos.y) || map.isObstacle(newPos.x + ballRadius, pos.y)) {
            velDirection.x = -velDirection.x;
            newPos.x = pos.x;
        }

        if (map.isObstacle(pos.x, newPos.y - ballRadius) || map.isObstacle(pos.x, newPos.y + ballRadius)) {
            velDirection.y = -velDirection.y;
            newPos.y = pos.y;
        }

        if (map.isObstacle(newPos.x - ballRadius, newPos.y - ballRadius) || 
            map.isObstacle(newPos.x + ballRadius, newPos.y - ballRadius) ||
            map.isObstacle(newPos.x - ballRadius, newPos.y + ballRadius) ||
            map.isObstacle(newPos.x + ballRadius, newPos.y + ballRadius)) {
            velDirection.x = -velDirection.x;
            velDirection.y = -velDirection.y;
            newPos = pos;
        }

        if (map.isHole(newPos.x, newPos.y)) {
            velMagnitude = 0;
            newPos = map.getHoleCenter();
            scaling = true;
            isHoleComplete = true;
        }

        sprite.setPosition(newPos);
    }
}

bool Ball::getHoleStatus() {
    return isHoleComplete;
}

void Ball::setHoleStatus() {
    isHoleComplete = false;
}

void Ball::reset() {
    sprite.setPosition(initialPos);
    scaling = false;
}
