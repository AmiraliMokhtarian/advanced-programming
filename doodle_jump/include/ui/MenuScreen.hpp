#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ResourceManager.hpp"
#include "Difficulty.hpp"
#include "HighScoreManager.hpp"

class MenuScreen {
public:
    MenuScreen(float windowWidth, ResourceManager<sf::Texture>& textures, const sf::Font& font);

    enum class Action { None, StartGame, OpenSettings };
    Action handleClick(sf::Vector2f mousePos) const;

    void render(sf::RenderWindow& window, const sf::Sprite& background, float windowWidth,
                Difficulty currentDifficulty, const HighScoreManager& highScoreMgr);

private:
    sf::Sprite startButtonSprite;
    sf::Sprite settingsButtonSprite;
    sf::Text titleText;
    sf::Text highScoreText;
};