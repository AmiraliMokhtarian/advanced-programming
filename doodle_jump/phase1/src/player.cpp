#include "player.hpp"

Player::Player(sf::Texture& texture)
    : position(250.f, 600.f) //near bottom of a 400x600 window
    , velocity(0.f, 0.f)
    , score(0)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Player::render(sf::RenderWindow& window) 
{
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const 
{
    return position;
}

void Player::setPosition(sf::Vector2f pos) 
{
    position = pos;
    sprite.setPosition(position);
}

void Player::update(float dt)
{
    float gravity = 900.f;

    velocity.y += gravity * dt;

    position += velocity * dt;

    sprite.setPosition(position);
}
