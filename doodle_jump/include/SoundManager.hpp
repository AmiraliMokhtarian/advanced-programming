#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class SoundManager {
public:
    SoundManager() {
        ifstream inFile("settings.txt");
        if (inFile.is_open()) {
            inFile >> currentVolume;
            inFile.close();
        } else {
            currentVolume = 50.0f; 
        }

        bgMusic.setVolume(currentVolume);
        loadSFX();
    }

    ~SoundManager() {
        ofstream outFile("settings.txt");
        if (outFile.is_open()) {
            outFile << currentVolume;
            outFile.close();
        }
    }

    bool playMusic(const string& filepath) {
        if (currentMusicPath == filepath && bgMusic.getStatus() == sf::Music::Playing) {
            return true;
        }

        if (!bgMusic.openFromFile(filepath)) {
            cerr << "[SoundManager] Failed to load music: " << filepath << endl;
            return false;
        }

        currentMusicPath = filepath;
        bgMusic.setLoop(true);
        bgMusic.setVolume(currentVolume); 
        bgMusic.play();
        return true;
    }

    void stopMusic() {
        if (bgMusic.getStatus() == sf::Music::Playing) {
            bgMusic.stop();
            currentMusicPath = "";
        }
    }

    bool isMusicPlaying() const {
        return bgMusic.getStatus() == sf::Music::Playing;
    }

    void playJump() {
        jumpSound.stop();
        jumpSound.play();
    }

    void playLose() {
        loseSound.stop();
        loseSound.play();
    }

    void setVolume(float volume) {
        currentVolume = volume;
        bgMusic.setVolume(currentVolume);

        jumpSound.setVolume(currentVolume);
        loseSound.setVolume(currentVolume);
    }

    float getVolume() const {
        return currentVolume;
    }

private:
    void loadSFX() {
        if (jumpBuffer.loadFromFile("sounds/Jumping_Sound.wav")) {
            jumpSound.setBuffer(jumpBuffer);
            jumpSound.setVolume(currentVolume);
        } else {
            cerr << "[SoundManager] Failed to load Jumping_Sound.wav\n";
        }

        if (loseBuffer.loadFromFile("sounds/Loosing_Sound.wav")) {
            loseSound.setBuffer(loseBuffer);
            loseSound.setVolume(currentVolume);
        } else {
            cerr << "[SoundManager] Failed to load Loosing_Sounds.wav\n";
        }
    }

    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer loseBuffer;
    sf::Sound jumpSound;
    sf::Sound loseSound;

    sf::Music bgMusic;
    float currentVolume{50.0f};
    string currentMusicPath{""};
};