#include "mainMenu.h"

MainMenuScreen::MainMenuScreen(UIRender &ui, InputHandler &input,
                                player &p, configManager &config,
                                bool &running, int &currentScreen)
    : Screen(ui, input), player_(p), config(config),
      running(running), currentScreen(currentScreen) {}

void MainMenuScreen::render()
{
    string lastSong = config.get("last_song", "None");

    ui.clearScreen();
    ui.printTopBorder("Terminal Music Player", 56);
    ui.printSection({ "Last played: " + lastSong });
    ui.printSection({
        "1. Now Playing",
        "2. Playlists",
        "3. Browse Playlist",
        "4. Settings"
    });
    ui.printSection({ "0. Quit (saves state)" });
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