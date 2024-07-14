#ifndef BALL_HPP
#define BALL_HPP

#include "../headers/map.hpp"
#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(int x, int y, int radius, const std::string& ballTextureFile, const std::string& arrowTextureFile);
    void draw(sf::RenderWindow& window);
    void setPos(int x, int y);
    sf::Vector2f getPos();
    void move(float velMagnitude, sf::Vector2f velDirection);
    void update(float deltaTime, const sf::RenderWindow& window, Map& map);
    void updatePosition(float deltaTime, const sf::RenderWindow& window, Map& map);
    void updateScaling(float deltaTime);
    void handleWindowCollisions(sf::Vector2f& newPos, const sf::RenderWindow& window);
    void handleObstacleCollisions(sf::Vector2f& newPos, const sf::Vector2f& pos, Map& map);
    bool getHoleStatus();
    void setHoleStatus();
    void reset();
    void updatePowerIndicator(float power, sf::Vector2f direction);
    void drawIndicator(sf::RenderWindow& window);
    void updatePreviousPos(sf::Vector2f pos);

private:
    sf::Vector2f initialPos;
    sf::Vector2f previousPos;
    sf::Sprite ballSprite;
    sf::Texture ballTexture;
    sf::Sprite arrowSprite;
    sf::Texture arrowTexture;
    float velMagnitude;
    sf::Vector2f velDirection;
    float friction;
    float scaleFactor;
    float arrowScaleFactor;
    bool scaling;
    float scalingOnScore;
    float scalingOnWater;
    bool isWater = false;
    bool isHoleComplete = false;
    sf::RectangleShape powerIndicator; 
    sf::RectangleShape powerIndicatorBorder;
    sf::VertexArray powerIndicatorVertices;
};

#endif // BALL_HPP
