#include "player.h"

using namespace std;

player::player()
    : currentSong(nullptr),
      state(STOPPED),
      mode(NO_REPEAT)
{}

player::~player()
{}

song* player::getCurrentSong() const
{
    return currentSong;
}

playerState player::getState() const
{
    return state;
}

playbackMode player::getMode() const
{
    return mode;
}

string player::getStateString() const 
{
    switch (state) {
        case PLAYING: return "PLAYING";
        case PAUSED:  return "PAUSED";
        case STOPPED: return "STOPPED";
    }
    return "";
}


string player::getModeString() const 
{
    switch (mode) {
        case NO_REPEAT:  return "NORMAL";
        case REPEAT_ONE: return "REPEAT_ONE";
        case REPEAT_ALL: return "REPEAT_ALL";
        case SHUFFLE:    return "SHUFFLE";
    }
    return "";
}