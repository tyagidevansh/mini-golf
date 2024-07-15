#include "../headers/game.hpp"
#include <cmath>
#include <iostream>
#include <SFML/System.hpp>

GolfGame::GolfGame(sf::RenderWindow& window) 
    : ball(390, 500, 10, "assets/golfBall.png", "assets/arrow.png"), 
      map(18, 32, window, "assets/obstacle.png", "assets/hole.png") {
    loadLevel("levels/level1.txt");

    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Error opening font file";
    }

    if (!bgMusic.openFromFile("assets/bg.ogg")) {
        std::cerr << "Failed to load background music" << std::endl;
    }

    if (!scoreSoundBuffer.loadFromFile("assets/clap.ogg")) {
        std::cerr << "Failed to load score sound effect" << std::endl;
    }
    scoreSound.setBuffer(scoreSoundBuffer);    

    updateTextPositions(window);

    if (!splashBgTexture.loadFromFile("assets/splashBg.png")) {
        std::cerr << "Error opening texture file";
    }

    splashBg.setTexture(splashBgTexture);
    splashBg.setPosition((window.getSize().x - splashBg.getLocalBounds().width) / 2, -200);

    splashBg2.setTexture(splashBgTexture);
    splashBg2.setPosition((window.getSize().x - splashBg.getLocalBounds().width) / 2, 800);

    if (!starTexture.loadFromFile("assets/star.png")) {
        std::cerr << "Error opening texture file";
    }

    float starScale = 0.25f;
    star1.setTexture(starTexture);
    star1.setPosition(window.getSize().x * 0.3f, window.getSize().y * 0.3f);
    star1.setScale(starScale, starScale);

    star2.setTexture(starTexture);
    star2.setPosition(window.getSize().x * 0.42f, window.getSize().y * 0.3f);
    star2.setScale(starScale, starScale);

    star3.setTexture(starTexture);
    star3.setPosition(window.getSize().x * 0.54f, window.getSize().y * 0.3f);
    star3.setScale(starScale, starScale);

    if (!boardTexture.loadFromFile("assets/board.png")) {
        std::cerr << "Error loading texture file";
    }

    board.setTexture(boardTexture);
    board.setPosition(window.getSize().x * 0.3f, window.getSize().y * 0.45f);

    bgMusic.setVolume(20);
    bgMusic.play();
    bgMusic.setLoop(true);
}

void GolfGame::updateTextPositions(const sf::RenderWindow& window) {
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);

    strokeText.setFont(font);
    strokeText.setCharacterSize(32);
    strokeText.setFillColor(sf::Color::White);
    strokeText.setPosition((windowWidth - strokeText.getLocalBounds().width - 110) / 2, windowHeight * 0.017f);

    holeText.setFont(font);
    holeText.setCharacterSize(32);
    holeText.setFillColor(sf::Color::White);
    holeText.setPosition((windowWidth - holeText.getLocalBounds().width - 80) / 2, windowHeight * 0.945f);
    holeText.setString("Hole : 1");

    levelUpText.setFont(font);
    levelUpText.setCharacterSize(55);
    levelUpText.setFillColor(sf::Color::White);
    levelUpText.setPosition(windowWidth * 0.35f, windowHeight * 0.5f);
}

void GolfGame::draw(sf::RenderWindow& window) {
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
       // bgMusic.pause();
        scoreSound.play();
        handleLevelUp(window);
        ball.setHoleStatus();
    }
    
    map.draw(); 

    if (!isMousePressed) {
        ball.updatePowerIndicator(0, sf::Vector2f(0, 0));
    } else {
        ball.drawIndicator(window);
    }
}

void GolfGame::loadLevel(const std::string& filePath) {
    bgMusic.play();
    map.loadMapFromFile(filePath);
    ball.reset();
    strokeCount = 0;

    if (curLevel == 4) {
        ball.setPos(200, 560);
    } else if (curLevel == 5) {
       ball.setPos(390, 560);
    } else if (curLevel == 7) {
        ball.setPos(40, 1000);
    } else if (curLevel == 8 || curLevel == 9) {
        ball.setPos(40, 40);
    } else if (curLevel == 10) {
        ball.setPos(80, 920);
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
    sf::sleep(sf::seconds(3));
    curLevel++;
    if (curLevel <= 10) {
        loadLevel("levels/level" + std::to_string(curLevel) + ".txt");
    } else {
        window.clear();
        showEndScreen(window);
        sf::sleep(sf::seconds(10));
        exit(0);
    }
    ball.updatePowerIndicator(0, sf::Vector2f(0, 0));
    holeText.setString("Hole : " + std::to_string(curLevel));
}

void GolfGame::displayLevelUpText(sf::RenderWindow& window) {
    window.draw(star1);
    if (strokeCount < 5) window.draw(star2);
    if (strokeCount < 3) window.draw(star3);

    window.draw(board);
    totalStrokes += strokeCount;
    levelUpText.setString("Level completed! \n Strokes : " + std::to_string(strokeCount) + " \n Total Stokes: " + std::to_string(totalStrokes));
    window.draw(levelUpText);
    window.display();
}

void GolfGame::showTitleScreen(sf::RenderWindow& window, float elapsedTime) {
    sf::Texture titleTexture;
    if (!titleTexture.loadFromFile("assets/title.png")) {
        std::cerr << "Error opening texture file";
    }

    sf::Sprite titleSprite(titleTexture);
    titleSprite.setPosition(window.getSize().x * 0.5f - titleTexture.getSize().x * 0.5f, window.getSize().y * 0.15f);
    window.draw(titleSprite);

    float amplitude = 15.0f;
    float frequency = 2.0f;
    float offset = amplitude * std::sin(frequency * elapsedTime);

    titleText.setFont(font);
    titleText.setCharacterSize(54);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(window.getSize().x * 0.5f - titleText.getLocalBounds().width * 0.5f, window.getSize().y * 0.7f + offset);
    titleText.setString("Press Enter To Play!");
    window.draw(titleText);

    bottomText.setFont(font);
    bottomText.setCharacterSize(40);
    bottomText.setFillColor(sf::Color::White);
    bottomText.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.9f);
    bottomText.setString("Press ESC to exit   |   Aim the ball using your mouse to score in fewest strokes   |   Made by @tyagidevansh");
    window.draw(bottomText);
}

void GolfGame::showEndScreen(sf::RenderWindow& window) {
    sf::Text endText;
    endText.setFont(font);
    endText.setCharacterSize(48);
    endText.setFillColor(sf::Color::White);
    endText.setString("You have completed all the levels! \n Total Strokes : " + std::to_string(totalStrokes));
    endText.setPosition((window.getSize().x - endText.getLocalBounds().width) / 2, window.getSize().y * 0.4f);

    sf::Text screenshotText;
    screenshotText.setFont(font);
    screenshotText.setCharacterSize(36);
    screenshotText.setFillColor(sf::Color::White);
    screenshotText.setString("If you did well, take a screenshot of total strokes and send it to me!");
    screenshotText.setPosition((window.getSize().x - screenshotText.getLocalBounds().width) / 2, window.getSize().y * 0.55f);

    sf::Text closeText;
    closeText.setFont(font);
    closeText.setCharacterSize(30);
    closeText.setFillColor(sf::Color::White);
    closeText.setString("Game will auto-exit in a few seconds");
    closeText.setPosition((window.getSize().x - closeText.getLocalBounds().width) / 2, window.getSize().y * 0.7f);

    window.draw(endText);
    window.draw(screenshotText);
    window.draw(closeText);
    window.display();
}