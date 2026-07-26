#pragma once
#include "platform.hpp"

class MovingPlatform : public Platform {
public:
    MovingPlatform(sf::Texture& texture, sf::Vector2f pos, float speed);
    void update(float dt) override;

private:
    float speed;
    int direction; // 1 -> right , -1 -> left
};