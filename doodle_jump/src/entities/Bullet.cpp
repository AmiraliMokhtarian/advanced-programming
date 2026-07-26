#include "Bullet.hpp"

Bullet::Bullet(float startX, float startY) 
{
    float radius = 6.0f;
    shape.setRadius(radius);
    shape.setFillColor(sf::Color(255, 215, 0));
    shape.setOutlineThickness(1.5f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOrigin(radius, radius);
    shape.setPosition(startX, startY);
}

void Bullet::update(float dt) 
{
    shape.move(0.0f, -speed * dt);
}

void Bullet::render(sf::RenderWindow& window) 
{
    window.draw(shape);
}