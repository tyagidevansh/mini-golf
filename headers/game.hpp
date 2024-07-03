#ifndef GAME_HPP
#define GAME_HPP

#include "ball.hpp"
#include "map.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class GolfGame {
public:
    GolfGame(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void handlePress(sf::Event& event);
    void handleRelease(sf::Event& event);
    void update(float deltaTime, const sf::RenderWindow& window);
    void loadLevel(const std::string& filePath);

private:
    Ball ball;
    Map map;
    sf::Font font;
    sf::Text strokeText;
    sf::Text levelUpText;
    int strokeCount;
    bool levelUp;
    int curLevel = 1;
    bool validClick;
    sf::Vector2f initialPos;
    sf::Vector2f finalPos;
    sf::Vector2f velDirection;
    float velMagnitude;

    void calculateVelocity();
    void handleLevelUp();
};

#endif // GAME_HPP
