#include <iostream>
#include "MusicLibrary.h"
#include "csvLoader.h"
#include "playlist.h"
#include "m3uLoader.h"

using namespace std;

int main(){
MusicLibrary library;

CsvLoader loader;
m3uLoader loader2;

loader.load(
    "../Data/library.csv",
    library
);

for(int i=0 ; i<int(library.getSongs().size()) ; i++){
    cout << library.getSongs()[i]->getTitle() << '\t' << library.getSongs()[i]->getDuration() << endl;
}

vector<playlist*> playlists =
    loader2.loadAll(
        "../Data/Playlists",
        library
    );

cout << playlists.size()
     << endl;

}