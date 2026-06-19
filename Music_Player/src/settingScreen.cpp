#include "settingScreen.h"
#include <iostream>

settingScreen::settingScreen(UIRender& ui, InputHandler& input,
                                player& p, configManager& config, int& currentScreen)
    : Screen(ui, input), player_(p), config(config), currentScreen(currentScreen) {}


void settingScreen::render() 
{
    string current = player_.getModeString();

    ui.clearScreen();
    ui.printTopBorder("⚙ Settings", 56);
    ui.printSection({ "Playback Mode (current: " + current + ")" });

    ui.printSection({
        "1. NO_REPEAT    play in order, stop at end"
            + string(player_.getMode() == NO_REPEAT  ? "  ◀ active" : ""),
        "2. REPEAT_ONE   repeat current song forever"
            + string(player_.getMode() == REPEAT_ONE ? "  ◀ active" : ""),
        "3. REPEAT_ALL   loop whole playlist"
            + string(player_.getMode() == REPEAT_ALL ? "  ◀ active" : ""),
        "4. SHUFFLE      random order"
            + string(player_.getMode() == SHUFFLE    ? "  ◀ active" : "")
    });

    ui.printSection({ "0. Back (changes saved automatically)" });
    ui.printBottomBorder();
    cout << " Choice: ";
}

void settingScreen::handleInput() {
    int choice = input.readInt(0, 4);

    if (choice == 0) {
        currentScreen = 0; //back to Main Menu
        return;
    }

    playbackMode newMode;
    switch (choice) {
        case 1: newMode = NO_REPEAT;  break;
        case 2: newMode = REPEAT_ONE; break;
        case 3: newMode = REPEAT_ALL; break;
        case 4: newMode = SHUFFLE;    break;
    }

    player_.setMode(newMode);
    config.set("playback_mode", player_.getModeString());
    config.save();
}