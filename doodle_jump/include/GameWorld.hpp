#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "platform.hpp"
#include "Monster.hpp"
#include "Hole.hpp"
#include "Bullet.hpp"
#include "Difficulty.hpp"
#include "player.hpp"
#include "SoundManager.hpp"

enum class CollisionOutcome { None, PlayerDied, HoleTriggered };

class GameWorld {
public:
    explicit GameWorld(ResourceManager<sf::Texture>& textures);
    ~GameWorld();

    void setDifficulty(Difficulty level);

    void generateInitial(sf::Vector2f playerStartPos);
    void reset(sf::Vector2f playerStartPos);

    void updateEntities(float dt);
    void scrollBy(float offsetY, int currentScore);

    void fireBullet(sf::Vector2f noseTip);
    void updateBullets(float dt);

    CollisionOutcome checkCollisions(Player& player, SoundManager& soundManager);

    void render(sf::RenderWindow& window);

private:
    bool isAreaFree(const sf::FloatRect& area) const;
    void spawnPlatform(float yPosition, int currentScore);
    void spawnMonster(float yPosition, int currentScore);
    void spawnHole(float yPosition, int currentScore);
    void clearAll();
    float findFreeX(float yPosition, sf::Vector2f size) const;

    ResourceManager<sf::Texture>& textures;

    std::vector<Platform*> platforms;
    std::vector<Monster*>  monsters;
    std::vector<Hole*>     holes;
    std::vector<Bullet*>   bullets;

    Difficulty currentDifficulty = Difficulty::EASY;
    DifficultySettings settings = DifficultyConfig::getSettings(Difficulty::EASY);
};