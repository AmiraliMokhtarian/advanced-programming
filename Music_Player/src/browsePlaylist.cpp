#include "browsePlaylist.h"
#include <algorithm>
#include <cctype>

using namespace std;

BrowsePlaylistScreen::BrowsePlaylistScreen(UIRender& ui, InputHandler& input,
                                            vector<playlist*>& playlists, player& p,
                                            configManager& config, int& currentScreen,
                                            int& activePlaylistIndex)
    : Screen(ui, input), playlists(playlists), player_(p), config(config),
      currentScreen(currentScreen), activePlaylistIndex(activePlaylistIndex),
      sortMode(SORT_DEFAULT) {}

string BrowsePlaylistScreen::sortModeString() const 
{
    switch (sortMode) {
        case SORT_DEFAULT:  return "Default";
        case SORT_TITLE:    return "Title";
        case SORT_ARTIST:   return "Artist";
        case SORT_ALBUM:    return "Album";
        case SORT_YEAR:     return "Year";
        case SORT_DURATION: return "Duration";
    }
    return "";
}


void BrowsePlaylistScreen::rebuildDisplayList() 
{
    playlist* pl = playlists[activePlaylistIndex];
    displayList  = pl->getSongs(); //copy of pointers -> original untouched

    //search filter
    if (!searchQuery.empty()) {
        string q = searchQuery;
        transform(q.begin(), q.end(), q.begin(), ::tolower);

        displayList.erase(remove_if(displayList.begin(), displayList.end(),
            [&](song* s) {
                string title  = s->getTitle();
                string artist = s->getArtist();
                transform(title.begin(),  title.end(),  title.begin(),  ::tolower);
                transform(artist.begin(), artist.end(), artist.begin(), ::tolower);
                return title.find(q) == string::npos && artist.find(q) == string::npos;
            }), displayList.end());
    }

    //sort
    switch (sortMode) {
        case SORT_TITLE:
            sort(displayList.begin(), displayList.end(),
                [](song* a, song* b){ return a->getTitle() < b->getTitle(); });
            break;

        case SORT_ARTIST:
            sort(displayList.begin(), displayList.end(),
                [](song* a, song* b){ return a->getArtist() < b->getArtist(); });
            break;

        case SORT_ALBUM:
            sort(displayList.begin(), displayList.end(),
                [](song* a, song* b){ return a->getAlbum() < b->getAlbum(); });
            break;

        case SORT_YEAR:
            sort(displayList.begin(), displayList.end(),
                [](song* a, song* b){ return a->getYear() < b->getYear(); });
            break;

        case SORT_DURATION:
            sort(displayList.begin(), displayList.end(),
                [](song* a, song* b){ return a->getDuration() < b->getDuration(); });
            break;
        default: break;
    }
}


void BrowsePlaylistScreen::render() 
{
    rebuildDisplayList();
    playlist* pl   = playlists[activePlaylistIndex];
    song*     cur  = player_.getCurrentSong();

    ui.clearScreen();
    ui.printTopBorder(pl->getName(), 56);
    ui.printSection({
        "Songs: " + to_string(pl->size()) + "   Sort: " + sortModeString()
        + (searchQuery.empty() ? "" : "   Search: \"" + searchQuery + "\"")
    });

    //song rows
    if (displayList.empty()) {
        ui.printSection({ "No songs match." });
    } else {
        ui.printSeparator();
        for (int i = 0; i < (int)displayList.size(); i++) {
            bool active = (cur && displayList[i] == cur);
            ui.printSongRow(i + 1, displayList[i], 56, active);
        }
    }

    ui.printSection({
        " [num] play   [s] sort   [f] filter   [/] search   /n [0] back"
    });
    ui.printBottomBorder();
    cout << " Choice: ";
}


void BrowsePlaylistScreen::handleInput() 
{
    char key = input.readKey();

    if (key == '0') {
        sortMode      = SORT_DEFAULT;
        searchQuery   = "";
        currentScreen = 2;

    } 
    else if (key == 's') 
    {
        ui.clearScreen();
        ui.printTopBorder("Sort By", 56);
        ui.printSection({ "1. Title", "2. Artist", "3. Album",
                          "4. Year",  "5. Duration", "0. Default" });
        ui.printBottomBorder();
        cout << " Sort choice: ";
        int choice = input.readInt(0, 5);
        sortMode = (SortMode)choice;

    } 
    else if (key == '/') 
    {
        ui.printSection({ "Search (empty to clear): " });
        searchQuery = input.readLine();

    } 
    else if (key == 'f') 
    {
        ui.printSection({ "Filter not yet implemented." });
        input.readKey();

    } 
    else if (isdigit(key) && key != '0') {
        int max   = (int)displayList.size();
        if (max == 0) return;
        int index = input.readInt(1, max) - 1;
        player_.setCurrentSong(displayList[index]);
        config.set("active_playlist", playlists[activePlaylistIndex]->getName());
        currentScreen = 1;
    }
}