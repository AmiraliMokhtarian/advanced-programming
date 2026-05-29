#include "package.h"

using namespace std;

package::package(string id, string title)
    : installable(id, title){}

void package::addChild(installable* child)
{
    children.push_back(child);
}

bool package::hasChild(const string &child_id) const
{   
    for(installable* child : children){
        if(child != nullptr && child->getId() == child_id)
            return true;
    }
    return false;
}

bool package::install(transactionContext &tx)
{
    if(state == componentState::INSTALLED)
        return true;

    if(isMockFail){
        setState(componentState::FAILED);
        tx.stateChangedNodes.push_back(this);
        return false; 
    }

    for(installable* child : children){
        if(child->getState() != componentState::INSTALLED){
            if(!child->install(tx)){
                return false;
            }
        }
        child->incParents();
        tx.countIncreasedNodes.push_back(child);
    }

    setState(componentState::INSTALLED);
    tx.stateChangedNodes.push_back(this);

    return true;
}

void package::unInstall()
{
    forcePending();
    for(int i = children.size()-1 ; i >= 0 ; i--){
        children[i]->decParents();

        if(children[i]->getState() == componentState::INSTALLED &&
            children[i]->getParentCount() == 0 && !children[i]->getExplicit()){
                children[i]->unInstall();
            }
    }
}