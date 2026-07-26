#include "Game.hpp"
#include "Constants.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

Game::Game()
    : window(sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT), "Doodle Jump")
    , textures()
    , font(loadFont())
    , currentState(GameState::Menu)
    , previousState(GameState::GameOver)
    , soundBuffers()
    , settingsManager()
    , highScoreMgr()
    , soundManager(soundBuffers, settingsManager.getVolume())
    , player(textures.load("player_left", "assets/left_doodle.png"),
             textures.load("player_right", "assets/right_doodle.png"))
    , settingsMenu(static_cast<float>(Config::Window::WIDTH), static_cast<float>(Config::Window::HEIGHT),
                   textures, font, settingsManager.getVolume(), settingsManager.getDifficulty())
    , menuScreen(static_cast<float>(Config::Window::WIDTH), textures, font)
    , gameOverScreen(static_cast<float>(Config::Window::WIDTH), textures, font)
    , world(textures)
{
    window.setFramerateLimit(Config::Window::FPS_LIMIT);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    loadTextures();
    initUI();

    setDifficulty(settingsManager.getDifficulty());
    world.setDifficulty(currentDifficulty);
    world.generateInitial(player.getPosition());
}

sf::Font Game::loadFont()
{
    sf::Font f;
    if (!f.loadFromFile("fonts/ariblk.ttf")) {
        throw std::runtime_error("Failed to load critical game font.");
    }
    return f;
}

void Game::loadTextures()
{
    textures.load("background", "assets/background.png");
    backgroundSprite.setTexture(textures.get("background"));

    textures.load("shoot_body", "assets/Shooting@Pose.png");
    textures.load("nose", "assets/Nose.png");
    player.setShootingTextures(textures.get("shoot_body"), textures.get("nose"));
}

void Game::initUI()
{
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(20.f, 20.f);
}

void Game::resetGame()
{
    player.cancelAbsorption();
    player.setPosition(sf::Vector2f(300.f, 600.f));
    player.setVelocity(sf::Vector2f(0.f, 0.f));
    player.resetScore();

    world.reset(player.getPosition());
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

            // فقط اعمال زنده (بدون نوشتن روی دیسک)
            setDifficulty(settingsMenu.getDifficulty());
            soundManager.setVolume(settingsMenu.getVolume());

            if (backPressed) {
                // فقط اینجا واقعاً ذخیره کن
                settingsManager.setVolume(settingsMenu.getVolume());
                settingsManager.setDifficulty(settingsMenu.getDifficulty());
                currentState = GameState::Menu;
            }
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
        soundManager.stopMusic();
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        auto action = menuScreen.handleClick(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));

        if (action == MenuScreen::Action::StartGame) {
            resetGame();
            currentState = GameState::Gameplay;
        } else if (action == MenuScreen::Action::OpenSettings) {
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
        return;
    }

    player.handleInput();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player.setShooting(true);

        if (fireClock.getElapsedTime().asSeconds() >= settings.fireRate) {
            sf::Vector2f noseTip = player.getNoseTipPosition();
            world.fireBullet(noseTip);
            soundManager.playShoot();
            fireClock.restart();
        }
    } else {
        player.setShooting(false);
    }

    player.update(dt, window.getSize().x);

    world.updateBullets(dt);
    world.updateEntities(dt);

    CollisionOutcome outcome = world.checkCollisions(player, soundManager);
    if (outcome == CollisionOutcome::PlayerDied) {
        soundManager.playLose();
        highScoreMgr.reportScore(currentDifficulty, player.getScore());
        currentState = GameState::GameOver;
        return;
    }
    if (player.isBeingAbsorbed()) return;

    handleScrolling(dt);

    scoreText.setString("Score: " + std::to_string(player.getScore()));

    if (player.getPosition().y > float(Config::Window::HEIGHT)) {
        soundManager.playLose();
        highScoreMgr.reportScore(currentDifficulty, player.getScore());
        currentState = GameState::GameOver;
    }
}

void Game::handleScrolling(float dt)
{
    sf::Vector2f playerPos = player.getPosition();
    float scrollThreshold = 400.f;

    if (playerPos.y < scrollThreshold) {
        float offsetY = scrollThreshold - playerPos.y;
        player.setPosition(sf::Vector2f(playerPos.x, scrollThreshold));

        int points = offsetY / 20;
        if (points < 1) points = 1;
        player.addScore(points);

        world.scrollBy(offsetY, player.getScore());
    }
}

void Game::updateGameOver(float dt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        auto action = gameOverScreen.handleClick(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));

        if (action == GameOverScreen::Action::Restart) {
            resetGame();
            currentState = GameState::Gameplay;
        } else if (action == GameOverScreen::Action::BackToMenu) {
            currentState = GameState::Menu;
        }
    }
}

void Game::renderMenu()
{
    menuScreen.render(window, backgroundSprite, float(Config::Window::WIDTH), currentDifficulty, highScoreMgr);
}

void Game::renderSettings()
{
    window.draw(backgroundSprite);
    settingsMenu.render(window);
}

void Game::renderGameplay()
{
    window.draw(backgroundSprite);
    world.render(window);
    player.render(window);
    window.draw(scoreText);
}

void Game::renderGameOver()
{
    gameOverScreen.render(window, backgroundSprite, float(Config::Window::WIDTH),
                          player.getScore(), currentDifficulty, highScoreMgr);
}

void Game::setDifficulty(Difficulty level)
{
    currentDifficulty = level;
    settings = DifficultyConfig::getSettings(level);
    world.setDifficulty(level);
}

void Game::updateSettingsFromMenu()
{
    settingsManager.setVolume(settingsMenu.getVolume());
    settingsManager.setDifficulty(settingsMenu.getDifficulty());
    setDifficulty(settingsMenu.getDifficulty());
    soundManager.setVolume(settingsMenu.getVolume());
}