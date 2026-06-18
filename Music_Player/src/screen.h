#pragma once
#include "UIRender.h"
#include "inputHandler.h"

class Screen {
protected:
    UIRender& ui;
    InputHandler& input;
public:
    Screen(UIRender& ui, InputHandler& input) : ui(ui), input(input) {}
    virtual ~Screen() = default;
    virtual void render() = 0;
    virtual void handleInput() = 0;
};