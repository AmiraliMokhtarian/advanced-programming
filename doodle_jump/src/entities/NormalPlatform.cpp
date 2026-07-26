#include "NormalPlatform.hpp"

NormalPlatform::NormalPlatform(sf::Texture& texture, sf::Vector2f pos)
    : Platform(texture, pos)
{}

void NormalPlatform::update(float dt) 
{
    //normal platforms don't move at all
}