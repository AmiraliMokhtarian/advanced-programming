#include "MovingPlatform.hpp"

MovingPlatform::MovingPlatform(sf::Texture& texture, sf::Vector2f pos)
    : Platform(texture, pos)
    , speed(150.f) 
    , direction(1)
{}

void MovingPlatform
::update(float dt) 
{
    position.x += direction * speed * dt;
    
    //postion.x of platform is always left upper corner of it
    float platformWidth = sprite.getGlobalBounds().width;
    if (position.x <= 0.f) {
        position.x = 0.f;
        direction = 1; // change direction to right
    } 
    else if (position.x + platformWidth >= 600.f) {
        position.x = 600.f - platformWidth;
        direction = -1; // change direction to left
    }

    sprite.setPosition(position);
}