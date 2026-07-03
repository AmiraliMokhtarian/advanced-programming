#include "platform.hpp"

Platform::Platform(sf::Texture& texture, sf::Vector2f pos)
    : position(pos)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Platform::render(sf::RenderWindow& window) 
{
    window.draw(sprite);
}

sf::FloatRect Platform::getBounds() const 
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Platform::getPosition() const 
{
    return position;
}

void Platform::setPosition(sf::Vector2f pos)
{
    position = pos;
}

void Platform::scroll(float offsetY) 
{
    position.y += offsetY;      
    sprite.setPosition(position); 
}