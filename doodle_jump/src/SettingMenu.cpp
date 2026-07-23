#include "SettingMenu.hpp"
#include <algorithm>
#include <cmath>

SettingsMenu::SettingsMenu(float windowWidth, float windowHeight, ResourceManager<sf::Texture>& textureMgr, const sf::Font& font) {
    float centerX = windowWidth / 2.0f;

    titleText.setFont(font);
    titleText.setString("SETTINGS");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::Black);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin(tb.width / 2.0f, tb.top + tb.height / 2.0f);
    titleText.setPosition(centerX, 70.0f);

    volLabel.setFont(font);
    volLabel.setString("Volume");
    volLabel.setCharacterSize(26);
    volLabel.setFillColor(sf::Color::Black);
    tb = volLabel.getLocalBounds();
    volLabel.setOrigin(tb.width / 2.0f, tb.top + tb.height / 2.0f);
    volLabel.setPosition(centerX, 150.0f);

    //slider
    sliderTrack.setSize(sf::Vector2f(260.0f, 8.0f));
    sliderTrack.setFillColor(sf::Color(180, 190, 205));
    sliderTrack.setOrigin(130.0f, 4.0f);
    sliderTrack.setPosition(centerX, 210.0f);

    sliderKnob.setRadius(14.0f);
    sliderKnob.setFillColor(sf::Color(20, 50, 90));
    sliderKnob.setOrigin(14.0f, 14.0f);

    volValueText.setFont(font);
    volValueText.setCharacterSize(18);
    volValueText.setFillColor(sf::Color::Black);

    updateSliderUI();

    backSprite.setTexture(textureMgr.load("back_button", "assets/back_button.png"));    sf::FloatRect sb = backSprite.getLocalBounds();
    backSprite.setOrigin(sb.width / 2.0f, sb.height / 2.0f);
    backSprite.setPosition(centerX, windowHeight - 90.0f);
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
    volValueText.setPosition(sliderTrack.getPosition().x, sliderTrack.getPosition().y + 20.0f);
}

bool SettingsMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouse(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (sliderKnob.getGlobalBounds().contains(mouse) || sliderTrack.getGlobalBounds().contains(mouse)) {
            isDragging = true;
        }

        if (backSprite.getGlobalBounds().contains(mouse)) {
            return true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
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
    window.draw(backSprite);
}