#include "Game.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BrokenPlatform.hpp"
#include "Constants.hpp"
#include <cstdlib> // srand
#include <ctime>   // time
#include <iostream>
#include <stdexcept>
#include <fstream>

Game::Game()
    : window(sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT), "Doodle Jump")
    , currentState(GameState::Menu)
    , font(loadFont())
    , player(textures.load("player_left", "assets/left_doodle.png"), 
             textures.load("player_right", "assets/right_doodle.png"))
    , settingsMenu(static_cast<float>(Config::Window::WIDTH), static_cast<float>(Config::Window::HEIGHT), textures, font)
{
    window.setFramerateLimit(Config::Window::FPS_LIMIT);
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    initUI();        
    loadTextures();
    generateInitialPlatforms();

    std::ifstream inputFile("highscore.txt");
    if (inputFile.is_open()) {
        inputFile >> highScore;
        inputFile.close();
    } else {
        highScore = 0;
    }
}

Game::~Game() 
{
    for (auto* platform : platforms) {
        delete platform; 
    }
    platforms.clear();
}

sf::Font Game::loadFont() {
    sf::Font f;
    if (!f.loadFromFile("fonts/ariblk.ttf")) {
        cerr << "Failed to load font!" << std::endl;
    }
    return f;
}

void Game::loadTextures()
{                                      
    textures.load("background", "assets/background.png");
    textures.load("platform_normal", "assets/normal_platform.png");
    textures.load("platform_moving", "assets/moving_platform.png");
    textures.load("platform_broken", "assets/broken_platform.png");
    textures.load("spring", "assets/spring_sprite.png");
    textures.load("start_button", "assets/start_button.png");
    textures.load("restart", "assets/restart_button.png");
    textures.load("menu", "assets/menu_button.png");
    
    textures.load("settings_button", "assets/Settings_button.png");

    backgroundSprite.setTexture(textures.get("background"));

    float centerX = Config::Window::WIDTH / 2.0f;

    // Start Button
    startButtonSprite.setTexture(textures.get("start_button"));
    sf::FloatRect sb = startButtonSprite.getLocalBounds();
    startButtonSprite.setOrigin(sb.width / 2.0f, sb.height / 2.0f);
    startButtonSprite.setPosition(centerX, 360.f);

    // Settings Button
    settingsButtonSprite.setTexture(textures.get("settings_button"));
    settingsButtonSprite.setScale(0.5f, 0.5f); 
    sf::FloatRect stb = settingsButtonSprite.getLocalBounds();
    settingsButtonSprite.setOrigin(stb.width / 2.0f, stb.height / 2.0f);
    settingsButtonSprite.setPosition(centerX, 450.f);

    // Restart Button
    restartButtonSprite.setTexture(textures.get("restart"));
    sf::FloatRect rb = restartButtonSprite.getLocalBounds();
    restartButtonSprite.setOrigin(rb.width / 2.0f, rb.height / 2.0f);
    restartButtonSprite.setPosition(centerX, 460.f);

    // Menu Button
    menuButtonSprite.setTexture(textures.get("menu"));
    sf::FloatRect mb = menuButtonSprite.getLocalBounds();
    menuButtonSprite.setOrigin(mb.width / 2.0f, mb.height / 2.0f);
    menuButtonSprite.setPosition(centerX, 550.f);
}

void Game::run() 
{
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() 
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (currentState == GameState::Settings) {
            bool backPressed = settingsMenu.handleEvent(event, window);
            
            soundManager.setVolume(settingsMenu.getVolume());

            if (backPressed) {
                currentState = GameState::Menu;
            }
        }
    }
}

void Game::update(float dt) 
{
if (currentState == GameState::Menu || currentState == GameState::Settings) {
        if (!soundManager.isMusicPlaying()) { 
            soundManager.playMusic("sounds/MainMenu_Song.flac");
        }
    } else {
        soundManager.stopMusic(); 
    }
    switch (currentState) {
        case GameState::Menu    : updateMenu(dt)    ; break;
        case GameState::Settings: updateSettings(dt); break; 
        case GameState::Gameplay: updateGameplay(dt); break;
        case GameState::GameOver: updateGameOver(dt); break;
    }
}

