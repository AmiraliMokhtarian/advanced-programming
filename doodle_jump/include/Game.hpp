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
#include "Bullet.hpp"
#include "Difficulty.hpp"
#include "SettingManager.hpp"
#include "HighScoreManager.hpp"

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
    GameState previousState = GameState::GameOver;
    Player player;
    vector<Platform*> platforms;
    vector<Monster*> monsters;

    ResourceManager<sf::SoundBuffer> soundBuffers;
    SettingsManager settingsManager;
    HighScoreManager highScoreMgr;
    SoundManager soundManager;      
    SettingsMenu settingsMenu;      
    
    sf::Sprite settingsButtonSprite;
    sf::Sprite backgroundSprite;
    sf::Text scoreText;

    sf::Sprite startButtonSprite;
    sf::Text titleText;
    sf::Text highScoreText;
    
    sf::Sprite restartButtonSprite;
    sf::Sprite menuButtonSprite;
    sf::Text gameOverTitleText;
    sf::Text finalScoreText;
    sf::Text gameOverHighScoreText;

    vector<Bullet*> bullets;
    sf::Clock fireClock;

    Difficulty currentDifficulty = Difficulty::EASY;
    DifficultySettings settings = DifficultyConfig::getSettings(Difficulty::EASY);

    void setDifficulty(Difficulty level);
    void updateSettingsFromMenu();
};