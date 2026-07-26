#include "MenuScreen.hpp"

MenuScreen::MenuScreen(float windowWidth, ResourceManager<sf::Texture>& textures, const sf::Font& font)
{
    float centerX = windowWidth / 2.0f;

    startButtonSprite.setTexture(textures.load("start_button", "assets/start_button.png"));
    sf::FloatRect sb = startButtonSprite.getLocalBounds();
    startButtonSprite.setOrigin(sb.width / 2.0f, sb.height / 2.0f);
    startButtonSprite.setPosition(centerX, 360.f);

    settingsButtonSprite.setTexture(textures.load("settings_button", "assets/Settings_button.png"));
    settingsButtonSprite.setScale(0.5f, 0.5f);
    sf::FloatRect stb = settingsButtonSprite.getLocalBounds();
    settingsButtonSprite.setOrigin(stb.width / 2.0f, stb.height / 2.0f);
    settingsButtonSprite.setPosition(centerX, 450.f);

    titleText.setFont(font);
    titleText.setString("DOODLE JUMP");
    titleText.setCharacterSize(55);
    titleText.setFillColor(sf::Color::Black);
    titleText.setStyle(sf::Text::Bold);
    float titleX = (windowWidth - titleText.getGlobalBounds().width) / 2.f;
    titleText.setPosition(titleX, 150.f);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(26);
    highScoreText.setFillColor(sf::Color(0, 102, 204));
    highScoreText.setStyle(sf::Text::Bold);
    highScoreText.setPosition(200.f, 530.f);
}

MenuScreen::Action MenuScreen::handleClick(sf::Vector2f mousePos) const
{
    if (startButtonSprite.getGlobalBounds().contains(mousePos))    return Action::StartGame;
    if (settingsButtonSprite.getGlobalBounds().contains(mousePos)) return Action::OpenSettings;
    return Action::None;
}

void MenuScreen::render(sf::RenderWindow& window, const sf::Sprite& background, float windowWidth,
                         Difficulty currentDifficulty, const HighScoreManager& highScoreMgr)
{
    window.draw(background);
    window.draw(titleText);
    window.draw(startButtonSprite);
    window.draw(settingsButtonSprite);

    highScoreText.setString("High Score (" + DifficultyConfig::name(currentDifficulty) + "): " +
                             std::to_string(highScoreMgr.get(currentDifficulty)));
    float hsX = (windowWidth - highScoreText.getGlobalBounds().width) / 2.f;
    highScoreText.setPosition(hsX, 220.f);
    window.draw(highScoreText);
}