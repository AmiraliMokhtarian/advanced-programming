#pragma once
#include "platform.hpp"

class NormalPlatform : public Platform {
public:
    NormalPlatform(sf::Texture& texture, sf::Vector2f pos);
    void update(float dt) override;
};