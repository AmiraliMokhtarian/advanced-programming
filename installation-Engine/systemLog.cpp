#include <iostream>
#include "systemLog.h"
#include "installable.h"

using namespace std;

void systemLogger::onStateChanged(const installable* comp, componentState oldState, componentState newState)
{
    cout << "[OBSERVER] Component " << comp->getId()
         << " changed from " << stateToString(oldState)
         << " to " << stateToString(newState) << endl;
}