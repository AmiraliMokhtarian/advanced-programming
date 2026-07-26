#include "Game.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BrokenPlatform.hpp"
#include "Constants.hpp"
#include "Monster.hpp"
#include <cstdlib> // srand
#include <ctime>   // time
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cmath>

Game::Game()
    : window(sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT), "Doodle Jump")
    , currentState(GameState::Menu)
    , font(loadFont())
    , soundBuffers()
    , settingsManager()
    , highScoreMgr()
    , soundManager(soundBuffers, settingsManager.getVolume())
    , player(textures.load("player_left", "assets/left_doodle.png"), 
             textures.load("player_right", "assets/right_doodle.png"))
    , settingsMenu(static_cast<float>(Config::Window::WIDTH), static_cast<float>(Config::Window::HEIGHT), 
                   textures, font, settingsManager.getVolume(), settingsManager.getDifficulty())
{
    window.setFramerateLimit(Config::Window::FPS_LIMIT);
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    initUI();        
    loadTextures();
    generateInitialPlatforms();

    setDifficulty(settingsManager.getDifficulty());
}

Game::~Game() 
{
    for (auto* platform : platforms) {
        delete platform; 
    }
    platforms.clear();
    clearMonsters();
    clearHoles();
}

void Game::clearMonsters() 
{
    for (auto* monster : monsters) {
        delete monster;
    }
    monsters.clear();
}

void Game::clearHoles() 
{
    for (auto* hole : holes) {
        delete hole;
    }
    holes.clear();
}

void Game::resetGame() 
{
    for (auto* platform : platforms) {
        delete platform;
    }
    platforms.clear();
    clearMonsters();
    clearHoles();
    player.cancelAbsorption();

    player.setPosition(sf::Vector2f(300.f, 600.f));
    player.setVelocity(sf::Vector2f(0.f, 0.f));
    player.resetScore();

    generateInitialPlatforms();
}

sf::Font Game::loadFont() 
{
    sf::Font f;
    if (!f.loadFromFile("fonts/ariblk.ttf")) {
        cerr << "Failed to load font!" << std::endl;
    }
    return f;
}

void Game::loadTextures()
{                                      
    textures.load("background", "assets/background.png");
    textures.load("platform_normal", "assets/normal_platform.png");
    textures.load("platform_moving", "assets/moving_platform.png");
    textures.load("platform_broken", "assets/broken_platform.png");
    textures.load("spring", "assets/spring_sprite.png");
    textures.load("start_button", "assets/start_button.png");
    textures.load("restart", "assets/restart_button.png");
    textures.load("menu", "assets/menu_button.png");
    
    textures.load("settings_button", "assets/Settings_button.png");

    backgroundSprite.setTexture(textures.get("background"));

    float centerX = Config::Window::WIDTH / 2.0f;

    // Start Button
    startButtonSprite.setTexture(textures.get("start_button"));
    sf::FloatRect sb = startButtonSprite.getLocalBounds();
    startButtonSprite.setOrigin(sb.width / 2.0f, sb.height / 2.0f);
    startButtonSprite.setPosition(centerX, 360.f);

    // Settings Button
    settingsButtonSprite.setTexture(textures.get("settings_button"));
    settingsButtonSprite.setScale(0.5f, 0.5f); 
    sf::FloatRect stb = settingsButtonSprite.getLocalBounds();
    settingsButtonSprite.setOrigin(stb.width / 2.0f, stb.height / 2.0f);
    settingsButtonSprite.setPosition(centerX, 450.f);

    // Restart Button
    restartButtonSprite.setTexture(textures.get("restart"));
    sf::FloatRect rb = restartButtonSprite.getLocalBounds();
    restartButtonSprite.setOrigin(rb.width / 2.0f, rb.height / 2.0f);
    restartButtonSprite.setPosition(centerX, 460.f);

    // Menu Button
    menuButtonSprite.setTexture(textures.get("menu"));
    sf::FloatRect mb = menuButtonSprite.getLocalBounds();
    menuButtonSprite.setOrigin(mb.width / 2.0f, mb.height / 2.0f);
    menuButtonSprite.setPosition(centerX, 550.f);

    textures.load("monster_green", "assets/green_monster.png");
    textures.load("monster_blue", "assets/BlueMonster.png");

    textures.load("shoot_body", "assets/Shooting@Pose.png");
    textures.load("nose", "assets/Nose.png");
    player.setShootingTextures(textures.get("shoot_body"), textures.get("nose"));

    textures.load("hole_small", "assets/hole.png");
    textures.load("hole_large", "assets/hole@2x.png");
}

