#include "../headers/ball.hpp"
#include <iostream>
#include <SFML/System.hpp>

Ball::Ball(int x, int y, int radius) : shape(radius), velMagnitude(0), friction(1.0f) {
    shape.setPosition(x, y);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Ball::setPos(int x, int y) {
    shape.setPosition(x, y);
}

sf::Vector2f Ball::getPos() {
    return shape.getPosition();
}

void Ball::move(float velMagnitude, sf::Vector2f velDirection) {
    this->velMagnitude = velMagnitude * 3.0f; 
    this->velDirection = velDirection; 
    std::cout << "Velocity Magnitude: " << this->velMagnitude << "\n";
    std::cout << "Velocity Direction: " << this->velDirection.x << " " << this->velDirection.y << "\n";
}

void Ball::update(float deltaTime) {
    if (velMagnitude > 0) {
        sf::Vector2f pos = shape.getPosition();
        pos += velDirection * velMagnitude * deltaTime;

        // Apply friction
        velMagnitude -= friction * velMagnitude * deltaTime;
        if (velMagnitude < 0) velMagnitude = 0;  // Ensure velocity doesn't go negative

        // Bounce off the walls
        if (pos.x < 0) {
            pos.x = 0;
            velDirection.x = -velDirection.x;
        }
        if (pos.x > 800 - shape.getRadius() * 2) { // Adjust for ball radius
            pos.x = 800 - shape.getRadius() * 2;
            velDirection.x = -velDirection.x;
        } 
        if (pos.y < 0) {
            pos.y = 0;
            velDirection.y = -velDirection.y;
        }
        if (pos.y > 600 - shape.getRadius() * 2) { // Adjust for ball radius
            pos.y = 600 - shape.getRadius() * 2;
            velDirection.y = -velDirection.y;
        }

        shape.setPosition(pos);
    }
}
