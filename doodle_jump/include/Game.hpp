#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ResourceManager.hpp"
#include "GameState.hpp"
#include "player.hpp"
#include "platform.hpp"
#include "SettingMenu.hpp"
#include "SoundManager.hpp"
#include "Monster.hpp"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void resetGame();

    void updateMenu(float dt);
    void updateGameplay(float dt);
    void updateGameOver(float dt);
    void updateSettings(float dt);

    void renderMenu();
    void renderGameplay();
    void renderGameOver();
    void renderSettings();

    // helper
    void handleCollisions();
    void handleScrolling(float dt);
    void loadTextures();

    // generating platforms
    void generateInitialPlatforms();
    void spawnPlatform(float yPosition);
    void spawnMonster(float yPosition); 
    void clearMonsters();

    void initUI();

    sf::RenderWindow window;
    ResourceManager<sf::Texture> textures;
    sf::Font font; 
    sf::Font loadFont();

    GameState currentState;
    Player player;
    vector<Platform*> platforms;
    vector<Monster*> monsters;

    SoundManager soundManager;      
    SettingsMenu settingsMenu;      
    
    sf::Sprite settingsButtonSprite;
    sf::Sprite backgroundSprite;
    sf::Text scoreText;

    sf::Sprite startButtonSprite;
    sf::Text titleText;
    sf::Text highScoreText;
    int highScore = 0;
    
    sf::Sprite restartButtonSprite;
    sf::Sprite menuButtonSprite;
    sf::Text gameOverTitleText;
    sf::Text finalScoreText;
    sf::Text gameOverHighScoreText;
};