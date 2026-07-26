#pragma once
#include <fstream>
#include <array>
#include <string>
#include "Difficulty.hpp"

class HighScoreManager {
public:
    HighScoreManager() { load(); }

    int get(Difficulty d) const { return scores[idx(d)]; }

    bool reportScore(Difficulty d, int score) {
        std::size_t i = idx(d);
        if (score > scores[i]) {
            scores[i] = score;
            save();
            return true;
        }
        return false;
    }

private:
    std::array<int, 3> scores{0, 0, 0}; // EASY, MEDIUM, HARD

    static std::size_t idx(Difficulty d) { return static_cast<std::size_t>(d); }

    void load() {
        std::ifstream in("highscore.txt");
        if (in.is_open()) { 
            in >> scores[0] >> scores[1] >> scores[2]; 
            in.close(); 
        }
    }

    void save() const {
        std::ofstream out("highscore.txt");
        if (out.is_open()) { 
            out << scores[0] << " " << scores[1] << " " << scores[2]; 
            out.close(); 
        }
    }
};