#pragma once
#include "platform.hpp"

class BrokenPlatform : public Platform {
public:
    BrokenPlatform(sf::Texture& texture, sf::Vector2f pos);

    void update(float dt) override;
    void breakPlatform();
    bool isBrokenPlatform() const;

private:
    bool isBroken;
    float fallSpeed;
};