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
    ui.printTopBorder(UIRender::MAGENTA + "Playlists" + UIRender::RESET, 56);

    if (playlists.empty()){
        ui.printSection({ UIRender::RED + "No playlists found." + UIRender::RESET });
    } 
    
    else{
        vector<string> rows;
        rows.push_back( UIRender::RED + " #   Name                           Songs" + UIRender::RESET );

        for (int i = 0; i < (int)playlists.size(); i++) {

            ostringstream oss;

            string marker = (i == activePlaylistIndex) ? UIRender::GRAY + " ▶ " + UIRender::RESET : "  ";

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

    ui.printSection({ UIRender::GRAY + " Enter number to switch active playlist" , " [0] back" + UIRender::RESET});
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
