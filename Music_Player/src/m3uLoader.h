#pragma once
#include <string>
#include "MusicLibrary.h"
#include "playlist.h"

using namespace std;

class m3uLoader{
public:
    vector<playlist*> loadAll(const string& dir, MusicLibrary& library);
};