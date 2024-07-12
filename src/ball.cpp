#include "../headers/ball.hpp"
#include <iostream>
#include <SFML/System.hpp>
#include <math.h>

Ball::Ball(int x, int y, int radius, const std::string& ballTextureFile, const std::string& arrowTextureFile)
    : velMagnitude(0), friction(1.0f), powerIndicator(sf::Vector2f(0, 5.0f)), powerIndicatorBorder(sf::Vector2f(100.0f, 10.0f)) {
    if (!ballTexture.loadFromFile(ballTextureFile)) {
        std::cerr << "Error loading texture from file" << std::endl;
    }
    initialPos = sf::Vector2f(x, y);
    ballSprite.setTexture(ballTexture);
    ballSprite.setPosition(x, y);
    scaleFactor = static_cast<float>(radius * 2) / ballTexture.getSize().x;
    ballSprite.setScale(scaleFactor, scaleFactor);
    ballSprite.setOrigin(radius, radius);

    if (!arrowTexture.loadFromFile(arrowTextureFile)) {
        std::cerr << "Error loading arrow texture" << std::endl;
    }
    arrowSprite.setTexture(arrowTexture);
    arrowSprite.setPosition(x, y);
    arrowScaleFactor = 5.0f;
    arrowSprite.setScale(arrowScaleFactor, arrowScaleFactor);
    arrowSprite.setOrigin(radius, arrowTexture.getSize().y / 2.0f);

    powerIndicator.setFillColor(sf::Color::Green);
    powerIndicator.setOrigin(0, powerIndicator.getSize().y / 2);

    powerIndicatorBorder.setFillColor(sf::Color::Transparent);
    powerIndicatorBorder.setOutlineColor(sf::Color::White);
    powerIndicatorBorder.setOutlineThickness(1.0f);
    powerIndicatorBorder.setOrigin(powerIndicatorBorder.getSize().x / 2 - 5, powerIndicatorBorder.getSize().y / 2 - 15);
    
    powerIndicatorVertices.setPrimitiveType(sf::Quads);
    powerIndicatorVertices.resize(4);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(ballSprite);
    // window.draw(powerIndicatorBorder); 
    // window.draw(powerIndicator); 
}

void Ball::setPos(int x, int y) {
    ballSprite.setPosition(x, y);
}

sf::Vector2f Ball::getPos() {
    return ballSprite.getPosition();
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
            ballSprite.setScale(scaleFactor * scalingOnScore, scaleFactor * scalingOnScore);
        }
        sf::sleep(sf::seconds(1));
    }

    if (velMagnitude > 0) {
        sf::Vector2f pos = ballSprite.getPosition();
        sf::Vector2f newPos = pos + velDirection * velMagnitude * deltaTime;

        velMagnitude -= friction * velMagnitude * deltaTime;
        if (velMagnitude < 0) velMagnitude = 0; 

        sf::Vector2u windowSize = window.getSize();

        if (newPos.x < 0) {
            newPos.x = 0;
            velDirection.x = -velDirection.x;
        }
        if (newPos.x > windowSize.x - ballSprite.getGlobalBounds().width) { 
            newPos.x = windowSize.x - ballSprite.getGlobalBounds().width;
            velDirection.x = -velDirection.x;   
        }
        if (newPos.y < 0) {
            newPos.y = 0;
            velDirection.y = -velDirection.y;
        }
        if (newPos.y > windowSize.y - ballSprite.getGlobalBounds().height) {
            newPos.y = windowSize.y - ballSprite.getGlobalBounds().height;
            velDirection.y = -velDirection.y;
        }

        float ballRadius = ballSprite.getGlobalBounds().width / 2;

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

        if (map.isSand(newPos.x, newPos.y)) {
            friction = 100.0f;
        } else {
            friction = 1.0f;
        }

        if (map.isHole(newPos.x, newPos.y)) {
            velMagnitude = 0;
            newPos = map.getHoleCenter();
            scaling = true;
            isHoleComplete = true;
        }

        ballSprite.setPosition(newPos);
    }
}

bool Ball::getHoleStatus() {
    return isHoleComplete;
}

void Ball::setHoleStatus() {
    isHoleComplete = false;
}