void Game::run() 
{
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() 
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (currentState == GameState::Settings) {
            bool backPressed = settingsMenu.handleEvent(event, window);
    
            updateSettingsFromMenu(); 

            if (backPressed)
                currentState = GameState::Menu;
        }
    }
}

void Game::update(float dt) 
{
    bool wantsMenuMusic = (currentState == GameState::Menu || currentState == GameState::Settings);
    bool wasMenuMusic   = (previousState == GameState::Menu || previousState == GameState::Settings);

    if (wantsMenuMusic && !wasMenuMusic) {
        soundManager.playMusic("sounds/MainMenu_Song.flac"); 
    } else if (!wantsMenuMusic && wasMenuMusic) {
        soundManager.stopMusic(); // just once for exit
    }
    previousState = currentState;

    switch (currentState) {
        case GameState::Menu    : updateMenu(dt)    ; break;
        case GameState::Settings: updateSettings(dt); break; 
        case GameState::Gameplay: updateGameplay(dt); break;
        case GameState::GameOver: updateGameOver(dt); break;
    }
}

void Game::render() 
{
    window.clear(sf::Color(240, 240, 250));
    switch (currentState) {
        case GameState::Menu    : renderMenu()    ; break;
        case GameState::Settings: renderSettings(); break; 
        case GameState::Gameplay: renderGameplay(); break;
        case GameState::GameOver: renderGameOver(); break;
    }
    window.display();
}

void Game::updateMenu(float dt) 
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (startButtonSprite.getGlobalBounds().contains(mousePosF)) 
        {
            resetGame();
            currentState = GameState::Gameplay;
        }
        else if (settingsButtonSprite.getGlobalBounds().contains(mousePosF))
        {
            currentState = GameState::Settings;
        }
    }
}

void Game::updateSettings(float dt) {}

void Game::updateGameplay(float dt) 
{

    if (player.isBeingAbsorbed()) {
        player.updateAbsorption(dt);

        if (player.isAbsorptionComplete()) {
            soundManager.playLose();
            highScoreMgr.reportScore(currentDifficulty, player.getScore());
            currentState = GameState::GameOver;
        }
        return; //exit -> to stop the world
    }

    player.handleInput();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player.setShooting(true);

        float fireInterval = 0.2f; 

        if (fireClock.getElapsedTime().asSeconds() >= fireInterval) {
            sf::Vector2f noseTip = player.getNoseTipPosition();
            bullets.push_back(new Bullet(noseTip.x, noseTip.y));

            soundManager.playShoot();
            fireClock.restart();
        }
    } else {
        player.setShooting(false);
    }

    player.update(dt, window.getSize().x);

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

        if (!bulletDestroyed) {
            ++bIt;
        }
    }

    for (auto* platform : platforms) {
        platform->update(dt); 
    }

    for (auto* monster : monsters) {
        monster->update(dt);
    } 

    handleCollisions();
    if (player.isBeingAbsorbed()) return; //don't scroll if there's an absorption
    handleScrolling(dt);

    scoreText.setString("Score: " + std::to_string(player.getScore()));

    if (player.getPosition().y > float(Config::Window::HEIGHT)) {
        if (currentState != GameState::GameOver) {
            soundManager.playLose();
        }
        
        highScoreMgr.reportScore(currentDifficulty, player.getScore());
        
        currentState = GameState::GameOver;
    }
}

