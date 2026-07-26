#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(sf::Texture& leftTex, sf::Texture& rightTex);

    void setShootingTextures(const sf::Texture& shootBodyTex, const sf::Texture& noseTex);

    void handleInput();
    void update(float dt, float windowWidth);
    void render(sf::RenderWindow& window);

    // Position and Bounds
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getVelocity() const;
    void setVelocity(sf::Vector2f vel);
    sf::FloatRect getBounds() const;

    void setShooting(bool shooting);
    bool isShooting() const { return m_isShooting; }
    sf::Vector2f getNoseTipPosition() const;

    // Game Mechanics
    void bounce();
    void addScore(float value) { score += value; }
    int getScore() const { return static_cast<int>(score); }
    void resetScore() { score = 0; }

    void startAbsorption(sf::Vector2f holeCenter);
    void updateAbsorption(float dt);
    void cancelAbsorption();
    bool isBeingAbsorbed() const { return absorbing; }
    bool isAbsorptionComplete() const { return absorbing && absorbProgress >= 1.f; }

private:
    sf::Sprite sprite;
    sf::Sprite noseSprite;

    sf::Vector2f position;
    sf::Vector2f velocity;
    float score;
    float moveSpeed;

    const sf::Texture* leftTexture = nullptr;
    const sf::Texture* rightTexture = nullptr;
    const sf::Texture* shootBodyTexture = nullptr;

    bool isFacingLeft = false;
    bool m_isShooting = false;

    bool absorbing = false;
    float absorbProgress = 0.f;
    const float absorbDuration = 0.8f; //pause time
    sf::Vector2f absorbStartPos;
    sf::Vector2f absorbTargetPos;
    const float baseScale = 0.9f;
};