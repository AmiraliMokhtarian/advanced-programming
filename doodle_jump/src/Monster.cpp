#include "Monster.hpp"

Monster::Monster(sf::Vector2f position, const sf::Texture& texture, MonsterType type, int health)
    : type(type), health(health) 
{
    sprite.setTexture(texture);

    if (type == MonsterType::Blue) {
        frameWidth = texture.getSize().x / 2;
        frameHeight = texture.getSize().y;
        sprite.setTextureRect(sf::IntRect(frameWidth, 0, frameWidth, frameHeight));
        moveSpeed = 140.f;
    } else {
        frameWidth = texture.getSize().x;
        frameHeight = texture.getSize().y;
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        moveSpeed = 120.f;
    }

    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(position);
}

void Monster::update(float dt) {
    if (!alive) return;

    sf::Vector2f pos = sprite.getPosition();
    pos.x += moveSpeed * moveDirection * dt;

    if (pos.x >= rightBound) {
        pos.x = rightBound;
        moveDirection = -1;

        if (type == MonsterType::Blue) {
            sprite.setTextureRect(sf::IntRect(frameWidth, 0, frameWidth, frameHeight));
        } else {
            sprite.setScale(1.f, 1.f); 
        }
    } 
    else if (pos.x <= leftBound) {
        pos.x = leftBound;
        moveDirection = 1;

        if (type == MonsterType::Blue) {
            sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        } else {
            sprite.setScale(-1.f, 1.f); 
        }
    }

    sprite.setPosition(pos);
}

void Monster::render(sf::RenderWindow& window) {
    if (alive) {
        window.draw(sprite);
    }
}

sf::FloatRect Monster::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Monster::getPosition() const {
    return sprite.getPosition();
}

void Monster::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

bool Monster::isAlive() const {
    return alive && health > 0;
}

void Monster::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        alive = false;
    }
}

int Monster::getHealth() const {
    return health;
}