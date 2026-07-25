#include "SettingMenu.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

SettingsMenu::SettingsMenu(float windowWidth, float windowHeight, ResourceManager<sf::Texture>& textureMgr, 
                           const sf::Font& font, float initialVolume, Difficulty initialDifficulty) 
{
    volume = initialVolume;
    currentDifficulty = initialDifficulty;

    float centerX = windowWidth / 2.0f;

    titleText.setFont(font);
    titleText.setString("SETTINGS");
    titleText.setCharacterSize(45);
    titleText.setFillColor(sf::Color(10, 40, 80));
    titleText.setStyle(sf::Text::Bold);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
    titleText.setPosition(centerX, 60.0f);

    volLabel.setFont(font);
    volLabel.setString("Volume");
    volLabel.setCharacterSize(22);
    volLabel.setFillColor(sf::Color(10, 40, 80));
    volLabel.setStyle(sf::Text::Bold);
    sf::FloatRect tbV = volLabel.getLocalBounds();
    volLabel.setOrigin(tbV.left + tbV.width / 2.0f, tbV.top + tbV.height / 2.0f);
    volLabel.setPosition(centerX, 140.0f);

    //SLIDER UI
    sliderTrack.setSize(sf::Vector2f(260.0f, 6.0f));
    sliderTrack.setFillColor(sf::Color(190, 200, 210));
    sliderTrack.setOrigin(130.0f, 3.0f);
    sliderTrack.setPosition(centerX, 180.0f);

    sliderKnob.setRadius(12.0f);
    sliderKnob.setFillColor(sf::Color(10, 40, 80));
    sliderKnob.setOrigin(12.0f, 12.0f);

    volValueText.setFont(font);
    volValueText.setCharacterSize(16);
    volValueText.setFillColor(sf::Color::Black);
    volValueText.setStyle(sf::Text::Bold);

    updateSliderUI();

    diffLabel.setFont(font);
    diffLabel.setString("Difficulty");
    diffLabel.setCharacterSize(22);
    diffLabel.setFillColor(sf::Color(10, 40, 80));
    diffLabel.setStyle(sf::Text::Bold);
    sf::FloatRect tbD = diffLabel.getLocalBounds();
    diffLabel.setOrigin(tbD.left + tbD.width / 2.0f, tbD.top + tbD.height / 2.0f);
    diffLabel.setPosition(centerX, 260.0f);

    //DIFFICULTY BUTTONS
    std::vector<std::pair<std::string, Difficulty>> levels = {
        {"EASY", Difficulty::EASY},
        {"MEDIUM", Difficulty::MEDIUM},
        {"HARD", Difficulty::HARD}
    };

    float btnWidth = 100.f;
    float btnHeight = 45.f;
    float spacing = 12.f;
    float totalWidth = (3 * btnWidth) + (2 * spacing);
    float startX = centerX - (totalWidth / 2.f) + (btnWidth / 2.f);

    for (size_t i = 0; i < levels.size(); ++i) {
        DifficultyButton btn;
        btn.level = levels[i].second;

        btn.box.setSize(sf::Vector2f(btnWidth, btnHeight));
        btn.box.setOrigin(btnWidth / 2.f, btnHeight / 2.f);
        btn.box.setPosition(startX + i * (btnWidth + spacing), 320.0f);
        btn.box.setOutlineThickness(2.f);
        btn.box.setOutlineColor(sf::Color(10, 40, 80));

        btn.text.setFont(font);
        btn.text.setString(levels[i].first);
        btn.text.setCharacterSize(16);
        btn.text.setStyle(sf::Text::Bold);

        sf::FloatRect textBounds = btn.text.getLocalBounds();
        btn.text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        btn.text.setPosition(btn.box.getPosition());

        diffButtons.push_back(btn);
    }

    updateDifficultyUI();

    // BACK BUTTON
    backSprite.setTexture(textureMgr.load("back_button", "assets/back_button.png"));
    sf::FloatRect sb = backSprite.getLocalBounds();
    backSprite.setOrigin(sb.width / 2.0f, sb.height / 2.0f);
    backSprite.setPosition(centerX, windowHeight - 70.0f);
    backSprite.setScale(0.5f, 0.5f);
}

void SettingsMenu::updateSliderUI() {
    float trackX = sliderTrack.getPosition().x - 130.0f;
    float trackWidth = sliderTrack.getSize().x;

    float knobX = trackX + (volume / 100.0f) * trackWidth;
    sliderKnob.setPosition(knobX, sliderTrack.getPosition().y);

    volValueText.setString(std::to_string(static_cast<int>(std::round(volume))) + "%");
    sf::FloatRect tb = volValueText.getLocalBounds();
    volValueText.setOrigin(tb.left + tb.width / 2.0f, tb.top);
    volValueText.setPosition(sliderTrack.getPosition().x, sliderTrack.getPosition().y + 18.0f);
}

void SettingsMenu::updateDifficultyUI() {
    for (auto& btn : diffButtons) {
        if (btn.level == currentDifficulty) {
            btn.box.setFillColor(sf::Color(10, 40, 80));
            btn.text.setFillColor(sf::Color::White);
        } else {
            btn.box.setFillColor(sf::Color(225, 232, 240));
            btn.text.setFillColor(sf::Color(10, 40, 80));
        }
    }
}

bool SettingsMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouse(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (sliderKnob.getGlobalBounds().contains(mouse) || sliderTrack.getGlobalBounds().contains(mouse)) {
            isDragging = true;
        }

        for (auto& btn : diffButtons) {
            if (btn.box.getGlobalBounds().contains(mouse)) {
                currentDifficulty = btn.level;
                updateDifficultyUI();
                break;
            }
        }

        if (backSprite.getGlobalBounds().contains(mouse)) {
            return true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (isDragging) {
            isDragging = false;
        }
    }

    if (isDragging && event.type == sf::Event::MouseMoved) {
        float trackX = sliderTrack.getPosition().x - 130.0f;
        float trackWidth = sliderTrack.getSize().x;
        float clampedX = std::clamp(mouse.x, trackX, trackX + trackWidth);

        volume = ((clampedX - trackX) / trackWidth) * 100.0f;
        updateSliderUI();
    }

    return false;
}

void SettingsMenu::render(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(volLabel);
    window.draw(sliderTrack);
    window.draw(sliderKnob);
    window.draw(volValueText);

    window.draw(diffLabel);
    for (const auto& btn : diffButtons) {
        window.draw(btn.box);
        window.draw(btn.text);
    }

    window.draw(backSprite);
}