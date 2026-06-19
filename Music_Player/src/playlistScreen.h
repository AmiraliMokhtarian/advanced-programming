#pragma once
#include "screen.h"
#include "player.h"
#include "playlist.h"
#include <vector>

using namespace std;

class playlistScreen : public Screen{
private:
    vector<playlist*>& playlists;
    player& player_;
    int& currentScreen;
    int& activePlaylistIndex;  //browseScreen needs this
public:
    playlistScreen(UIRender& ui, InputHandler& input,
                   vector<playlist*>& playlists, player& p,
                   int& currentScreen, int& activePlaylistIndex);
    void render() override;
    void handleInput() override;
};