#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include "ResourceManager.hpp"

class SoundManager {
public:
    explicit SoundManager(ResourceManager<sf::SoundBuffer>& soundResources) 
        : soundBuffers(soundResources) 
    {
        std::ifstream inFile("settings.txt");
        if (inFile.is_open()) {
            inFile >> currentVolume;
            inFile.close();
        } else {
            currentVolume = 50.0f; 
        }

        bgMusic.setVolume(currentVolume);
        initSFX();
    }

    ~SoundManager() {
        stopMusic();
        jumpSound.stop();
        loseSound.stop();
        shootSound.stop();

        std::ofstream outFile("settings.txt");
        if (outFile.is_open()) {
            outFile << currentVolume;
            outFile.close();
        }
    }

    bool playMusic(const std::string& filepath) {
        if (currentMusicPath == filepath && bgMusic.getStatus() == sf::SoundSource::Playing) {
            return true;
        }

        bgMusic.stop();

        if (!bgMusic.openFromFile(filepath)) {
            std::cerr << "[SoundManager] Failed to load music: " << filepath << std::endl;
            currentMusicPath = "";
            return false;
        }

        currentMusicPath = filepath;
        bgMusic.setLoop(true);
        bgMusic.setVolume(currentVolume); 
        bgMusic.play();
        return true;
    }

    void stopMusic() {
        if (bgMusic.getStatus() != sf::SoundSource::Stopped) {
            bgMusic.stop();
        }
    }

    bool isMusicPlaying() const {
        return bgMusic.getStatus() == sf::SoundSource::Playing;
    }

    void playJump() {
        jumpSound.stop();
        jumpSound.play();
    }

    void playLose() {
        loseSound.stop();
        loseSound.play();
    }

    void playShoot() {
        shootSound.stop();
        shootSound.play();
    }

    void setVolume(float volume) {
        currentVolume = volume;
        bgMusic.setVolume(currentVolume);

        jumpSound.setVolume(currentVolume);
        loseSound.setVolume(currentVolume);
        shootSound.setVolume(currentVolume);
    }

    float getVolume() const {
        return currentVolume;
    }

private:
    void initSFX() {
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

    ResourceManager<sf::SoundBuffer>& soundBuffers;

    sf::Sound jumpSound;
    sf::Sound loseSound;
    sf::Sound shootSound;

    sf::Music bgMusic;
    float currentVolume{50.0f};
    std::string currentMusicPath{""};
};