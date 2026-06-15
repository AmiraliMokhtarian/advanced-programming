#include "song.h"

using namespace std;

song::song() {}

song::song(
    string _title_,
    string _artist_,
    string _album_,
    string _genre_,
    int _year_,
    int _duration_,
    string _path_  
)
    : title(_title_),
      artist(_artist_),
      album(_album_),
      genre(_genre_),
      year(_year_),
      duration(_duration_),
      path(_path_)
{}

string song::getTitle() const {return title;}
string song::getArtist() const {return artist;}
string song::getAlbum() const {return album;}
string song::getGenre() const {return genre;}
int song::getYear() const {return year;}
int song::getDuration() const {return duration;}
string song::getFilePath() const {return path;}