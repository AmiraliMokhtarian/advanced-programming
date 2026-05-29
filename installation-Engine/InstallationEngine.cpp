#include <iostream>
#include <string>
#include <sstream>
#include "InstallationEngine.h"
#include "module.h"
#include "package.h"


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
            string title = getNextToken(line);

            if(getComponent(id) != nullptr){
                cout << "ERROR: Component with ID " << id << " already exists";
                continue;
            }

            if(type == "MODULE"){
                components.push_back(new module(id, title));
            }
            else if(type == "PACKAGE"){
                components.push_back(new package(id, title));
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
    string TO_word = getNextToken(line);
    string child_id = getNextToken(line);

    //priority 1
    if (child_id.empty() || parent_id.empty()) {
        cout << "ERROR: Invalid command" << endl;
        return;
    }

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
    if(id_installing.empty()){
        cout << "ERROR: Invalid command" << endl;
        return;
    }

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
    
    else{
        for (installable* node : tx.stateChangedNodes){
            if(node->getState() == componentState::INSTALLED) //just pend the installed nodes
                node->setState(componentState::PENDING); 
        }
        for (installable* node : tx.countIncreasedNodes){
            node->decParents();
        }
    }
}

void InstallationEngine::handleResolve(string &line)
{
    string id = getNextToken(line);

    if(id.empty()){
        cout << "ERROR: Invalid command" << endl;
        return;
    }

    installable* comp = getComponent(id);
    if(!comp){
        cout << "ERROR: Component " << id << " is already installed" << endl;
        return; 
    }

    if(comp->getState() != componentState::FAILED){
        cout << "ERROR: Component " << id << " is not in a mock fail state" << endl;
        return;
    }

    comp->setMockFail(false);
    comp->setState(componentState::PENDING);
}