void Game::render() 
{
    window.clear(sf::Color(240, 240, 250));
    switch (currentState) {
        case GameState::Menu    : renderMenu()    ; break;
        case GameState::Settings: renderSettings(); break; 
        case GameState::Gameplay: renderGameplay(); break;
        case GameState::GameOver: renderGameOver(); break;
    }
    window.display();
}

void Game::updateMenu(float dt) 
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (startButtonSprite.getGlobalBounds().contains(mousePosF)) 
        {
            currentState = GameState::Gameplay;
        }
        else if (settingsButtonSprite.getGlobalBounds().contains(mousePosF))
        {
            currentState = GameState::Settings;
        }
    }
}

void Game::updateSettings(float dt) {}

void Game::updateGameplay(float dt) 
{
    player.handleInput();
    player.update(dt, window.getSize().x);

    for (auto* platform : platforms) {
        platform->update(dt); 
    }
    handleCollisions();
    handleScrolling(dt);

    scoreText.setString("Score: " + std::to_string(player.getScore()));

    if (player.getPosition().y > float(Config::Window::HEIGHT)) {
        if (player.getScore() > highScore) {
            highScore = player.getScore(); 
        
            std::ofstream outputFile("highscore.txt");
            if (outputFile.is_open()) {
                outputFile << highScore;
                outputFile.close();
            }
        }
        currentState = GameState::GameOver;
    }
}

void Game::handleCollisions()
{
    if (player.getVelocity().y > 0.f) 
    {
        for (auto* platform : platforms) 
        {
            if (platform->checkSpringCollision(player.getBounds())) 
            {
                player.setVelocity(sf::Vector2f(player.getVelocity().x, -900.f)); 
                platform->triggerSpring();
                return;
            }

            if (player.getBounds().intersects(platform->getBounds())){
                if (player.getBounds().top + player.getBounds().height <= platform->getBounds().top + 20.f){
                    BrokenPlatform* broken = dynamic_cast<BrokenPlatform*>(platform);
                    if (broken) {
                        broken->breakPlatform();
                        player.bounce();
                    } 
                    else {
                        player.bounce(); 
                    }

                    break; 
                }
            }
        }
    }
}

void Game::handleScrolling(float dt) 
{
    sf::Vector2f playerPos = player.getPosition();
    float scrollThreshold = 400.f; 

    if (playerPos.y < scrollThreshold)
    {
        float offsetY = scrollThreshold - playerPos.y;
        player.setPosition(sf::Vector2f(playerPos.x, scrollThreshold));

        int points = offsetY / 20;
        if (points < 1) points = 1;
        player.addScore(points);

        for (auto it = platforms.begin(); it != platforms.end(); )
        {
            Platform* platform = *it;
            platform->scroll(offsetY);

            if (platform->getPosition().y > float(Config::Window::HEIGHT))
            {
                delete platform;
                it = platforms.erase(it);
            }
            else
            {
                ++it;
            }
        }

        while (platforms.size() < 12) 
        {
            float highestY = float(Config::Window::HEIGHT);
            for (auto* p : platforms) {
                if (p->getPosition().y < highestY) {
                    highestY = p->getPosition().y;
                }
            }
            
            float newSpawnY = highestY - (70.f + std::rand() % 60);
            spawnPlatform(newSpawnY);
        }
    }
}

void Game::updateGameOver(float dt) 
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (restartButtonSprite.getGlobalBounds().contains(mousePosF)) 
        {
            for (auto* platform : platforms) {
                delete platform;
            }
            platforms.clear();

            player.setPosition(sf::Vector2f(300.f, 600.f));
            player.setVelocity(sf::Vector2f(0.f, 0.f));
            player.resetScore();

            generateInitialPlatforms();
            currentState = GameState::Gameplay;
        }
        else if (menuButtonSprite.getGlobalBounds().contains(mousePosF)) 
        {
            currentState = GameState::Menu;
        }
    }
}

void Game::renderMenu() 
{
    window.clear();
    window.draw(backgroundSprite);

    window.draw(titleText);
    window.draw(startButtonSprite);
    window.draw(settingsButtonSprite); 
    highScoreText.setString("High Score: " + std::to_string(highScore));
    float hsX = (float(Config::Window::WIDTH) - highScoreText.getGlobalBounds().width) / 2.f;
    highScoreText.setPosition(hsX, 220.f);
    window.draw(highScoreText);
}

