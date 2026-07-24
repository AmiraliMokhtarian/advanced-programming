#pragma once
#include <SFML/Graphics.hpp>

enum class MonsterType { Green, Blue };

class Monster {
public:
    Monster(sf::Vector2f position, const sf::Texture& texture, MonsterType type, int health = 1);
    virtual ~Monster() = default;

    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);

    bool isAlive() const;
    void takeDamage(int amount = 1);
    int getHealth() const;

private:
    sf::Sprite sprite;
    MonsterType type;
    int health;
    bool alive{true};

    int frameWidth{0};
    int frameHeight{0};
    float moveSpeed{120.f};
    int moveDirection{-1};
    float leftBound{25.f};
    float rightBound{575.f};
};