void Game::handleCollisions()
{
    for (auto* hole : holes) {
        if (player.getBounds().intersects(hole->getBounds())) {
            player.startAbsorption(hole->getCenter());
            return; //ignore other collisions
        }
    }

    if (player.getVelocity().y > 0.f) 
    {
        for (auto* platform : platforms) 
        {
            if (platform->checkSpringCollision(player.getBounds())) 
            {
                player.setVelocity(sf::Vector2f(player.getVelocity().x, -900.f)); 
                platform->triggerSpring();
                soundManager.playJump();
                return;
            }

            if (player.getBounds().intersects(platform->getBounds())){
                if (player.getBounds().top + player.getBounds().height <= platform->getBounds().top + 20.f){
                    BrokenPlatform* broken = dynamic_cast<BrokenPlatform*>(platform);
                    if (broken) {
                        broken->breakPlatform();
                        soundManager.playJump();
                        player.bounce();
                    } 
                    else {
                        soundManager.playJump();
                        player.bounce(); 
                    }
                    break; 
                }
            }
        }
    }

    for (auto* monster : monsters) 
    {
        if (!monster->isAlive()) continue;

        sf::FloatRect monsterBounds = monster->getBounds();
        sf::FloatRect playerBounds = player.getBounds();

        if (playerBounds.intersects(monsterBounds)) 
        {
            if (player.getVelocity().y > 0.f && 
                (playerBounds.top + playerBounds.height <= monsterBounds.top + 25.f)) 
            {
                monster->takeDamage(1);
                player.bounce();
                soundManager.playJump();
            } 
            else 
            {
                if (currentState != GameState::GameOver) {
                    soundManager.playLose();
                    currentState = GameState::GameOver;
                    highScoreMgr.reportScore(currentDifficulty, player.getScore());
                }
                break;
            }
        }
    }
}

void Game::handleScrolling(float dt) 
{
    sf::Vector2f playerPos = player.getPosition();
    float scrollThreshold = 400.f; 

    if (playerPos.y < scrollThreshold)
    {
        float offsetY = scrollThreshold - playerPos.y;
        player.setPosition(sf::Vector2f(playerPos.x, scrollThreshold));

        int points = offsetY / 20;
        if (points < 1) points = 1;
        player.addScore(points);

        for (auto it = platforms.begin(); it != platforms.end(); )
        {
            Platform* platform = *it;
            platform->scroll(offsetY);

            if (platform->getPosition().y > float(Config::Window::HEIGHT))
            {
                delete platform;
                it = platforms.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = monsters.begin(); it != monsters.end(); )
        {
            (*it)->setPosition(sf::Vector2f((*it)->getPosition().x, (*it)->getPosition().y + offsetY));

            if ((*it)->getPosition().y > float(Config::Window::HEIGHT) + 50.f) {
                delete *it;
                it = monsters.erase(it); 
            }
            else {
                ++it;
            }
        }

        for (auto it = holes.begin(); it != holes.end(); ) {
            (*it)->scroll(offsetY);
            if ((*it)->getPosition().y > float(Config::Window::HEIGHT) + 50.f) {
                delete *it;
                it = holes.erase(it);
            } 
            else {
                ++it;
            }
        }

        while (platforms.size() < 12) 
        {
            float highestY = float(Config::Window::HEIGHT);
            for (auto* p : platforms) {
                if (p->getPosition().y < highestY) {
                    highestY = p->getPosition().y;
                }
            }
            
            float newSpawnY = highestY - (70.f + std::rand() % 60);
            spawnPlatform(newSpawnY);
        }
    }
}

void Game::updateGameOver(float dt) 
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (restartButtonSprite.getGlobalBounds().contains(mousePosF)) 
        {
            for (auto* platform : platforms) {
                delete platform;
            }
            platforms.clear();
            clearMonsters();

            player.setPosition(sf::Vector2f(300.f, 600.f));
            player.setVelocity(sf::Vector2f(0.f, 0.f));
            player.resetScore();
            resetGame();
            currentState = GameState::Gameplay;
        }
        else if (menuButtonSprite.getGlobalBounds().contains(mousePosF)) 
        {
            currentState = GameState::Menu;
        }
    }
}

