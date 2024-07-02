#include "../headers/ball.hpp"
#include <iostream>
#include <SFML/System.hpp>

Ball::Ball(int x, int y, int radius, const std::string& textureFile) : velMagnitude(0), friction(1.0f) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error loading texture from file" << std::endl;
    }
    initialPos = sf::Vector2f(x, y);
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    scaleFactor = static_cast<float>(radius * 2) / texture.getSize().x;
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
}

void Ball::update(float deltaTime, const sf::RenderWindow& window, Map& map) {
    float scalingOnScore = scaleFactor;
    if (scaling) {
        while (scalingOnScore != 0) {
            scalingOnScore -= deltaTime;
            if (scalingOnScore <= 0) {
                scalingOnScore = 0;
            }
            sprite.setScale(scaleFactor * scalingOnScore, scaleFactor * scalingOnScore);
        }
        sf::sleep(sf::seconds(1));
    }

    if (velMagnitude > 0) {
        sf::Vector2f pos = sprite.getPosition();
        sf::Vector2f newPos = pos + velDirection * velMagnitude * deltaTime;

        velMagnitude -= friction * velMagnitude * deltaTime;
        if (velMagnitude < 0) velMagnitude = 0; 

        sf::Vector2u windowSize = window.getSize();


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
    sprite.setPosition(initialPos.x, initialPos.y);
    scaling = false;
    sprite.setScale(scaleFactor, scaleFactor);
}