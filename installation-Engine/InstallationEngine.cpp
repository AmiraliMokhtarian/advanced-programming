#include <iostream>
#include <string>
#include <sstream>
#include "InstallationEngine.h"
#include "module.h"
#include "package.h"
#include "systemLog.h"


using namespace std;


void InstallationEngine::processCommand(){
    string line;
    while(getline(cin, line)){

        if(line == "END") break;
        if(line.empty()) continue;
        
        string cmd = getNextToken(line);

        if(cmd == "ADD"){
            string type = getNextToken(line);
            string id = getNextToken(line);
            string title = line;

            if(getComponent(id) != nullptr){
                cout << "ERROR: Component with ID " << id << " already exists";
                continue;
            }

            if(type == "MODULE"){
                installable* new_comp = new module(id, title);
                components.push_back(new_comp);
                new_comp->addObservers(&logger);
                
            }
            else if(type == "PACKAGE"){
                installable* new_comp = new package(id, title);
                components.push_back(new_comp);
                new_comp->addObservers(&logger);
            }
        } 
        
        else if(cmd == "ATTACH"){
            handleAttach(line);
        }

        else if(cmd == "INSTALL"){
            handleInstall(line);
        }

        else if(cmd == "MOCK_FAIL"){
            string id = getNextToken(line);

            if(id.empty()){
                cout << "ERROR: Invalid command" << endl; 
                continue;
            }

            installable* comp = getComponent(id);
            if(!comp){
                cout << "ERROR: Component " << id << " does not exist" << endl;
                continue;
            }

            if(comp->isMockFail()){
                cout << "ERROR: Component " << id << " is already set to fail" << endl;
                continue;
            }

            if(comp->getState() == componentState::INSTALLED){
                cout << "ERROR: Component " << id << " is already installed" << endl;
                continue;
            }

            comp->setMockFail(true);
        }

        else if(cmd == "RESOLVE_FAIL"){
            handleResolve(line);
        }

        else if(cmd == "UNINSTALL"){
            handleUnInstall(line);
        }

        else{
            cout << "ERROR: Invalid command" << endl;
        }
    }
}


installable* InstallationEngine::getComponent(const string& id){
    for(installable* comp : components){
        if(comp->getId() == id)
            return comp;
    }
    return nullptr;
}

string InstallationEngine::getNextToken(string& line) {
    size_t pos = line.find(' ');
    if (pos == string::npos) { 
        string temp = line;
        line = "";
        return temp;
    }
    string token = line.substr(0, pos);
    line.erase(0, pos + 1); 
    return token;
}


void InstallationEngine::handleAttach(string& line)
{
    string parent_id = getNextToken(line);
    string child_id = getNextToken(line);

    installable* child = getComponent(child_id);
    installable* parent = getComponent(parent_id);

    //priority 2
    if(parent == nullptr){
        cout << "ERROR: Component with ID " << parent_id << " does not exist" << endl;
        return;
    }
    if(child == nullptr){
        cout << "ERROR: Component with ID " << child_id << " does not exist" << endl;
        return;
    }

    //priority 3
    if(!parent->isPackage()){
        cout << "ERROR: Cannot attach to a module" << endl;
        return;
    }

    //priority 4
    for (installable* comp : components) {
        if (comp->isPackage()) {
            if (((package*)comp)->hasChild(child_id)) { 
                cout << "ERROR: Component " << child_id << " is already attached to " << comp->getId() << endl;
                return;
            }
        }
    }

    //priority 5
    if (parent->getState() == componentState::INSTALLED) {
        cout << "ERROR: Cannot attach to an already installed package" << endl;
        return;
    }

    ((package*)parent)->addChild(child);
}


void InstallationEngine::handleInstall(string& line)
{
    string id_installing = getNextToken(line);
    installable* installing_comp = getComponent(id_installing);
    if(!installing_comp){
        cout << "ERROR: Component " << id_installing << " does not exist" << endl;
        return;
    }

    if (installing_comp->getState() == componentState::INSTALLED) {
        cout << "ERROR: Component " << id_installing << " is already installed" << endl;
        return;
    }

    //if there was no error
    transactionContext tx;
    if (installing_comp->install(tx)){ 
        installing_comp->setExplicit(true);
    }
    
    else
    {
        //rollback
        for(int i = tx.stateChangedNodes.size()-1; i >= 0; --i){
            installable* node = tx.stateChangedNodes[i];

            if(node->getState() == componentState::INSTALLED)
                node->setState(componentState::PENDING);
        }

        //rollback dependency counts
        for(int i = tx.countIncreasedNodes.size()-1; i >= 0; --i){
            tx.countIncreasedNodes[i]->decParents();
        }

        //fail packages
        for(installable* p : tx.failedPackages){
            p->setState(componentState::FAILED);
        }
    }
}

void InstallationEngine::handleResolve(string &line)
{
    string id = getNextToken(line);

    installable* comp = getComponent(id);
    if(!comp){
        cout << "ERROR: Component " << id << " does not exist" << endl;
        return; 
    }

    if(!comp->isMockFail()){
        cout << "ERROR: Component " << id
            << " is not in a mock fail state" << endl;
        return;
    }

    comp->setMockFail(false);
}

void InstallationEngine::handleUnInstall(string &line)
{
    string id = getNextToken(line);

    if (id == "-A"){
        bool anyInstalled = false;

        for (auto comp : components){
            if (comp->getState() == componentState::INSTALLED){
                anyInstalled = true;
                break;
            }
        }

        if (!anyInstalled){
            cout << "ERROR: No installed components to uninstall" << endl;
            return;
        }

        for (int i = components.size() - 1; i >= 0; i--){
            if (components[i]->getState() == componentState::INSTALLED){
                    components[i]->unInstall();
            }
        }
        return;
    }

    installable* comp = getComponent(id);
    if (!comp) {
        cout << "ERROR: Component " << id << " does not exist" << endl;
        return;
    }
    if (comp->getState() != componentState::INSTALLED) {
        cout << "ERROR: Component " << id << " is not currently installed" << endl;
        return;
    }
    if (comp->getParentCount() > 0) {
        cout << "ERROR: Component " << id << " is required by another package" << endl;
        return;
    }

    comp->setExplicit(false);
    comp->unInstall();
}