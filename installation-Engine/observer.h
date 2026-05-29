#pragma once
#include "componentState.h"

class installable;

class observer {
public:
    virtual void onStateChanged(const installable* comp,
        componentState oldState, componentState newState) = 0;
    virtual ~observer() {}
};