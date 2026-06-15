#pragma once
#include <string>
#include <vector>
#include "song.h"

using namespace std;

class playlist{
private:
    string name;
    vector<song*> songs;
public:
    playlist(const string& name);
    void addSong(song* song);
    string getName() const;
    vector<song*>& getSongs();
    int size() const;
};