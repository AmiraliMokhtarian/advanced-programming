#include "SoundManager.hpp"
#include <iostream>
#include <fstream>

SoundManager::SoundManager(ResourceManager<sf::SoundBuffer>& soundResources, float initialVolume) 
    : soundBuffers(soundResources) , currentVolume(initialVolume) 
{
    initSFX();
}

SoundManager::~SoundManager() 
{
    stopMusic();
    jumpSound.stop();
    loseSound.stop();
    shootSound.stop();
}


bool SoundManager::playMusic(const std::string& filepath) 
{
    if (currentMusicPath != filepath) {
        if (!bgMusic.openFromFile(filepath)) {
            std::cerr << "[SoundManager] Failed to load music: " << filepath << std::endl;
            currentMusicPath = "";
            return false;
        }
        currentMusicPath = filepath;
        bgMusic.setLoop(true);
        bgMusic.setVolume(currentVolume);
    }

    if (bgMusic.getStatus() != sf::SoundSource::Playing) {
        bgMusic.play(); 
    }
    return true;
}


void SoundManager::stopMusic() 
{
    if (bgMusic.getStatus() != sf::SoundSource::Stopped) {
        bgMusic.pause();
    }
}

bool SoundManager::isMusicPlaying() const 
{
    return bgMusic.getStatus() == sf::SoundSource::Playing;
}

void SoundManager::playJump() 
{
    jumpSound.stop();
    jumpSound.play();
}

void SoundManager::playLose() 
{
    loseSound.stop();
    loseSound.play();
}

void SoundManager::playShoot() 
{
    shootSound.stop();
    shootSound.play();
}

void SoundManager::setVolume(float volume) 
{
    currentVolume = volume;
    bgMusic.setVolume(currentVolume);

    jumpSound.setVolume(currentVolume);
    loseSound.setVolume(currentVolume);
    shootSound.setVolume(currentVolume);
}

float SoundManager::getVolume() const 
{
    return currentVolume;
}

void SoundManager::initSFX() 
{
    try {
        soundBuffers.load("jump", "sounds/Jumping_Sound.wav");
        soundBuffers.load("lose", "sounds/Loosing_Sound.wav");
        soundBuffers.load("shoot", "sounds/Shooting_Sound.wav");

        jumpSound.setBuffer(soundBuffers.get("jump"));
        jumpSound.setVolume(currentVolume);

        loseSound.setBuffer(soundBuffers.get("lose"));
        loseSound.setVolume(currentVolume);

        shootSound.setBuffer(soundBuffers.get("shoot"));
        shootSound.setVolume(currentVolume);
    }
    catch (const std::exception& e) {
        std::cerr << "[SoundManager] Error loading sound buffers: " << e.what() << std::endl;
    }
}

