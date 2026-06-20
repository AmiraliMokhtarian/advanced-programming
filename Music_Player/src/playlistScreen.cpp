#include "playlistScreen.h"
#include <iostream>
#include <sstream>
#include <iomanip>

playlistScreen::playlistScreen(UIRender& ui, InputHandler& input,
                                vector<playlist*>& playlists, player& p,
                                int& currentScreen, int& activePlaylistIndex)
    : Screen(ui, input), playlists(playlists), player_(p),
      currentScreen(currentScreen), activePlaylistIndex(activePlaylistIndex) {}


void playlistScreen::render() {

    ui.clearScreen();
    ui.printTopBorder("Playlists", 56);

    if (playlists.empty()){
        ui.printSection({ "No playlists found." });
    } 
    
    else{
        vector<string> rows;
        rows.push_back(" #   Name                           Songs");

        for (int i = 0; i < (int)playlists.size(); i++) {

            ostringstream oss;

            string marker = (i == activePlaylistIndex) ? " ▶ " : "  ";

            oss << " "
                << left
                << setw(4)  << (to_string(i + 1) + ".")
                << setw(30) << playlists[i]->getName()
                << right
                << setw(5)  << to_string(playlists[i]->getSongs().size())
                << "   "
                << marker;

            rows.push_back(oss.str());
        }
        ui.printSection(rows);
    }

    ui.printSection({ " Enter number to switch active playlist.  [0] back" });
    ui.printBottomBorder();
    cout << " Choice: ";
}


void playlistScreen::handleInput() {
    int choice = input.readInt(0, (int)playlists.size());
    if (choice == 0) {
        currentScreen = 0;
    } 
    else {
        activePlaylistIndex = choice - 1;
        currentScreen = 3;  //browsePlaylistScreen
    }
}
