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

    //filter
    if (filterType != FilterType::None && !filterValue.empty()) {
        displayList.erase(
            remove_if(displayList.begin(), displayList.end(),
                [&](song* s) {
                    if (filterType == FilterType::Artist)
                        return s->getArtist() != filterValue;

                    if (filterType == FilterType::Album)
                        return s->getAlbum() != filterValue;

                    return false;
                }),
            displayList.end()
        );
    }

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
    if(!searchQuery.empty()){
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
        filterType    = FilterType::None;
        filterValue   = "";
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
        ui.clearScreen();
        ui.printTopBorder("Filter By", 56);
        ui.printSection({"1. Artist", "2. Album", "0. Clear Filter"});
        ui.printBottomBorder();

        int choice = input.readInt(0,2);

        if (choice == 0) {
            filterType = FilterType::None;
            filterValue = "";
            return;
        }

        playlist* pl = playlists[activePlaylistIndex];
        vector<string> values;

        for (song* s : pl->getSongs()) {

            string v = (choice == 1) ? s->getArtist() : s->getAlbum();
            if (find(values.begin(), values.end(), v) == values.end())
                values.push_back(v);
        }

        ui.clearScreen();
        ui.printTopBorder(choice==1 ? "Select Artist" : "Select Album",56);

        vector<string> rows;

        for (int i=0;i<values.size();i++)
            rows.push_back(to_string(i+1)+". "+values[i]);

        ui.printSection(rows);
        ui.printBottomBorder();

        int pick = input.readInt(1, values.size());

        filterValue = values[pick-1];
        filterType = (choice==1) ? FilterType::Artist : FilterType::Album;
    }
 
    else if (isdigit(key) && key != '0') {
        int max = (int)displayList.size();
        if (max == 0) return;
        int index = input.readInt(1, max) - 1;

        playlist* pl = playlists[activePlaylistIndex];
        auto& origSongs = pl->getSongs();
        auto it = find(origSongs.begin(), origSongs.end(), displayList[index]);

        if (it != origSongs.end()) {
            int realIndex = (int)(it - origSongs.begin());
            player_.loadPlaylist(pl, realIndex);
            config.set("active_playlist", pl->getName());
            currentScreen = 1;
        }
    }
}