void Game::renderMenu() 
{
    window.clear();
    window.draw(backgroundSprite);

    window.draw(titleText);
    window.draw(startButtonSprite);
    window.draw(settingsButtonSprite); 
    highScoreText.setString("High Score (" + DifficultyConfig::name(currentDifficulty) + "): " + 
                            to_string(highScoreMgr.get(currentDifficulty)));    
    float hsX = (float(Config::Window::WIDTH) - highScoreText.getGlobalBounds().width) / 2.f;
    highScoreText.setPosition(hsX, 220.f);
    window.draw(highScoreText);
}

void Game::renderSettings()
{
    window.clear();
    window.draw(backgroundSprite); 
    settingsMenu.render(window);   
}

void Game::renderGameplay() 
{
    window.clear();
    window.draw(backgroundSprite);

    for (auto* platform : platforms) {
        platform->render(window);
    }

    for (auto* hole : holes) {
        hole->render(window);
    }

    for (auto bullet : bullets) {
        bullet->render(window); 
    }
    
    for (auto* monster : monsters) {
        monster->render(window);
    }
    player.render(window);
    window.draw(scoreText);
}

void Game::renderGameOver() 
{
    window.clear();
    window.draw(backgroundSprite);

    gameOverTitleText.setPosition((float(Config::Window::WIDTH) - gameOverTitleText.getGlobalBounds().width) / 2.f, 150.f);
    window.draw(gameOverTitleText);

    finalScoreText.setString("Your Score: " + std::to_string(player.getScore()));
    finalScoreText.setPosition((float(Config::Window::WIDTH) - finalScoreText.getGlobalBounds().width) / 2.f, 260.f);
    window.draw(finalScoreText);

    gameOverHighScoreText.setString("High Score (" + DifficultyConfig::name(currentDifficulty) + "): " + 
                                    to_string(highScoreMgr.get(currentDifficulty)));
    gameOverHighScoreText.setPosition((float(Config::Window::WIDTH) - gameOverHighScoreText.getGlobalBounds().width) / 2.f, 330.f);
    window.draw(gameOverHighScoreText);

    window.draw(restartButtonSprite);
    window.draw(menuButtonSprite);
}

bool Game::isAreaFree(const sf::FloatRect& area) const {
    for (auto* platform : platforms) 
        if (platform->getBounds().intersects(area)) return false;
    
    for (auto* monster : monsters)
        if (monster->getBounds().intersects(area)) return false;

    for (auto* hole : holes)
        if (hole->getBounds().intersects(area)) return false;
    
    return true;
}

void Game::generateInitialPlatforms() {
    sf::Vector2f playerPos = player.getPosition();

    float firstPlatformX = playerPos.x - 20.f; 
    float firstPlatformY = playerPos.y + 80.f; 

    platforms.push_back(new NormalPlatform(textures.get("platform_normal"), sf::Vector2f(firstPlatformX, firstPlatformY)));

    float currentY = firstPlatformY - (85.f + std::rand() % 40); 
    while (currentY > 0.f) {
        spawnPlatform(currentY);
        float gapY = 85.f + static_cast<float>(std::rand() % 40);
        currentY -= gapY;
    }
}

void Game::spawnPlatform(float yPosition) {
    const sf::Vector2f platformSize(60.f, 20.f);
    float xPosition = 0.f;

    //find somewhere without any overlap
    for (int attempt = 0; attempt < 15; ++attempt) {
        xPosition = static_cast<float>(std::rand() % (Config::Window::WIDTH - 60));
        sf::FloatRect candidate(xPosition, yPosition, platformSize.x, platformSize.y);
        if (isAreaFree(candidate)) break;
    }

    int randType = std::rand() % 100;

    if (randType < 70) {
        NormalPlatform* np = new NormalPlatform(textures.get("platform_normal"), sf::Vector2f(xPosition, yPosition));        
        if (std::rand() % 100 < 10) {
            np->addSpring(textures.get("spring"));
        }
        platforms.push_back(np);
    } 
    else if (randType < 90) {
        platforms.push_back(new MovingPlatform(textures.get("platform_moving"), sf::Vector2f(xPosition, yPosition), settings.movingPlatformSpeed));
    } 
    else {
        platforms.push_back(new BrokenPlatform(textures.get("platform_broken"), sf::Vector2f(xPosition, yPosition)));
    }

    spawnMonster(yPosition);

    if (currentDifficulty == Difficulty::HARD)
        spawnHole(yPosition);
}

