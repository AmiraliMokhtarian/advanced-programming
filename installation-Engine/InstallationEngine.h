#pragma once
#include <string>
#include <vector>
#include "installable.h"

using namespace std;

class InstallationEngine{
private:
    vector<installable*> components;
    systemLogger logger;
    installable* getComponent(const string& id);
public:
    void processCommand();
    string getNextToken(string& id);
    void handleAttach(string& line);
    void handleInstall(string& line);
    void handleResolve(string& line);
    void handleUnInstall(string& line);
};