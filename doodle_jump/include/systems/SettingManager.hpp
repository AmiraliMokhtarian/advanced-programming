#pragma once
#include <fstream>
#include "Difficulty.hpp"

using namespace std;

class SettingsManager {
public:
    SettingsManager() { load(); }

    float getVolume() const { return volume; }
    Difficulty getDifficulty() const { return difficulty; }

    void setVolume(float v)          { volume = v;     save(); }
    void setDifficulty(Difficulty d) { difficulty = d; save(); }

private:
    float volume = 50.0f;
    Difficulty difficulty = Difficulty::EASY;

    void load() {
        ifstream in("settings.txt");
        if (in.is_open()) {
            int diffIndex = 0;
            if (in >> volume >> diffIndex) {
                difficulty = static_cast<Difficulty>(diffIndex);
            }
            in.close();
        }
    }

    void save() const {
        ofstream out("settings.txt");
        if (out.is_open()) {
            out << volume << " " << static_cast<int>(difficulty);
            out.close();
        }
    }
};