#pragma once

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

struct DifficultySettings {
    int monsterHealth;          
    float movingPlatformSpeed;  
    float monsterSpawnRate;     
    float fireRate;             
};

class DifficultyConfig {
public:
    static DifficultySettings getSettings(Difficulty level) {
        switch (level) {
            case Difficulty::EASY:
                return DifficultySettings{ 3, 130.f, 0.08f, 0.25f };
            case Difficulty::MEDIUM:
                return DifficultySettings{ 4, 170.f, 0.13f, 0.20f };
            case Difficulty::HARD:
                return DifficultySettings{ 5, 270.f, 0.23f, 0.15f };
            default:
                return DifficultySettings{ 3, 110.f, 0.05f, 0.15f };
        }
    }

    static string name(Difficulty d) {
        switch (d) {
            case Difficulty::EASY:   return "EASY";
            case Difficulty::MEDIUM: return "MEDIUM";
            case Difficulty::HARD:   return "HARD";
        }
        return "";
    }
};