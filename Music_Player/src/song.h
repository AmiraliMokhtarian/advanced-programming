#pragma once
#include <iostream>
using namespace std;

class song{
private:
    string title;
    string artist;
    string album;
    string genre;
    int year;
    int duration;
    string path;
public:
    song();

    song(
        string _title_,
        string _artist_,
        string _album_,
        string _genre_,
        int _year_,
        int _duration_,
        string _path_  
    );

    string getTitle() const;
    string getArtist() const;
    string getAlbum() const;
    string getGenre() const;
    int getYear() const;
    int getDuration() const;
    string getFilePath() const;
};