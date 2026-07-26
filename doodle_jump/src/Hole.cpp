#include "Hole.hpp"

Hole::Hole(sf::Vector2f pos, const sf::Texture& texture, HoleSize s)
    : position(pos), size(s)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Hole::render(sf::RenderWindow& window) 
{ 
    window.draw(sprite); 
}

void Hole::scroll(float offsetY) 
{
    position.y += offsetY;
    sprite.setPosition(position);
}

sf::FloatRect Hole::getBounds() const 
{ 
    return sprite.getGlobalBounds(); 
}

sf::Vector2f Hole::getPosition() const 
{ 
    return position; 
}

sf::Vector2f Hole::getCenter() const 
{
    sf::FloatRect b = sprite.getGlobalBounds();
    return sf::Vector2f(b.left + b.width / 2.f, b.top + b.height / 2.f);
}