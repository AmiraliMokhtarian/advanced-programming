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

void player::play(song* song)
{
    currentSong = song;
    state = PLAYING;
}

void player::pause()
{
    state = PAUSED;
}

void player::resume()
{
    state = PLAYING;
}

void player::stop()
{
    state = STOPPED;
}

void player::togglePause()
{
    if(state == PLAYING)
        pause();
    else if(state == PAUSED)
        resume();
}

void player::next()
{
}

void player::previous()
{
}

void player::setCurrentSong(song* s)
{
    currentSong = s;
}