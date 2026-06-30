#include "Game.hpp"

Game::Game()
    : window(sf::VideoMode(600, 800), "Doodle Jump")
    , currentState(GameState::Gameplay)
    , player(textures.load("player" , "assets/right_doodle.png"))
{
    window.setFramerateLimit(60);
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
    player.update(dt);
}
void Game::updateGameOver(float dt) {}

void Game::renderMenu() {}
void Game::renderGameplay() 
{
    player.render(window);
}
void Game::renderGameOver() {}