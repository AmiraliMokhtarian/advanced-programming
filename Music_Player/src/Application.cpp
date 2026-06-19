#include "Application.h"
#include "csvLoader.h"
#include "m3uLoader.h"
#include <iostream>
using namespace std;

Application::Application()
    : config("../Data/settings.cfg"),
      running(true),
      currentScreen(0),
      activePlaylistIndex(0),
      mainMenu(ui, input, player_, config, running, currentScreen),
      nowPlaying(ui, input, player_, currentScreen, config),
      playlistMenu(ui, input, playlists, player_, currentScreen, activePlaylistIndex),
      browsePlaylist(ui, input, playlists, player_, config, currentScreen, activePlaylistIndex),
      settings(ui, input, player_, config, currentScreen)
{}

void Application::init() {
    config.load();

    try {
        CsvLoader csvLoader;
        csvLoader.load("../Data/library.csv", library);
    } catch (const exception& e) {
        cerr << "Fatal: could not load library: " << e.what() << "\n";
        running = false;
        return;
    }

    try {
        m3uLoader m3u;
        playlists = m3u.loadAll("../Data/Playlists", library);
    } catch (const exception& e) {
        cerr << "Warning: could not load playlists: " << e.what() << "\n";
        // not fatal — app can still run with no playlists
    }
}

Screen* Application::getCurrentScreen() {
    switch (currentScreen) {
        case 0: return &mainMenu;
        case 1: return &nowPlaying;
        case 2: return &playlistMenu;
        case 3: return &browsePlaylist;
        case 4: return &settings;
    }
    return &mainMenu;
}

void Application::run() {
    init();
    if (!running) return;  // init failed

    while (running) {
        player_.tick();
        Screen* screen = getCurrentScreen();
        screen->render();
        screen->handleInput();
    }

    //save state on quit
    if (player_.getCurrentSong()) {
        config.set("last_song", player_.getCurrentSong()->getFilePath());
        config.set("last_song_title", player_.getCurrentSong()->getTitle());
    }
    config.set("playback_mode", player_.getModeString());
    config.save();

    ui.clearScreen();
    cout << "Goodbye!\n";
}