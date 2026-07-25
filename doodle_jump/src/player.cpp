#include "player.hpp"

Player::Player(sf::Texture& leftTex, sf::Texture& rightTex)
    : position(250.f, 600.f)
    , velocity(0.f, 0.f)
    , score(0)
    , moveSpeed(400.f)
    , leftTexture(&leftTex)
    , rightTexture(&rightTex)
{
    sprite.setTexture(rightTex);
    sprite.setPosition(position);
    sprite.setScale(0.9f, 0.9f);
}

void Player::setShootingTextures(const sf::Texture& shootBodyTex, const sf::Texture& noseTex) 
{
    shootBodyTexture = &shootBodyTex;
    noseSprite.setTexture(noseTex);
}

void Player::setShooting(bool shooting)
{
    if (m_isShooting == shooting) return;
    m_isShooting = shooting;

    if (m_isShooting && shootBodyTexture) {
        sprite.setTexture(*shootBodyTexture);
        sprite.setScale(0.9f, 0.9f);
    } else {
        sprite.setTexture(isFacingLeft ? *leftTexture : *rightTexture);
        sprite.setScale(0.9f, 0.9f);
    }
}

void Player::handleInput()
{
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -moveSpeed;
        isFacingLeft = true;
        if (!m_isShooting && leftTexture) sprite.setTexture(*leftTexture);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = moveSpeed;
        isFacingLeft = false;
        if (!m_isShooting && rightTexture) sprite.setTexture(*rightTexture);    
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

    if (m_isShooting) {
        float bodyCenterX = sprite.getPosition().x + (sprite.getGlobalBounds().width / 2.0f);
        
        float noseX = bodyCenterX - (noseSprite.getGlobalBounds().width);
        float noseY = sprite.getPosition().y; 

        noseSprite.setPosition(noseX, noseY);
    }
}

sf::Vector2f Player::getNoseTipPosition() const 
{
    if (m_isShooting) {
        float noseCenterX = noseSprite.getPosition().x + (noseSprite.getGlobalBounds().width / 2.0f);
        float noseTopY = noseSprite.getPosition().y;
        
        return sf::Vector2f(noseCenterX, noseTopY);
    }
    
    return sf::Vector2f(sprite.getPosition().x + (sprite.getGlobalBounds().width / 2.0f), sprite.getPosition().y);
}

sf::Vector2f Player::getPosition() const { return position; }
void Player::setPosition(sf::Vector2f pos) { position = pos; sprite.setPosition(position); }
sf::Vector2f Player::getVelocity() const { return velocity; }
void Player::setVelocity(sf::Vector2f vel) { velocity = vel; }
sf::FloatRect Player::getBounds() const { return sprite.getGlobalBounds(); }

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
    if (m_isShooting) {
        window.draw(noseSprite);
    }
}

void Player::bounce()
{
    velocity.y = -650.f;
}