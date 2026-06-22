# 🎵 Terminal Music Player (C++)

A simple **terminal-based music player and library manager** written in **C++**.  
The application runs completely in the **command line (CLI/TUI)** and demonstrates object-oriented design, file handling, and modular multi-file C++ architecture.

This project manages songs stored on disk, allows playlist organization, and provides basic playback controls through a terminal interface.

---

## ✨ Features

- 🎧 Playback controls  
  - Play  
  - Pause / Resume  
  - Stop  
  - Next / Previous  

- 📚 Music library management  
  - Store and organize songs  
  - Load songs from external files  

- 🎶 Playlist system  
  - Create and manage playlists  
  - Browse songs inside playlists  

- 📁 File loading  
  - Import songs from **CSV files**  
  - Import playlists from **M3U files**

- 🔀 Playback options  
  - Shuffle mode  
  - Different Repeat modes

- ⚙️ Configuration system  
  - Save last played song  
  - Save active playlist  
  - Restore player state on startup  

- 🖥️ Terminal user interface  
  - Screen-based navigation  
  - Colored terminal output  

---

## 🖥️ Application Screens

The interface is organized into several screens:

- **Now Playing**  
  Displays the current song and playback status.

- **Playlists**  
  Shows all available playlists.

- **Browse Playlist**  
  Allows users to view songs inside a selected playlist.

- **Settings**  
  Manages configuration and saved player state.

---

## 📂 Project Structure

The project is divided into several classes:

- `Application` — controls the main program flow
- `Player` — handles music playback
- `MusicLibrary` — stores and manages songs
- `Playlist` — manages playlist data
- `Song` — represents song information
- `ConfigManager` — saves and loads settings
- `CSVLoader` / `M3ULoader` — load music data from files
- `UIRender` — handles terminal output
- Screen classes — manage different parts of the UI

## 🛠️ Build

Compile with `g++` using C++17:
```bash
g++ -std=c++17 $(Get-ChildItem *.cpp -Name) -o player.exe -lwinmm -lole32 -luuid
