#include "Game.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BrokenPlatform.hpp"
#include <cstdlib> //srand
#include <ctime>   //time

Game::Game()
    : window(sf::VideoMode(600, 800), "Doodle Jump")
    , currentState(GameState::Gameplay)
    , player(textures.load("player_left", "assets/left_doodle.png"), 
             textures.load("player_right", "assets/right_doodle.png"))
{
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    //loading textures of platforms
    textures.load("platform_normal", "assets/normal_platform.png");
    textures.load("platform_moving", "assets/moving_platform.png");
    textures.load("platform_broken", "assets/broken_platform.png");

    generateInitialPlatforms();
}

Game::~Game() 
{
    for (auto* platform : platforms) {
        delete platform; 
    }
    platforms.clear();
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

void Game::updateMenu(float dt) {}
void Game::updateGameplay(float dt) 
{
    player.handleInput();
    player.update(dt, window.getSize().x);

    for (auto* platform : platforms) {
        platform->update(dt); 
    }
}
void Game::updateGameOver(float dt) {}

void Game::renderMenu() {}

void Game::renderGameplay() 
{
    for (auto* platform : platforms) {
        platform->render(window);
    }
    player.render(window);
}
void Game::renderGameOver() {}

void Game::generateInitialPlatforms() {
    sf::Vector2f playerPos = player.getPosition();

    float firstPlatformX = playerPos.x - 20.f; 
    float firstPlatformY = playerPos.y + 60.f; 

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
        platforms.push_back(new NormalPlatform(textures.get("platform_normal"), sf::Vector2f(xPosition, yPosition)));
    } 
    else if (randType < 90) {
        platforms.push_back(new MovingPlatform(textures.get("platform_moving"), sf::Vector2f(xPosition, yPosition)));
    } 
    else {
        platforms.push_back(new BrokenPlatform(textures.get("platform_broken"), sf::Vector2f(xPosition, yPosition)));
    }
}