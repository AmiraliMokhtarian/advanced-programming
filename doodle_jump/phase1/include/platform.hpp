#pragma once
#include <SFML/Graphics.hpp>

class Platform {
public:
    Platform(sf::Texture& texture, sf::Vector2f pos);
    virtual ~Platform() = default; 

    virtual void update(float dt) = 0; 
    
    virtual void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);

    void scroll(float offsetY);

protected:
    sf::Sprite sprite;
    sf::Vector2f position;
};