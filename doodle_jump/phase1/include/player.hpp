#pragma once
#include <SFML/Graphics.hpp>

class Player{
public:
    Player(sf::Texture& leftTex, sf::Texture& rightTex);

    void render(sf::RenderWindow& window);
    void handleInput();
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    void update(float dt, float windowWidth);

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    int score;
    float moveSpeed;

    sf::Texture& leftTexture;
    sf::Texture& rightTexture;
};