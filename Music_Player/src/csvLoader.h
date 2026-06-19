#pragma once 
#include <string>

using namespace std;

class MusicLibrary;

class CsvLoader{
public:
    void load(const string& path, MusicLibrary& library);
};