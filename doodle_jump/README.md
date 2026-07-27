# 🚀 Doodle Jump - 2D Game Engine (Phase 1 & 2)

A modular, component-based 2D platformer game engine inspired by the classic **Doodle Jump**, built from scratch using **C++** and **SFML**. 
This project demonstrates clean code practices, Object-Oriented Programming (OOP) design patterns, real-time physics handling, and a flexible directory architecture designed for scalability.

---

## ✨ Key Features

* **Modular Architecture**: Decoupled systems separating core logic, game entities, user interfaces, and resource management.
* **Entity Management**: Varied platform types (Normal, Moving, Broken), player physics, monsters, bullets, and black hole hazards.
* **System Managers**:
  * `SoundManager`: Handles background music, jump effects, and dynamic audio cues.
  * `Difficulty`: Scales difficulty dynamically based on player height and progress.
  * `HighScoreManager` & `SettingManager`: Handles persistence for game configurations and high scores.
  * `ResourceManager`: Centralized texture and font loading.
* **Interactive UI**: Custom menu screens, game over sequences, and setting overlays.

---

📁 Directory Structure
The project was refactored into a clean, multi-tier directory structure to ensure maintainability:

## 📁 Directory Structure

The project was refactored into a clean, multi-tier directory structure to ensure maintainability:

```text
doodle_jump/
├── include/
│   ├── core/         # Engine core, Game loop, State management, Constants
│   ├── entities/     # Player, Platforms (Normal, Moving, Broken), Monster, Bullet, Hole
│   ├── systems/      # SoundManager, Difficulty, HighScoreManager, SettingManager
│   ├── ui/           # MenuScreen, GameOverScreen, SettingMenu
│   ├── utils/        # ResourceManager
│   └── world/        # GameWorld logic & physics step
├── src/              # Implementation files matching header hierarchy
├── assets/           # Textures, Fonts, Audio files
└── Makefile          # Recursive build setup with automatic dependency resolution
--- 

## 🛠️ Tech Stack & Requirements

* **Language**: C++ (C++17 recommended)
* **Graphics & Audio Library**: [SFML (Simple and Fast Multimedia Library)](https://www.sfml-dev.org/)
* **Build System**: `Make` (Supports GCC / Clang)
* **Environment Compatibility**: Windows (MSYS2 / UCRT64), WSL2, Linux, macOS

---

## 🎮 Controls

* **Move Left:** `A` or `Left Arrow`
* **Move Right:** `D` or `Right Arrow`
* **Shoot Bullet:** `Space`

---

## 🎓 Academic Context

Developed as part of the **Advanced Programming (AP)** course curriculum at the **University of Tehran**. Focuses on software design patterns, C++ memory management, SFML graphics rendering, and modular architecture.