void Game::renderSettings()
{
    window.clear();
    window.draw(backgroundSprite); 
    settingsMenu.render(window);   
}

void Game::renderGameplay() 
{
    window.clear();
    window.draw(backgroundSprite);

    for (auto* platform : platforms) {
        platform->render(window);
    }
    player.render(window);
    window.draw(scoreText);
}

void Game::renderGameOver() 
{
    window.clear();
    window.draw(backgroundSprite);

    gameOverTitleText.setPosition((float(Config::Window::WIDTH) - gameOverTitleText.getGlobalBounds().width) / 2.f, 150.f);
    window.draw(gameOverTitleText);

    finalScoreText.setString("Your Score: " + std::to_string(player.getScore()));
    finalScoreText.setPosition((float(Config::Window::WIDTH) - finalScoreText.getGlobalBounds().width) / 2.f, 260.f);
    window.draw(finalScoreText);

    gameOverHighScoreText.setString("High Score: " + std::to_string(highScore));
    gameOverHighScoreText.setPosition((float(Config::Window::WIDTH) - gameOverHighScoreText.getGlobalBounds().width) / 2.f, 330.f);
    window.draw(gameOverHighScoreText);

    window.draw(restartButtonSprite);
    window.draw(menuButtonSprite);
}

void Game::generateInitialPlatforms() {
    sf::Vector2f playerPos = player.getPosition();

    float firstPlatformX = playerPos.x - 20.f; 
    float firstPlatformY = playerPos.y + 80.f; 

    platforms.push_back(new NormalPlatform(textures.get("platform_normal"), sf::Vector2f(firstPlatformX, firstPlatformY)));

    float currentY = firstPlatformY - (70.f + std::rand() % 80); 
    while (currentY > 0.f) {
        spawnPlatform(currentY);
        float gapY = 70.f + static_cast<float>(std::rand() % 80);
        currentY -= gapY;
    }
}

void Game::spawnPlatform(float yPosition) {
    float xPosition = static_cast<float>(std::rand() % (600 - 60)); 
    int randType = std::rand() % 100;

    if (randType < 70) {
        NormalPlatform* np = new NormalPlatform(textures.get("platform_normal"), sf::Vector2f(xPosition, yPosition));        
        if (std::rand() % 100 < 10) {
            np->addSpring(textures.get("spring"));
        }
        platforms.push_back(np);
    } 
    else if (randType < 90) {
        platforms.push_back(new MovingPlatform(textures.get("platform_moving"), sf::Vector2f(xPosition, yPosition)));
    } 
    else {
        platforms.push_back(new BrokenPlatform(textures.get("platform_broken"), sf::Vector2f(xPosition, yPosition)));
    }
}

void Game::initUI() 
{
    if (!font.loadFromFile("fonts/ariblk.ttf")) 
    {
        std::cerr << "Error: Could not load font from fonts/ariblk.ttf!" << std::endl;
        throw std::runtime_error("Failed to load critical game font.");
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24); 
    scoreText.setFillColor(sf::Color::Black); 
    scoreText.setPosition(20.f, 20.f); 

    // main menu:
    titleText.setFont(font);
    titleText.setString("DOODLE JUMP");
    titleText.setCharacterSize(55); 
    titleText.setFillColor(sf::Color::Black);
    titleText.setStyle(sf::Text::Bold);
    float titleX = (float(Config::Window::WIDTH) - titleText.getGlobalBounds().width) / 2.f;
    titleText.setPosition(titleX, 150.f); 

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(26);            
    highScoreText.setFillColor(sf::Color(0, 102, 204)); 
    highScoreText.setStyle(sf::Text::Bold);
    highScoreText.setPosition(200.f, 530.f);

    // game over
    gameOverTitleText.setFont(font);
    gameOverTitleText.setString("GAME OVER");
    gameOverTitleText.setCharacterSize(55);
    gameOverTitleText.setFillColor(sf::Color::Red);
    gameOverTitleText.setStyle(sf::Text::Bold);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(30);
    finalScoreText.setFillColor(sf::Color::Black);

    gameOverHighScoreText.setFont(font);
    gameOverHighScoreText.setCharacterSize(26);
    gameOverHighScoreText.setFillColor(sf::Color(0, 102, 204));
    gameOverHighScoreText.setStyle(sf::Text::Bold);
}