#include "configManager.h"
#include <fstream>
#include <sstream>

using namespace std;

configManager::configManager(const string& path)
{
    this->path = path;
}

void configManager::set(const string& key, const string& value)
{
    setting[key] = value;
}

void configManager::load() {
    ifstream file(path);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t eq = line.find('=');
        if (eq == string::npos) continue;
        string key   = line.substr(0, eq);
        string value = line.substr(eq + 1);
        setting[key] = value;
    }
}

void configManager::save() {
    ofstream file(path);
    if (!file.is_open()) return;
    for (const auto& pair : setting)
        file << pair.first << "=" << pair.second << "\n";
}

string configManager::get(const string& key, const string& defaultValue) const
{
    auto it = setting.find(key);
    
    if(it == setting.end())
        return defaultValue;

    return it->second;
}