#pragma once
#include "MusicLibrary.h"
#include "player.h"
#include "configManager.h"
#include "UIRender.h"
#include "inputHandler.h"
#include "playlist.h"
#include "mainMenu.h"
#include "nowPlaying.h"
#include "playlistScreen.h"
#include "browsePlaylist.h"
#include "settingScreen.h"
#include <vector>

using namespace std;

class Application {
private:
    MusicLibrary    library;
    player          player_;
    configManager   config;
    UIRender          ui;
    InputHandler      input;
    vector<playlist*> playlists;

    bool running;
    int currentScreen;
    int activePlaylistIndex;

    MainMenuScreen          mainMenu;
    NowPlayingScreen        nowPlaying;
    playlistScreen          playlistMenu;
    BrowsePlaylistScreen    browsePlaylist;
    settingScreen           settings;

    Screen* getCurrentScreen();

public:
    Application();
    void init();
    void run();
};