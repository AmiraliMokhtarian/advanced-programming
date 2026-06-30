#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(sf::Texture& texture);

    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    void update(float dt);

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    int score;
};