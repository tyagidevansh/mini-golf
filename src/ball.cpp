#include "../headers/ball.hpp"
#include <iostream>
#include <SFML/System.hpp>

Ball::Ball(int x, int y, int radius, const std::string& textureFile) : velMagnitude(0), friction(1.0f) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error loading texture from file" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    float scaleFactor = static_cast<float>(radius * 2) / texture.getSize().x;
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setOrigin(radius, radius);
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
    std::cout << "Velocity Magnitude: " << this->velMagnitude << "\n";
    std::cout << "Velocity Direction: " << this->velDirection.x << " " << this->velDirection.y << "\n";
}

void Ball::update(float deltaTime, const sf::RenderWindow& window, Map& map) {
    if (velMagnitude > 0) {
        sf::Vector2f pos = sprite.getPosition();
        sf::Vector2f newPos = pos + velDirection * velMagnitude * deltaTime;

        velMagnitude -= friction * velMagnitude * deltaTime;
        if (velMagnitude < 0) velMagnitude = 0; 

        sf::Vector2u windowSize = window.getSize();

        // Wall collisions
        if (newPos.x < 0) {
            newPos.x = 0;
            velDirection.x = -velDirection.x;
        }
        if (newPos.x > windowSize.x - sprite.getGlobalBounds().width) { 
            newPos.x = windowSize.x - sprite.getGlobalBounds().width;
            velDirection.x = -velDirection.x;
        }
        if (newPos.y < 0) {
            newPos.y = 0;
            velDirection.y = -velDirection.y;
        }
        if (newPos.y > windowSize.y - sprite.getGlobalBounds().height) {
            newPos.y = windowSize.y - sprite.getGlobalBounds().height;
            velDirection.y = -velDirection.y;
        }

        float ballRadius = sprite.getGlobalBounds().width / 2;
        
        if (map.isObstacle(newPos.x, pos.y)) {
            velDirection.x = -velDirection.x;
            newPos.x = pos.x;
        }

        if (map.isObstacle(pos.x, newPos.y)) {
            velDirection.y = -velDirection.y;
            newPos.y = pos.y;
        }

        if (map.isObstacle(newPos.x + ballRadius, pos.y)) {
            velDirection.x = -velDirection.x;
            newPos.x = pos.x;
        }

        if (map.isObstacle(pos.x, newPos.y + ballRadius)) {
            velDirection.y = -velDirection.y;
            newPos.y = pos.y;
        }

        // if (map.isObstacle(newCenter.x, pos.y) == 1|| map.isObstacle(newCenter.x, pos.y) == 1 ||
        //     map.isObstacle(newCenter.x, pos.y + ballRadius.y * 2) == 1|| map.isObstacle(newCenter.x, pos.y - ballRadius.y * 2) == 1) {
        //     velDirection.x = -velDirection.x;
        //     newPos.x = pos.x;
        // }

        // if (map.isObstacle(pos.x, newCenter.y) == 1 || map.isObstacle(pos.x, newCenter.y) == 1 ||
        //     map.isObstacle(pos.x, newCenter.y + ballRadius.y * 2) == 1 || map.isObstacle(pos.x, newCenter.y - ballRadius.y * 2) == 1) {
        //     velDirection.y = -velDirection.y;
        //     newPos.y = pos.y;
        // }

        sprite.setPosition(newPos);
    }
}
