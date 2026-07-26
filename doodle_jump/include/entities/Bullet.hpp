#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(float startX, float startY);

    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool isOutOfBounds() const { return shape.getPosition().y < -20.0f; }

private:
    sf::CircleShape shape;
    float speed = 900.0f;
};