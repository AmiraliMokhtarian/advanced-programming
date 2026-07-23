#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

using namespace std;

class SoundManager {
public:
    SoundManager() = default;

    bool playMusic(const string& filepath) {
        if (!bgMusic.openFromFile(filepath)) {
            cerr << "[SoundManager] Failed to load music: " << filepath << endl;
            return false;
        }
        bgMusic.setLoop(true);
        bgMusic.play();
        return true;
    }

    void setVolume(float volume) {
        bgMusic.setVolume(volume);
    }

    float getVolume() const {
        return bgMusic.getVolume();
    }

private:
    sf::Music bgMusic;
};