#include "configManager.h"

using namespace std;

configManager::configManager(const string& path)
{
    this->path = path;
}

void configManager::set(const string& key, const string& value)
{
    setting[key] = value;
}

string configManager::get(const string& key) const
{
    auto it = setting.find(key);
    
    if(it == setting.end())
        return "";

    return it->second;
}