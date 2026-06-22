#include "mainMenu.h"

MainMenuScreen::MainMenuScreen(UIRender &ui, InputHandler &input,
                                player &p, configManager &config,
                                bool &running, int &currentScreen)
    : Screen(ui, input), player_(p), config(config),
      running(running), currentScreen(currentScreen) {}

void MainMenuScreen::render()
{
    string lastSong = config.get("last_song_title", "None");

    ui.clearScreen();
    ui.printTopBorder(UIRender::BOLD + UIRender::MAGENTA + "Terminal Music Player" + UIRender::RESET, 56);
    ui.printSection({
        string("Last played: ") + UIRender::GRAY + lastSong + UIRender::RESET 
    });

    ui.printSection({
        UIRender::CYAN + string("1. Now Playing") + UIRender::RESET,
        UIRender::CYAN + string("2. Playlists") + UIRender::RESET,
        UIRender::CYAN + string("3. Browse Playlist") + UIRender::RESET,
        UIRender::CYAN + string("4. Settings") + UIRender::RESET
    });
    ui.printSection({ UIRender::GRAY + string("0. Quit (saves state)") + UIRender::RESET});
    ui.printBottomBorder();
}

void MainMenuScreen::handleInput() 
{
    cout << "Enter your choice: ";
    int choice = input.readInt(0, 4);
    switch (choice) {
        case 0: running = false;   break;
        case 1: currentScreen = 1; break;
        case 2: currentScreen = 2; break;
        case 3: currentScreen = 3; break;
        case 4: currentScreen = 4; break;
    }
}