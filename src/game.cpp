#include "../headers/game.hpp"
#include <cmath>
#include <iostream>
#include <SFML/System.hpp>

GolfGame::GolfGame(sf::RenderWindow& window) : ball(390, 500, 10, "assets/golfBall.png", "assets/arrow.png"), map(18, 32, window, "assets/obstacle.png", "assets/hole.png") {
    loadLevel("levels/level5.txt");

    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error opening font file";
    }

    strokeText.setFont(font);
    strokeText.setCharacterSize(32);
    strokeText.setFillColor(sf::Color::White);
    strokeText.setPosition((window.getSize().x - strokeText.getLocalBounds().width - 110) / 2, 17); 

    holeText.setFont(font);
    holeText.setCharacterSize(32);
    holeText.setFillColor(sf::Color::White);
    holeText.setPosition((window.getSize().x - holeText.getLocalBounds().width - 80) / 2, 1017); 
    holeText.setString("Hole : 1");

    levelUpText.setFont(font);
    levelUpText.setCharacterSize(48);
    levelUpText.setFillColor(sf::Color::White);
    levelUpText.setPosition(400, 300);

    if (!splashBgTexture.loadFromFile("assets/splashBg.png")) {
        std::cerr << "Error opening texture file";
    }

    splashBg.setTexture(splashBgTexture);
    splashBg.setPosition((window.getSize().x - splashBg.getLocalBounds().width) / 2, -200); 

    splashBg2.setTexture(splashBgTexture);
    splashBg2.setPosition((window.getSize().x - splashBg.getLocalBounds().width) / 2, 800);
}

void GolfGame::draw(sf::RenderWindow& window) {
    //map.draw(); //removed draw from here so that the map can be drawn before the indicator to avoid it being hidden under the sand, possibly causing some nonsensical performance issues?
    ball.draw(window);

    strokeText.setString("Strokes: " + std::to_string(strokeCount));

    window.draw(splashBg);
    window.draw(strokeText);
    window.draw(splashBg2);
    window.draw(holeText);
}

void GolfGame::handlePress(sf::Event& event) {
    validClick = false;
    if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f curPos = ball.getPos();
        if (event.mouseButton.x <= curPos.x + 30 && event.mouseButton.x >= curPos.x - 30 
            && event.mouseButton.y <= curPos.y + 30 && event.mouseButton.y >= curPos.y - 30) {
            initialPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            validClick = true;
            isMousePressed = true; 
        }
    }
}

void GolfGame::handleRelease(sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left && validClick) {
        finalPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
        calculateVelocity();
        ball.move(velMagnitude, velDirection);
        strokeCount++;
        isMousePressed = false; 
        ball.updatePreviousPos(initialPos);
    }
}

void GolfGame::handleMouseMoved(sf::Event& event) {
    if (isMousePressed && validClick) {
        sf::Vector2f currentPos(event.mouseMove.x, event.mouseMove.y);
        velDirection = initialPos - currentPos;
        velMagnitude = std::sqrt(velDirection.x * velDirection.x + velDirection.y * velDirection.y);
        if (velMagnitude != 0) {
            velDirection /= velMagnitude;
        }
        ball.updatePowerIndicator(velMagnitude, velDirection); 
    }
}

void GolfGame::update(float deltaTime, sf::RenderWindow& window) {
    ball.update(deltaTime, window, map);
    if (ball.getHoleStatus()) {
        handleLevelUp(window);
        ball.setHoleStatus();
    }

    map.draw(); //aware that having the main draw out of the draw function is stupid but it works

    if (!isMousePressed) {
        ball.updatePowerIndicator(0, sf::Vector2f(0, 0));
    } else {
        ball.drawIndicator(window);
    }
}

void GolfGame::loadLevel(const std::string& filePath) {
    map.loadMapFromFile(filePath);
    ball.reset();
    strokeCount = 0;

    if (curLevel == 4) {
        ball.setPos(200, 560);
    }
}

void GolfGame::calculateVelocity() {
    velDirection = initialPos - finalPos;
    velMagnitude = std::sqrt(velDirection.x * velDirection.x + velDirection.y * velDirection.y);
    if (velMagnitude != 0) {
        velDirection /= velMagnitude;
    }
}

void GolfGame::handleLevelUp(sf::RenderWindow& window) {
    displayLevelUpText(window);
    sf::sleep(sf::seconds(1));
    curLevel++;
    if (curLevel <= 6) {
        loadLevel("levels/level" + std::to_string(curLevel) + ".txt");
    } else {
        exit(0);
    }
    ball.updatePowerIndicator(0, sf::Vector2f(0, 0));
    holeText.setString("Hole : " + std::to_string(curLevel));
}

void GolfGame::displayLevelUpText(sf::RenderWindow& window) {
    levelUpText.setString("Hole complete!");
    window.draw(levelUpText);
    window.display();
}

void GolfGame::showTitleScreen(sf::RenderWindow& window, float elapsedTime) {
    sf::Texture titleTexture;
    if (!titleTexture.loadFromFile("assets/title.png")) {
        std::cerr << "Error opening texture file";
    }

    sf::Sprite titleSprite(titleTexture);
    titleSprite.setPosition(500, 150);
    window.draw(titleSprite);

    float amplitude = 15.0f;
    float frequency = 2.0f;
    float offset = amplitude * std::sin(frequency * elapsedTime);

    titleText.setFont(font);
    titleText.setCharacterSize(54);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(790, 700 + offset);
    titleText.setString("Press Enter To Play!");
    window.draw(titleText);

    bottomText.setFont(font);
    bottomText.setCharacterSize(40);
    bottomText.setFillColor(sf::Color::White);
    bottomText.setPosition(150, 1000);
    bottomText.setString("Hold ESC to exit   |   Aim the ball using your mouse to score in fewest strokes   |   Made by @tyagidevansh");
    window.draw(bottomText);
}
