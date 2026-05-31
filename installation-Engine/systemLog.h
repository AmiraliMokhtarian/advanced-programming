#pragma once
#include "observer.h"

class systemLogger : public observer{
public:
    void onStateChanged(const installable* comp,
        componentState oldState, componentState newState) override;
};