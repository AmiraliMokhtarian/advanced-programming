#include <fstream>
#include <string>
#include <filesystem>
#include "m3uLoader.h"
#include "playlist.h"
#include "MusicLibrary.h"
#include "playlist.h"

using namespace std;
namespace fs = std::filesystem;


vector<playlist*> m3uLoader::loadAll(
    const string& dir,
    MusicLibrary& library)
{
    vector<playlist*> playlists;

    for(const auto& entry :
        fs::directory_iterator(dir))
    {
        if(entry.path().extension() != ".m3u")
            continue;

        ifstream file(entry.path());

        if(!file.is_open())
            continue;

        string name =
            entry.path().stem().string();

        playlist* Playlist =
            new playlist(name);

        string line;

        while(getline(file, line))
        {
            if(line.empty())
                continue;

            song* currentSong =
                library.findByPath(line);

            if(currentSong)
                Playlist->addSong(currentSong);
        }

        playlists.push_back(Playlist);
    }

    return playlists;
}

