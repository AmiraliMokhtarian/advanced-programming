#pragma once
#include <fstream>
#include <array>
#include <string>
#include "Difficulty.hpp"

using namespace std;

class HighScoreManager {
public:
    HighScoreManager() { load(); }

    int get(Difficulty d) const { return scores[idx(d)]; }

    bool reportScore(Difficulty d, int score) {
        size_t i = idx(d);
        if (score > scores[i]) {
            scores[i] = score;
            save();
            return true;
        }
        return false;
    }

private:
    array<int, 3> scores{0, 0, 0}; // EASY, MEDIUM, HARD

    static size_t idx(Difficulty d) { return static_cast<size_t>(d); }

    void load() {
        ifstream in("highscore.txt");
        if (in.is_open()) { 
            in >> scores[0] >> scores[1] >> scores[2]; 
            in.close(); 
        }
    }

    void save() const {
        ofstream out("highscore.txt");
        if (out.is_open()) { 
            out << scores[0] << " " << scores[1] << " " << scores[2]; 
            out.close(); 
        }
    }
};