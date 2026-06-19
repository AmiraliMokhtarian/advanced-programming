#pragma once
#include "screen.h"
#include "player.h"
#include "playlist.h"
#include "configManager.h"
#include <vector>
#include <string>

using namespace std;

enum SortMode { SORT_DEFAULT, SORT_TITLE, SORT_ARTIST, SORT_ALBUM, SORT_YEAR, SORT_DURATION };

class BrowsePlaylistScreen : public Screen {
private:
    vector<playlist*>& playlists;
    player&            player_;
    configManager&     config;
    int&               currentScreen;
    int&               activePlaylistIndex;

    SortMode           sortMode;
    string             searchQuery;
    vector<song*>      displayList; //never touches original

    void rebuildDisplayList();
    string sortModeString() const;

public:
    BrowsePlaylistScreen(UIRender& ui, InputHandler& input,
                         vector<playlist*>& playlists, player& p,
                         configManager& config, int& currentScreen,
                         int& activePlaylistIndex);
    void render() override;
    void handleInput() override;
};