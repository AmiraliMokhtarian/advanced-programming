#include "playlist.h"

playlist::playlist(const string& name)
{
    this->name = name;
}

void playlist::addSong(song* song)
{
    songs.push_back(song);
}

string playlist::getName() const
{
    return name;
}

vector<song*>& playlist::getSongs()
{
    return songs;
}

int playlist::size() const
{
    return songs.size();
}