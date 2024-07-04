#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "ball.hpp"
#include "map.hpp"

class GolfGame {
private:
    Ball ball;
    sf::Vector2f initialPos;
    sf::Vector2f finalPos;
    float velMagnitude;
    sf::Vector2f velDirection;
    Map map;
    bool validClick = false;
    int curLevel = 1;
public:
    GolfGame(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void handlePress(sf::Event& event);
    void handleRelease(sf::Event& event);
    void calculateVelocity();
    void update(float deltaTime, const sf::RenderWindow& window);
    void loadLevel(const std::string& filePath);
    void handleLevelUp();
    int strokeCount = 0;
    sf::Font font;
    sf::Text strokeText;
    sf::Text levelUpText;
};

#endif // GAME_HPP