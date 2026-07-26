#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "ResourceManager.hpp"
#include "Difficulty.hpp"

class SettingsMenu {
public:
    SettingsMenu(float windowWidth, float windowHeight, ResourceManager<sf::Texture>& textureMgr,
                const sf::Font& font, float initialVolume, Difficulty initialDifficulty);

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    float getVolume() const { return volume; }
    Difficulty getDifficulty() const { return currentDifficulty; }

private:
    sf::Text titleText;
    sf::Text volLabel;
    sf::Text volValueText;
    sf::Text diffLabel;

    // Slider UI
    sf::RectangleShape sliderTrack;
    sf::CircleShape sliderKnob;
    bool isDragging = false;
    float volume = 50.0f;
    sf::Sprite backSprite;

    // Difficulty UI
    Difficulty currentDifficulty = Difficulty::EASY;
    
    struct DifficultyButton {
        sf::RectangleShape box;
        sf::Text text;
        Difficulty level;
    };
    
    std::vector<DifficultyButton> diffButtons;

    void updateSliderUI();
    void updateDifficultyUI();
};