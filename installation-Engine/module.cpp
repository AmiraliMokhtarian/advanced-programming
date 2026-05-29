#include "module.h"

using namespace std;

module::module(string id, string title)
    : installable(id, title){}

    
bool module::install(transactionContext &tx)
{
    if(state == componentState::INSTALLED)
        return true;

    if(isMockFail){
        setState(componentState::FAILED);
        tx.stateChangedNodes.push_back(this);
        return false;
    }

    setState(componentState::INSTALLED);
    tx.stateChangedNodes.push_back(this);

    return true;
}