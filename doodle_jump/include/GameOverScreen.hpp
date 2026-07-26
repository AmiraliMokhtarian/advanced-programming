#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ResourceManager.hpp"
#include "Difficulty.hpp"
#include "HighScoreManager.hpp"

class GameOverScreen {
public:
    GameOverScreen(float windowWidth, ResourceManager<sf::Texture>& textures, const sf::Font& font);

    enum class Action { None, Restart, BackToMenu };
    Action handleClick(sf::Vector2f mousePos) const;

    void render(sf::RenderWindow& window, const sf::Sprite& background, float windowWidth,
                int finalScore, Difficulty currentDifficulty, const HighScoreManager& highScoreMgr);

private:
    sf::Sprite restartButtonSprite;
    sf::Sprite menuButtonSprite;
    sf::Text gameOverTitleText;
    sf::Text finalScoreText;
    sf::Text gameOverHighScoreText;
};