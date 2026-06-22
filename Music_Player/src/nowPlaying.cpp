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
    ui.printTopBorder(UIRender::MAGENTA + "Now Playing" + UIRender::RESET , 56);

    if (!s) {
        ui.printSection({ UIRender::RED + "No song loaded." + UIRender::RESET });
    } 
    else {
        ui.printSection({
            UIRender::CYAN + "Title  : " + UIRender::RESET + s->getTitle(),
            UIRender::CYAN + "Artist : " + UIRender::RESET + s->getArtist(),
            UIRender::CYAN + "Album  : " + UIRender::RESET + s->getAlbum() + " [" + to_string(s->getYear()) + "]",
            UIRender::CYAN + "Genre  : " + UIRender::RESET + s->getGenre()
        });

        ostringstream line1, line2;
        line1 << left << setw(30) << player_.getStateString()
                    << UIRender::CYAN + "Playlist: " + UIRender::RESET + config.get("active_playlist", "None");
        line2 << left << setw(30) << (UIRender::CYAN + "PlayBack Mode : " + UIRender::RESET + player_.getModeString())
                    << UIRender::CYAN + "        Duration: " + UIRender::RESET + ui.formatDuration(s->getDuration());

        ui.printSection({ line1.str(), line2.str() });
    }

    ui.printSection({ UIRender::GRAY + "[p]pause  [n]next  [b]prev  [s]stop  [f]+10s  [r]-10s" , "[q]menu" + UIRender::RESET});
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
        case 'f': player_.seekBy(10);    break;
        case 'r': player_.seekBy(-10);   break;
        case 'q': currentScreen = 0;     break;
    }
}