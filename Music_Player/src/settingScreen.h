#pragma once
#include "screen.h"
#include "player.h"
#include "configManager.h"

using namespace std;

class settingScreen : public Screen {
private:
    player& player_;
    configManager& config;
    int& currentScreen;
public:
    settingScreen(UIRender& ui, InputHandler& input,
                   player& p, configManager& config, int& currentScreen);
    void render() override;
    void handleInput() override;
};