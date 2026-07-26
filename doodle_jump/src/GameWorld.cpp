#include "GameWorld.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BrokenPlatform.hpp"
#include "Constants.hpp"
#include <cstdlib>
#include <cmath>

GameWorld::GameWorld(ResourceManager<sf::Texture>& tex) : textures(tex)
{
    textures.load("platform_normal", "assets/normal_platform.png");
    textures.load("platform_moving", "assets/moving_platform.png");
    textures.load("platform_broken", "assets/broken_platform.png");
    textures.load("spring", "assets/spring_sprite.png");

    textures.load("monster_green", "assets/green_monster.png");
    textures.load("monster_blue", "assets/BlueMonster.png");

    textures.load("hole_small", "assets/hole.png");
    textures.load("hole_large", "assets/hole@2x.png");
}

GameWorld::~GameWorld() { clearAll(); }

void GameWorld::clearAll()
{
    for (auto* p : platforms) delete p;
    platforms.clear();
    for (auto* m : monsters) delete m;
    monsters.clear();
    for (auto* h : holes) delete h;
    holes.clear();
    for (auto* b : bullets) delete b;
    bullets.clear();
}

void GameWorld::setDifficulty(Difficulty level)
{
    currentDifficulty = level;
    settings = DifficultyConfig::getSettings(level);
}

void GameWorld::generateInitial(sf::Vector2f playerStartPos)
{
    float firstPlatformX = playerStartPos.x - 20.f;
    float firstPlatformY = playerStartPos.y + 80.f;

    platforms.push_back(new NormalPlatform(textures.get("platform_normal"), sf::Vector2f(firstPlatformX, firstPlatformY)));

    float currentY = firstPlatformY - (85.f + std::rand() % 40);
    while (currentY > 0.f) {
        spawnPlatform(currentY, 0); 
        float gapY = 85.f + static_cast<float>(std::rand() % 40);
        currentY -= gapY;
    }
}

void GameWorld::reset(sf::Vector2f playerStartPos)
{
    clearAll();
    generateInitial(playerStartPos);
}

void GameWorld::updateEntities(float dt)
{
    for (auto* platform : platforms) platform->update(dt);
    for (auto* monster : monsters)  monster->update(dt);
}

void GameWorld::fireBullet(sf::Vector2f noseTip)
{
    bullets.push_back(new Bullet(noseTip.x, noseTip.y));
}

void GameWorld::updateBullets(float dt)
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        (*it)->update(dt);
        if ((*it)->isOutOfBounds()) {
            delete *it;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    for (auto bIt = bullets.begin(); bIt != bullets.end(); ) {
        bool bulletDestroyed = false;

        for (auto mIt = monsters.begin(); mIt != monsters.end(); ) {
            if ((*bIt)->getBounds().intersects((*mIt)->getBounds())) {
                (*mIt)->takeDamage(1);

                if (!(*mIt)->isAlive()) {
                    delete *mIt;
                    mIt = monsters.erase(mIt);
                } else {
                    ++mIt;
                }

                delete *bIt;
                bIt = bullets.erase(bIt);
                bulletDestroyed = true;
                break;
            } else {
                ++mIt;
            }
        }

        if (!bulletDestroyed) ++bIt;
    }
}

CollisionOutcome GameWorld::checkCollisions(Player& player, SoundManager& soundManager)
{
    for (auto* hole : holes) {
        if (player.getBounds().intersects(hole->getBounds())) {
            player.startAbsorption(hole->getCenter());
            return CollisionOutcome::HoleTriggered;
        }
    }

    if (player.getVelocity().y > 0.f) {
        for (auto* platform : platforms) {
            if (platform->checkSpringCollision(player.getBounds())) {
                player.setVelocity(sf::Vector2f(player.getVelocity().x, -900.f));
                platform->triggerSpring();
                soundManager.playJump();
                return CollisionOutcome::None;
            }

            if (player.getBounds().intersects(platform->getBounds())) {
                if (player.getBounds().top + player.getBounds().height <= platform->getBounds().top + 20.f) {
                    BrokenPlatform* broken = dynamic_cast<BrokenPlatform*>(platform);
                    if (broken) {
                        broken->breakPlatform();
                        soundManager.playJump();
                        player.bounce();
                    } else {
                        soundManager.playJump();
                        player.bounce();
                    }
                    break;
                }
            }
        }
    }

    for (auto* monster : monsters) {
        if (!monster->isAlive()) continue;

        sf::FloatRect monsterBounds = monster->getBounds();
        sf::FloatRect playerBounds = player.getBounds();

        if (playerBounds.intersects(monsterBounds)) {
            if (player.getVelocity().y > 0.f &&
                (playerBounds.top + playerBounds.height <= monsterBounds.top + 25.f)) {
                monster->takeDamage(1);
                player.setVelocity(sf::Vector2f(player.getVelocity().x, -800.f));
                soundManager.playJump();
            } else {
                return CollisionOutcome::PlayerDied;
            }
        }
    }

    return CollisionOutcome::None;
}

