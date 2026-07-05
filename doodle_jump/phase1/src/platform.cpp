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
    if (hasSpring) {
        window.draw(springSprite);
    }
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
    //spring scrolling
    if (hasSpring) {
        springSprite.move(0.f, offsetY);
    }
}

void Platform::addSpring(sf::Texture& springTexture) 
{
    hasSpring = true;
    springSprite.setTextureRect(sf::IntRect(0, 0, 25, 26)); 
    
    isSpringTriggered = false;
    
    float springX = position.x + (sprite.getGlobalBounds().width / 2.f) - (springSprite.getGlobalBounds().width / 2.f);
    float springY = position.y - springSprite.getGlobalBounds().height + 5.f; 
    
    springSprite.setPosition(springX, springY);
}

bool Platform::checkSpringCollision(sf::FloatRect playerBounds) 
{
    if (!hasSpring) return false;
    return playerBounds.intersects(springSprite.getGlobalBounds());
}

void Platform::triggerSpring()
{
    if (!isSpringTriggered) { 
        isSpringTriggered = true; 
        springSprite.setTextureRect(sf::IntRect(0, 26, 25, 26)); 
    }
}