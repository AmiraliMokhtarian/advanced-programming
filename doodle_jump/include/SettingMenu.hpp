#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "ResourceManager.hpp" 

enum class Difficulty { EASY, MEDIUM, HARD };

class SettingsMenu {
public:
    SettingsMenu(float windowWidth, float windowHeight, ResourceManager<sf::Texture>& textureMgr, const sf::Font& font);

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    void render(sf::RenderWindow& window);

    float getVolume() const { return volume; }

private:
    sf::Text titleText;
    sf::Text volLabel;
    sf::Text volValueText;
    sf::Text diffLabel;

    sf::RectangleShape sliderTrack;
    sf::CircleShape sliderKnob;
    bool isDragging = false;
    float volume = 50.0f;
    sf::Sprite backSprite;

    void updateSliderUI();
};