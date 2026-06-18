#pragma once
#include "song.h"

using namespace std;

enum playerState{PLAYING, STOPPED, PAUSED};

enum playbackMode{NO_REPEAT, REPEAT_ONE, REPEAT_ALL, SHUFFLE};

class player{
private:
    song* currentSong;
    playerState state;
    playbackMode mode;
public:
    player();
    ~player();
    song* getCurrentSong() const;
    playerState getState() const;
    playbackMode getMode() const;
    string getStateString() const;
};