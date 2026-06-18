#pragma once
#include <string>
#include <map>

using namespace std;

class configManager{
private:
    string path;
    map <string,string> setting;

public:
    configManager(const string& path);
    void set(const string& key, const string& value);
    void load();
    void save();
    string get(const string& key, const string& defaultValue = "") const;
};
