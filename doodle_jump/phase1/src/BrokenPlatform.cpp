#include "BrokenPlatform.hpp"

BrokenPlatform::BrokenPlatform(sf::Texture& texture, sf::Vector2f pos)
    : Platform(texture, pos)
    , isBroken(false)
    , fallSpeed(400.f)
{}

void BrokenPlatform::update(float dt) 
{
    if (isBroken) {
        position.y += fallSpeed * dt;
        sprite.setPosition(position);
    }
}

void BrokenPlatform::breakPlatform() 
{
    isBroken = true;
}

bool BrokenPlatform::isBrokenPlatform() const 
{
    return isBroken;
}