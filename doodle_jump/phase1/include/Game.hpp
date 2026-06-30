#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "GameState.hpp"
#include "player.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void updateMenu(float dt);
    void updateGameplay(float dt);
    void updateGameOver(float dt);

    void renderMenu();
    void renderGameplay();
    void renderGameOver();

    sf::RenderWindow window;
    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;

    GameState currentState;
    Player player;
};