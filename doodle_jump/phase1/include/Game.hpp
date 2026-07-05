#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "GameState.hpp"
#include "player.hpp"
#include "platform.hpp"

class Game {
public:
    Game();
    ~Game();
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
    
    //helper
    void handleCollisions();
    void handleScrolling(float dt);
    void loadTextures();

    //generating platforms
    void generateInitialPlatforms();
    void spawnPlatform(float yPosition);

    sf::RenderWindow window;
    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;

    GameState currentState;
    Player player;
    vector<Platform*> platforms;

    sf::Sprite backgroundSprite;
    sf::Font font;
    sf::Text scoreText;
    void initUI();

    sf::Sprite startButtonSprite;
    sf::Text titleText;
    sf::Text highScoreText;
    int highScore = 0;
};