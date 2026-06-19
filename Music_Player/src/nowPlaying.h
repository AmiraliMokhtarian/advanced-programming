#pragma once
#include "screen.h"
#include "player.h"
#include "configManager.h"
using namespace std;

class NowPlayingScreen : public Screen {
private:
    player& player_;
    int& currentScreen;
    configManager& config;
public:
    NowPlayingScreen(UIRender& ui, InputHandler& input,
                     player& p, int& currentScreen, configManager& config);
    void render() override;
    void handleInput() override;
};