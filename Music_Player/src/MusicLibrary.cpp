#include "MusicLibrary.h"

using namespace std;

MusicLibrary::MusicLibrary() {};

MusicLibrary::~MusicLibrary() 
{
    for(song* song : songs){
        delete song;
    }
}

void MusicLibrary::addSong(song* song)
{
    songs.push_back(song);
}

vector<song *> &MusicLibrary::getSongs()
{
    return songs;
}

song *MusicLibrary::findByPath(const string &path)
{
    for(song* s : songs){
        if(s->getFilePath() == path){
            return s;
        }
    }
    return nullptr;
}