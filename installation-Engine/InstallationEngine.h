#pragma once
#include <string>
#include <vector>
#include "installable.h"
#include "systemLog.h"
using namespace std;

class InstallationEngine{
private:
    vector<installable*> components;
    systemLogger logger;
    installable* getComponent(const string& id);
public:
    ~InstallationEngine();
    void processCommand();
    void handleAttach(string& line);
    void handleInstall(string& line);
    void handleResolve(string& line);
    void handleUnInstall(string& line);
};