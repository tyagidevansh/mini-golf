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

void Ball::update(float deltaTime, const sf::RenderWindow& window) {
    if (velMagnitude > 0) {
        sf::Vector2f pos = sprite.getPosition();
        pos += velDirection * velMagnitude * deltaTime;

        velMagnitude -= friction * velMagnitude * deltaTime;
        if (velMagnitude < 0) velMagnitude = 0; 

        sf::Vector2u windowSize = window.getSize();

        if (pos.x < 0) {
            pos.x = 0;
            velDirection.x = -velDirection.x;
        }
        if (pos.x > windowSize.x - sprite.getGlobalBounds().width) { 
            pos.x = windowSize.x - sprite.getGlobalBounds().height;
            velDirection.x = -velDirection.x;
        }
        if (pos.y < 0) {
            pos.y = 0;
            velDirection.y = -velDirection.y;
        }
        if (pos.y > windowSize.y - sprite.getGlobalBounds().height) { 
            pos.y = windowSize.y - sprite.getGlobalBounds().height;
            velDirection.y = -velDirection.y;
        }

        sprite.setPosition(pos);
    }
}
