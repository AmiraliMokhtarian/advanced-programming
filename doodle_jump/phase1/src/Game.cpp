#include "Game.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BrokenPlatform.hpp"
#include <cstdlib> //srand
#include <ctime>   //time
#include <iostream>
#include <stdexcept>

Game::Game()
    : window(sf::VideoMode(600, 800), "Doodle Jump")
    , currentState(GameState::Menu)
    , player(textures.load("player_left", "assets/left_doodle.png"), 
             textures.load("player_right", "assets/right_doodle.png"))
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    loadTextures();
    initUI();
    generateInitialPlatforms();
}

Game::~Game() 
{
    for (auto* platform : platforms) {
        delete platform; 
    }
    platforms.clear();
}

void Game::loadTextures()
{                                   
    textures.load("background", "assets/background.png");
    textures.load("platform_normal", "assets/normal_platform.png");
    textures.load("platform_moving", "assets/moving_platform.png");
    textures.load("platform_broken", "assets/broken_platform.png");
    textures.load("spring", "assets/spring_sprite.png");
    textures.load("start_button", "assets/start_button.png");

    backgroundSprite.setTexture(textures.get("background"));

    startButtonSprite.setTexture(textures.get("start_button"));
    float buttonX = (600.f - startButtonSprite.getGlobalBounds().width) / 2.f;
    float buttonY = 400.f;
    startButtonSprite.setPosition(buttonX, buttonY);
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
    //while any new event happend:
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(float dt) 
{
    switch (currentState) {
        case GameState::Menu    : updateMenu(dt)    ; break;
        case GameState::Gameplay: updateGameplay(dt); break;
        case GameState::GameOver: updateGameOver(dt); break;
    }
}

void Game::render() 
{
    window.clear(sf::Color(240, 240, 250));
    switch (currentState) {
        case GameState::Menu    : renderMenu()    ; break;
        case GameState::Gameplay: renderGameplay(); break;
        case GameState::GameOver: renderGameOver(); break;
    }
    window.display();
}

void Game::updateMenu(float dt) 
{
    //left_click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (startButtonSprite.getGlobalBounds().contains(mousePosF)) 
        {
            currentState = GameState::Gameplay;
        }
    }
}


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

    if (player.getPosition().y > 800.f) {
        if (player.getScore() > highScore) {
            highScore = player.getScore(); 
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
            //any collision with spring?
            if (platform->checkSpringCollision(player.getBounds())) 
            {
                player.setVelocity(sf::Vector2f(player.getVelocity().x, -900.f)); 
                return;
            }

            //is there any collision between player and platform?
            if (player.getBounds().intersects(platform->getBounds())){

                //ensure the player is landing on top of the platform
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

        //player couldn't jump upper than mid page:
        player.setPosition(sf::Vector2f(playerPos.x, scrollThreshold));

        int points = offsetY / 20;
        if (points < 1) points = 1;
        player.addScore(points);

        for (auto it = platforms.begin(); it != platforms.end(); )
        {
            Platform* platform = *it;
            
            platform->scroll(offsetY);

            if (platform->getPosition().y > 800.f)
            {
                delete platform;
                it = platforms.erase(it); //memory management
            }
            else
            {
                ++it;
            }
        }

        while (platforms.size() < 12) 
        {
            float highestY = 800.f;
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


void Game::updateGameOver(float dt) {}

void Game::renderMenu() 
{
    window.clear();
    window.draw(backgroundSprite);

    window.draw(titleText);
    window.draw(startButtonSprite);

    highScoreText.setString("High Score: " + std::to_string(highScore));
    float hsX = (600.f - highScoreText.getGlobalBounds().width) / 2.f;
    highScoreText.setPosition(hsX, 530.f);
    window.draw(highScoreText);
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
void Game::renderGameOver() {}

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
        std::cerr << "Error: Could not load font from assets/ariblk.ttf!" << std::endl;
        throw std::runtime_error("Failed to load critical game font.");
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24); 
    scoreText.setFillColor(sf::Color::Black); 
    scoreText.setPosition(20.f, 20.f); 

    //main menu:
    titleText.setFont(font);
    titleText.setString("DOODLE JUMP");
    titleText.setCharacterSize(55); 
    titleText.setFillColor(sf::Color::Black);
    titleText.setStyle(sf::Text::Bold);
    float titleX = (600.f - titleText.getGlobalBounds().width) / 2.f;
    titleText.setPosition(titleX, 200.f); 

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(26);            
    highScoreText.setFillColor(sf::Color(0, 102, 204)); 
    highScoreText.setStyle(sf::Text::Bold);
    highScoreText.setPosition(200.f, 530.f);
}