void Ball::reset() {
    ballSprite.setPosition(initialPos.x, initialPos.y);
    scaling = false;
    ballSprite.setScale(scaleFactor, scaleFactor);
}

sf::Vector3f rgbToHsv(sf::Color color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;

    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    float delta = max - min;

    float h = 0.0f;
    if (delta > 0.0f) {
        if (max == r) {
            h = 60.0f * fmod((g - b) / delta, 6);
        } else if (max == g) {
            h = 60.0f * ((b - r) / delta + 2);
        } else {
            h = 60.0f * ((r - g) / delta + 4);
        }
    }

    if (h < 0) h += 360.0f;

    float s = (max == 0) ? 0 : delta / max;
    float v = max;

    return sf::Vector3f(h, s, v);
}

sf::Color hsvToRgb(sf::Vector3f hsv) {
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;

    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;

    if (0 <= h && h < 60) {
        r = c, g = x, b = 0;
    } else if (60 <= h && h < 120) {
        r = x, g = c, b = 0;
    } else if (120 <= h && h < 180) {
        r = 0, g = c, b = x;
    } else if (180 <= h && h < 240) {
        r = 0, g = x, b = c;
    } else if (240 <= h && h < 300) {
        r = x, g = 0, b = c;
    } else {
        r = c, g = 0, b = x;
    }

    return sf::Color(
        static_cast<sf::Uint8>((r + m) * 255),
        static_cast<sf::Uint8>((g + m) * 255),
        static_cast<sf::Uint8>((b + m) * 255)
    );
}

sf::Color lerpColorHSV(sf::Color start, sf::Color end, float t) {
    sf::Vector3f startHSV = rgbToHsv(start);
    sf::Vector3f endHSV = rgbToHsv(end);

    float h = startHSV.x + t * (endHSV.x - startHSV.x);
    float s = startHSV.y + t * (endHSV.y - startHSV.y);
    float v = startHSV.z + t * (endHSV.z - startHSV.z);

    return hsvToRgb(sf::Vector3f(h, s, v));
}


void Ball::updatePowerIndicator(float power, sf::Vector2f direction) {
    float arrowLength = power / 1500.0f;
    arrowSprite.setScale(sf::Vector2f(0.1f, 0.1f));
    arrowSprite.setPosition(ballSprite.getPosition() + sf::Vector2f(5.0f, 5.0f));
    arrowSprite.setRotation(atan2(direction.y, direction.x) * 180 / 3.14159);

    if (power > 1500) {
        power = 1500;
    }

    float maxPowerWidth = 100; 
    float powerWidth = (maxPowerWidth * power) / 1500.0f;

    sf::Vector2f position = ballSprite.getPosition();
    float height = 10.0f; 

    powerIndicatorVertices[0].position = sf::Vector2f(position.x - maxPowerWidth / 2 + 5, position.y + ballSprite.getGlobalBounds().height / 2 + 16);
    powerIndicatorVertices[1].position = sf::Vector2f(position.x - maxPowerWidth / 2 + 5 + powerWidth, position.y + ballSprite.getGlobalBounds().height / 2 + 16);
    powerIndicatorVertices[2].position = sf::Vector2f(position.x - maxPowerWidth / 2 + 5 + powerWidth, position.y + ballSprite.getGlobalBounds().height / 2 + 15 + height);
    powerIndicatorVertices[3].position = sf::Vector2f(position.x - maxPowerWidth / 2 + 5, position.y + ballSprite.getGlobalBounds().height / 2 + 15 + height);

    float t = power / 1500.0f;
    sf::Color color = lerpColorHSV(sf::Color::Green, sf::Color::Red, t);

    powerIndicatorVertices[0].color = color;
    powerIndicatorVertices[1].color = color;
    powerIndicatorVertices[2].color = color;
    powerIndicatorVertices[3].color = color;

    powerIndicatorBorder.setPosition(ballSprite.getPosition().x, 
                                     ballSprite.getPosition().y + ballSprite.getGlobalBounds().height / 2 + 5);
}

void Ball::drawIndicator(sf::RenderWindow& window) {
    window.draw(arrowSprite); 
    window.draw(powerIndicatorBorder);
    window.draw(&powerIndicatorVertices[0], powerIndicatorVertices.getVertexCount(), powerIndicatorVertices.getPrimitiveType()); 
}