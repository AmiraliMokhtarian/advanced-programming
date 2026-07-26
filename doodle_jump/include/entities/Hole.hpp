#pragma once
#include <SFML/Graphics.hpp>

enum class HoleSize { Small, Large };

class Hole {
public:
    Hole(sf::Vector2f position, const sf::Texture& texture, HoleSize size);

    void render(sf::RenderWindow& window);
    void scroll(float offsetY);

    sf::FloatRect getBounds() const;
    sf::Vector2f getCenter() const;
    sf::Vector2f getPosition() const;

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    HoleSize size;
};