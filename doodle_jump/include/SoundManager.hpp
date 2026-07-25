#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include "ResourceManager.hpp"

class SoundManager {
public:
    explicit SoundManager(ResourceManager<sf::SoundBuffer>& soundResources, float initialVolume);
    ~SoundManager();

    bool playMusic(const std::string& filepath);
    void stopMusic();
    bool isMusicPlaying() const;

    void playJump();
    void playLose();
    void playShoot();

    void setVolume(float volume);
    float getVolume() const;

private:
    void initSFX();

    ResourceManager<sf::SoundBuffer>& soundBuffers;

    sf::Sound jumpSound;
    sf::Sound loseSound;
    sf::Sound shootSound;

    sf::Music bgMusic;
    float currentVolume{50.0f};
    std::string currentMusicPath{""};
};