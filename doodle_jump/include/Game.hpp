#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "GameState.hpp"
#include "player.hpp"
#include "SettingMenu.hpp"
#include "SoundManager.hpp"
#include "Difficulty.hpp"
#include "SettingManager.hpp"
#include "HighScoreManager.hpp"
#include "MenuScreen.hpp"
#include "GameOverScreen.hpp"
#include "GameWorld.hpp"

class Game {
public:
    Game();
    ~Game() = default;
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

    void handleScrolling(float dt);
    void loadTextures();
    void initUI();

    void setDifficulty(Difficulty level);
    void updateSettingsFromMenu();

    sf::RenderWindow window;
    ResourceManager<sf::Texture> textures;
    sf::Font font;
    sf::Font loadFont();

    GameState currentState;
    GameState previousState = GameState::GameOver;

    ResourceManager<sf::SoundBuffer> soundBuffers;
    SettingsManager settingsManager;
    HighScoreManager highScoreMgr;
    SoundManager soundManager;

    Player player;
    SettingsMenu settingsMenu;
    MenuScreen menuScreen;
    GameOverScreen gameOverScreen;
    GameWorld world;

    sf::Sprite backgroundSprite;
    sf::Text scoreText;

    sf::Clock fireClock;

    Difficulty currentDifficulty = Difficulty::EASY;
    DifficultySettings settings = DifficultyConfig::getSettings(Difficulty::EASY);
    
};