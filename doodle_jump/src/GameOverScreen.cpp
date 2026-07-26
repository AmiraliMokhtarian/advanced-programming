#include "GameOverScreen.hpp"

GameOverScreen::GameOverScreen(float windowWidth, ResourceManager<sf::Texture>& textures, const sf::Font& font)
{
    float centerX = windowWidth / 2.0f;

    restartButtonSprite.setTexture(textures.load("restart", "assets/restart_button.png"));
    sf::FloatRect rb = restartButtonSprite.getLocalBounds();
    restartButtonSprite.setOrigin(rb.width / 2.0f, rb.height / 2.0f);
    restartButtonSprite.setPosition(centerX, 460.f);

    menuButtonSprite.setTexture(textures.load("menu", "assets/menu_button.png"));
    sf::FloatRect mb = menuButtonSprite.getLocalBounds();
    menuButtonSprite.setOrigin(mb.width / 2.0f, mb.height / 2.0f);
    menuButtonSprite.setPosition(centerX, 550.f);

    gameOverTitleText.setFont(font);
    gameOverTitleText.setString("GAME OVER");
    gameOverTitleText.setCharacterSize(55);
    gameOverTitleText.setFillColor(sf::Color::Red);
    gameOverTitleText.setStyle(sf::Text::Bold);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(30);
    finalScoreText.setFillColor(sf::Color::Black);

    gameOverHighScoreText.setFont(font);
    gameOverHighScoreText.setCharacterSize(26);
    gameOverHighScoreText.setFillColor(sf::Color(0, 102, 204));
    gameOverHighScoreText.setStyle(sf::Text::Bold);
}

GameOverScreen::Action GameOverScreen::handleClick(sf::Vector2f mousePos) const
{
    if (restartButtonSprite.getGlobalBounds().contains(mousePos)) return Action::Restart;
    if (menuButtonSprite.getGlobalBounds().contains(mousePos))    return Action::BackToMenu;
    return Action::None;
}

void GameOverScreen::render(sf::RenderWindow& window, const sf::Sprite& background, float windowWidth,
                             int finalScore, Difficulty currentDifficulty, const HighScoreManager& highScoreMgr)
{
    window.draw(background);

    gameOverTitleText.setPosition((windowWidth - gameOverTitleText.getGlobalBounds().width) / 2.f, 150.f);
    window.draw(gameOverTitleText);

    finalScoreText.setString("Your Score: " + std::to_string(finalScore));
    finalScoreText.setPosition((windowWidth - finalScoreText.getGlobalBounds().width) / 2.f, 260.f);
    window.draw(finalScoreText);

    gameOverHighScoreText.setString("High Score (" + DifficultyConfig::name(currentDifficulty) + "): " +
                                    std::to_string(highScoreMgr.get(currentDifficulty)));
    gameOverHighScoreText.setPosition((windowWidth - gameOverHighScoreText.getGlobalBounds().width) / 2.f, 330.f);
    window.draw(gameOverHighScoreText);

    window.draw(restartButtonSprite);
    window.draw(menuButtonSprite);
}