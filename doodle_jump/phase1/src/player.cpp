#include "player.hpp"

Player::Player(sf::Texture& leftTex, sf::Texture& rightTex)
    : position(250.f, 600.f) //near bottom of a 400*600 window
    , velocity(0.f, 0.f)
    , score(0)
    , moveSpeed(400.f)
    , leftTexture(leftTex)
    , rightTexture(rightTex)
{
    sprite.setTexture(rightTex);
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

void Player::handleInput()
{
    //reset x velocity in each frame
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -moveSpeed;
        sprite.setTexture(leftTexture); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = moveSpeed;
        sprite.setTexture(rightTexture);
    }
}

void Player::update(float dt, float windowWidth)
{
    float gravity = 800.f;

    velocity.y += gravity * dt;
    position += velocity * dt;

    if (position.x < 0.f) {
        position.x = windowWidth;
    }
    else if (position.x > windowWidth) {
        position.x = 0.f;
    }
    
    sprite.setPosition(position);
}
