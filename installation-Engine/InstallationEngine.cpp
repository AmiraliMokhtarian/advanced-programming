#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include "InstallationEngine.h"
#include "module.h"
#include "package.h"
#include "systemLog.h"


using namespace std;


string trimLeading(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return ""; 
    return str.substr(first);
}

string getNextToken(string& str) {
    str = trimLeading(str);
    if (str.empty()) return "";
    
    size_t space_pos = str.find_first_of(" \t\r\n");
    if (space_pos == string::npos) { 
        string token = str;
        str = "";
        return token;
    }
    
    string token = str.substr(0, space_pos);
    str = str.substr(space_pos);
    return token;
}

InstallationEngine::~InstallationEngine()
{
    for (installable* comp : components) {
        delete comp; 
    }
}


void InstallationEngine::processCommand(){
    string line;
    while(getline(cin, line)){
        if (!line.empty() && line.back() == '\n') line.pop_back();
        if (!line.empty() && line.back() == '\r') line.pop_back();
        
        if(line == "END") break;
        if(line.empty()) continue;
        
        string cmd = getNextToken(line);

        if(cmd == "ADD"){
            string type = getNextToken(line);
            string id = getNextToken(line);
            string title = trimLeading(line);

            if(getComponent(id) != nullptr){
                cout << "ERROR: Component with ID " << id << " already exists" << endl;
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
            else{
                cout << "ERROR: Invalid command" << endl;
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


void InstallationEngine::handleAttach(string& line)
{
    string parent_id = getNextToken(line);
    string child_id = getNextToken(line);

    installable* child = getComponent(child_id);
    installable* parent = getComponent(parent_id);

    //priority 2
    if(parent == nullptr){
        cout << "ERROR: Component " << parent_id << " does not exist" << endl;
        return;
    }
    if(child == nullptr){
        cout << "ERROR: Component " << child_id << " does not exist" << endl;
        return;
    }

    //priority 3
    if(!parent->isPackage()){
        cout << "ERROR: Cannot attach to a module" << endl;
        return;
    }

    //priority 4
    package* pkg = (package*)parent;
    if (pkg->hasChild(child_id)) { 
        cout << "ERROR: Component " << child_id << " is already attached to " << parent_id << endl;
        return;
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
    
    //rollback is done in package::install --> there's no "else"
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
        bool any_active = false;

        for (auto comp : components){
            if (comp->getState() == componentState::INSTALLED 
                || comp->getState() == componentState::FAILED){
                any_active = true;
                break;
            }
        }

        if (!any_active){
            cout << "ERROR: No installed components to uninstall" << endl;
            return;
        }

        for (int i = components.size() - 1; i >= 0; i--){
            componentState s = components[i]->getState();
            if (s == componentState::INSTALLED || s == componentState::FAILED){
                components[i]->forcePending();

                while (components[i]->getParentCount() > 0)
                    components[i]->decParents();

                components[i]->setExplicit(false);
                components[i]->setMockFail(false);
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