void Game::spawnMonster(float yPosition) {
    if (player.getScore() < 100) return;
    if (rand() % 100 >= 15) return;

    const sf::Vector2f monsterSize(50.f, 50.f); 

    for (int attempt = 0; attempt < 15; ++attempt) {
        float xPos = static_cast<float>(rand() % (Config::Window::WIDTH - 160) + 80);
        float monsterY = yPosition - 60.f;
        sf::FloatRect candidate(xPos, monsterY, monsterSize.x, monsterSize.y);

        if (isAreaFree(candidate)) {
            MonsterType type = (rand() % 2 == 0) ? MonsterType::Green : MonsterType::Blue;

            const sf::Texture& tex = (type == MonsterType::Green) ? 
                                     textures.get("monster_green") : 
                                     textures.get("monster_blue");

            monsters.push_back(new Monster(sf::Vector2f(xPos, monsterY), tex, type, settings.monsterHealth));
            return; 
        }
    }
}

void Game::spawnHole(float yPosition) 
{
    if (player.getScore() < 100) return;
    if (rand() % 100 >= 10) return;

    HoleSize size = (rand() % 2 == 0) ? HoleSize::Small : HoleSize::Large;
    
    const sf::Texture& holeTex = (size == HoleSize::Small) ? 
                                 textures.get("hole_small") : 
                                 textures.get("hole_large");

    //getting real size of texture
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
        float xPos = static_cast<float>(rand() % (Config::Window::WIDTH - static_cast<int>(holeDims.x) - 40) + 20);
        float holeY = yPosition - (holeDims.y / 2.f);
        
        sf::FloatRect candidate(xPos, holeY, holeDims.x, holeDims.y);

        if (isSafePath(candidate)) {
            holes.push_back(new Hole(sf::Vector2f(xPos, holeY), holeTex, size));
            return;
        }
    }
}

void Game::initUI() 
{
    if (!font.loadFromFile("fonts/ariblk.ttf")) 
    {
        std::cerr << "Error: Could not load font from fonts/ariblk.ttf!" << std::endl;
        throw std::runtime_error("Failed to load critical game font.");
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24); 
    scoreText.setFillColor(sf::Color::Black); 
    scoreText.setPosition(20.f, 20.f); 

    // main menu:
    titleText.setFont(font);
    titleText.setString("DOODLE JUMP");
    titleText.setCharacterSize(55); 
    titleText.setFillColor(sf::Color::Black);
    titleText.setStyle(sf::Text::Bold);
    float titleX = (float(Config::Window::WIDTH) - titleText.getGlobalBounds().width) / 2.f;
    titleText.setPosition(titleX, 150.f); 

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(26);            
    highScoreText.setFillColor(sf::Color(0, 102, 204)); 
    highScoreText.setStyle(sf::Text::Bold);
    highScoreText.setPosition(200.f, 530.f);

    // game over
    gameOverTitleText.setFont(font);
    gameOverTitleText.setString("GAME OVER");
    gameOverTitleText.setCharacterSize(55);
    gameOverTitleText.setFillColor(sf::Color::Red);
    gameOverTitleText.setStyle(sf::Text::Bold);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(30);
    finalScoreText.setFillColor(sf::Color::Black);

    gameOverHighScoreText.setFont(font);
    gameOverHighScoreText.setCharacterSize(26);
    gameOverHighScoreText.setFillColor(sf::Color(0, 102, 204));
    gameOverHighScoreText.setStyle(sf::Text::Bold);
}

void Game::setDifficulty(Difficulty level) {
    currentDifficulty = level;
    settings = DifficultyConfig::getSettings(level);
}

void Game::updateSettingsFromMenu() {
    settingsManager.setVolume(settingsMenu.getVolume());
    settingsManager.setDifficulty(settingsMenu.getDifficulty());
    setDifficulty(settingsMenu.getDifficulty());
    soundManager.setVolume(settingsMenu.getVolume());
}