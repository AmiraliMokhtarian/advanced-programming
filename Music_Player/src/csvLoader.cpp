#include "csvLoader.h"
#include "MusicLibrary.h"
#include "song.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

void CsvLoader::load(const string& path, MusicLibrary& library)
{
    ifstream file(path);

    if (!file.is_open())
    {
        throw runtime_error(
            "Cannot open csv file"
        );
    }

    string line;
    while(getline(file, line)){
        stringstream ss(line);
        string title, artist, album, genre, year, duration, file_path;

        getline(ss, title, ',');
        getline(ss, artist, ',');
        getline(ss, album, ',');
        getline(ss, genre, ',');
        getline(ss, year, ',');
        getline(ss, duration, ',');
        getline(ss, file_path);

        song* s = new song(
            title,
            artist,
            album,
            genre,
            stoi(year),
            stoi(duration),
            file_path
        );

        library.addSong(s);
    }
}
