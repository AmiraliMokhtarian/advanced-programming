#include "nowPlaying.h"
#include <sstream>
#include <iomanip>
#include <conio.h>

NowPlayingScreen::NowPlayingScreen(UIRender& ui, InputHandler& input,
                                    player& p, int& currentScreen, configManager& config)
    : Screen(ui, input), player_(p), currentScreen(currentScreen), config(config){}


void NowPlayingScreen::render() 
{
    song* s = player_.getCurrentSong();

    ui.clearScreen();
    ui.printTopBorder("Now Playing", 56);

    if (!s) {
        ui.printSection({ "No song loaded." });
    } 
    else {
        ui.printSection({
            "Title  : " + s->getTitle(),
            "Artist : " + s->getArtist(),
            "Album  : " + s->getAlbum() + " [" + to_string(s->getYear()) + "]",
            "Genre  : " + s->getGenre()
        });

        ostringstream line1, line2;
        line1 << left << setw(30) << player_.getStateString()
                    << "Playlist: " + config.get("active_playlist", "None");
        line2 << left << setw(30) << ("PlayBack Mode : " + player_.getModeString())
                    << "Duration: " + ui.formatDuration(s->getDuration());

        ui.printSection({ line1.str(), line2.str() });
    }

    ui.printSection({ "[p]pause  [n]next  [b]prev  [s]stop  [f]+10s  [r]-10s" , "[q]menu" });
    ui.printBottomBorder();
}

void NowPlayingScreen::handleInput() {
    if (!input.keyAvailable()) return;
    char key = (char)_getch();
    switch (key) {
        case 'p': player_.togglePause(); break;
        case 'n': player_.next();        break;
        case 'b': player_.previous();    break;
        case 's': player_.stop();        break;
        case 'f': player_.seekBy(40);    break;
        case 'r': player_.seekBy(-10);   break;
        case 'q': currentScreen = 0;     break;
    }
}