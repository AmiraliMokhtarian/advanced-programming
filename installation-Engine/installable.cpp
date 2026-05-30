#include "installable.h"

using namespace std;

installable::installable(string _id_, string _title_)
    : id(_id_), title(_title_), state(componentState::PENDING),
      mockFail(false), installedParentCount(0), is_explicitly_installed(false) {}

    
string stateToString(componentState state)
{
    switch(state){
        case componentState::PENDING: return "PENDING";
        case componentState::INSTALLED: return "INSTALLED";
        case componentState::FAILED: return "FAILED";
        default: return "UNKNOWN";
    }
}


void installable::incParents()
{
    installedParentCount++;
}

void installable::decParents()
{
    if(installedParentCount > 0)
        installedParentCount--;
}

void installable::setState(componentState new_state)
{
    componentState old_state = state;
    state = new_state;

    for(observer* obs : observers){
        obs->onStateChanged(this, old_state, new_state);
    }
}

void installable::forcePending()
{
    setState(componentState::PENDING);
}

void installable::addObservers(observer *obs)
{
    observers.push_back(obs);
}