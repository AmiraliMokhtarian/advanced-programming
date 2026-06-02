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

    if(isMockFail()){
        setState(componentState::FAILED);
        tx.stateChangedNodes.push_back(this);
        return false; 
    }

    int start_state = (int)tx.stateChangedNodes.size();
    int start_count = (int)tx.countIncreasedNodes.size();

    for(installable* child : children){
        if(child->getState() != componentState::INSTALLED){
            if(!child->install(tx)){
                // undo state changes since we started(LIFO)
                for(int i = (int)tx.stateChangedNodes.size() - 1; i >= start_state; i--){
                    if(tx.stateChangedNodes[i]->getState() == componentState::INSTALLED)
                        tx.stateChangedNodes[i]->setState(componentState::PENDING);
                }
                tx.stateChangedNodes.erase(tx.stateChangedNodes.begin() + start_state,
                                            tx.stateChangedNodes.end());

                // undo the dependency counts added since we started (LIFO ).
                for(int i = (int)tx.countIncreasedNodes.size() - 1; i >= start_count; i--){
                    tx.countIncreasedNodes[i]->decParents();
                }
                tx.countIncreasedNodes.erase(tx.countIncreasedNodes.begin() + start_count,
                                                tx.countIncreasedNodes.end());

                setState(componentState::FAILED);
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