#pragma once
#include <vector>
#include <string>
#include "song.h"

using namespace std;

class MusicLibrary{
private:
    vector<song*> songs;
public:
    MusicLibrary();
    ~MusicLibrary();

    void addSong(song* song);
    vector<song*>& getSongs();
    song* findByPath(const string& path);
};