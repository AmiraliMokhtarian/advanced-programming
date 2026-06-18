#pragma once
#include "screen.h"
#include "player.h"
#include "configManager.h"
using namespace std;

class MainMenuScreen : public Screen {
private:
    player& player_;
    configManager& config;
    bool& running;
    int& currentScreen;
public:
    MainMenuScreen(UIRender& ui, InputHandler& input,
                   player& p, configManager& config,
                   bool& running, int& currentScreen);
    void render() override;
    void handleInput() override;
};