void GameWorld::scrollBy(float offsetY, int currentScore)
{
    for (auto it = platforms.begin(); it != platforms.end(); ) {
        Platform* platform = *it;
        platform->scroll(offsetY);

        if (platform->getPosition().y > float(Config::Window::HEIGHT)) {
            delete platform;
            it = platforms.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = monsters.begin(); it != monsters.end(); ) {
        (*it)->setPosition(sf::Vector2f((*it)->getPosition().x, (*it)->getPosition().y + offsetY));

        if ((*it)->getPosition().y > float(Config::Window::HEIGHT) + 50.f) {
            delete *it;
            it = monsters.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = holes.begin(); it != holes.end(); ) {
        (*it)->scroll(offsetY);
        if ((*it)->getPosition().y > float(Config::Window::HEIGHT) + 50.f) {
            delete *it;
            it = holes.erase(it);
        } else {
            ++it;
        }
    }

    while (platforms.size() < 12) {
        float highestY = float(Config::Window::HEIGHT);
        for (auto* p : platforms) {
            if (p->getPosition().y < highestY) {
                highestY = p->getPosition().y;
            }
        }

        float newSpawnY = highestY - (70.f + std::rand() % 60);
        spawnPlatform(newSpawnY, currentScore);
    }
}

bool GameWorld::isAreaFree(const sf::FloatRect& area) const
{
    for (auto* platform : platforms)
        if (platform->getBounds().intersects(area)) return false;

    for (auto* monster : monsters)
        if (monster->getBounds().intersects(area)) return false;

    for (auto* hole : holes)
        if (hole->getBounds().intersects(area)) return false;

    return true;
}

void GameWorld::spawnPlatform(float yPosition, int currentScore)
{
    const sf::Vector2f platformSize(60.f, 20.f);
    float xPosition = findFreeX(yPosition, platformSize);

    int randType = std::rand() % 100;

    if (randType < 70) {
        NormalPlatform* np = new NormalPlatform(textures.get("platform_normal"), sf::Vector2f(xPosition, yPosition));
        if (std::rand() % 100 < 10) {
            np->addSpring(textures.get("spring"));
        }
        platforms.push_back(np);
    } else if (randType < 90) {
        platforms.push_back(new MovingPlatform(textures.get("platform_moving"), sf::Vector2f(xPosition, yPosition), settings.movingPlatformSpeed));
    } else {
        platforms.push_back(new BrokenPlatform(textures.get("platform_broken"), sf::Vector2f(xPosition, yPosition)));
    }

    spawnMonster(yPosition, currentScore);

    if (currentDifficulty == Difficulty::HARD)
        spawnHole(yPosition, currentScore);
}

void GameWorld::spawnMonster(float yPosition, int currentScore)
{
    if (currentScore < 100) return;
    if (std::rand() % 100 >= 15) return;

    const sf::Vector2f monsterSize(50.f, 50.f);

    for (int attempt = 0; attempt < 15; ++attempt) {
        float xPos = static_cast<float>(std::rand() % (Config::Window::WIDTH - 160) + 80);
        float monsterY = yPosition - 60.f;
        sf::FloatRect candidate(xPos, monsterY, monsterSize.x, monsterSize.y);

        if (isAreaFree(candidate)) {
            MonsterType type = (std::rand() % 2 == 0) ? MonsterType::Green : MonsterType::Blue;

            const sf::Texture& tex = (type == MonsterType::Green) ?
                                     textures.get("monster_green") :
                                     textures.get("monster_blue");

            monsters.push_back(new Monster(sf::Vector2f(xPos, monsterY), tex, type, settings.monsterHealth));
            return;
        }
    }
}

void GameWorld::spawnHole(float yPosition, int currentScore)
{
    if (currentScore < 100) return;
    if (std::rand() % 100 >= 10) return;

    HoleSize size = (std::rand() % 2 == 0) ? HoleSize::Small : HoleSize::Large;

    const sf::Texture& holeTex = (size == HoleSize::Small) ?
                                 textures.get("hole_small") :
                                 textures.get("hole_large");

    sf::Vector2f holeDims(static_cast<float>(holeTex.getSize().x),
                          static_cast<float>(holeTex.getSize().y));

    auto isSafePath = [&](const sf::FloatRect& candidate) -> bool {
        if (!isAreaFree(candidate)) return false;

        const float SAFE_MARGIN_X = 60.f;
        const float SAFE_MARGIN_Y = 60.f;

        for (const auto* p : platforms) {
            sf::FloatRect pBounds = p->getBounds();
            if (std::abs(candidate.top - pBounds.top) < SAFE_MARGIN_Y) {
                if (std::abs(candidate.left - pBounds.left) < SAFE_MARGIN_X) {
                    return false;
                }
            }
        }
        return true;
    };

    for (int attempt = 0; attempt < 15; ++attempt) {
        float xPos = static_cast<float>(std::rand() % (Config::Window::WIDTH - static_cast<int>(holeDims.x) - 40) + 20);
        float holeY = yPosition - (holeDims.y / 2.f);

        sf::FloatRect candidate(xPos, holeY, holeDims.x, holeDims.y);

        if (isSafePath(candidate)) {
            holes.push_back(new Hole(sf::Vector2f(xPos, holeY), holeTex, size));
            return;
        }
    }
}

void GameWorld::render(sf::RenderWindow& window)
{
    for (auto* platform : platforms) platform->render(window);
    for (auto* hole : holes)         hole->render(window);
    for (auto* bullet : bullets)     bullet->render(window);
    for (auto* monster : monsters)   monster->render(window);
}

float GameWorld::findFreeX(float yPosition, sf::Vector2f size) const
{
    for (int attempt = 0; attempt < 20; ++attempt) {
        float x = static_cast<float>(std::rand() % (Config::Window::WIDTH - static_cast<int>(size.x)));
        sf::FloatRect candidate(x, yPosition, size.x, size.y);
        if (isAreaFree(candidate)) return x;
    }

    // if randomness doesn't work:
    for (int x = 0; x <= Config::Window::WIDTH - static_cast<int>(size.x); x += 10) {
        sf::FloatRect candidate(static_cast<float>(x), yPosition, size.x, size.y);
        if (isAreaFree(candidate)) return static_cast<float>(x);
    }

    //it shouldn't happen -> just for safety
    return 0.f;
}