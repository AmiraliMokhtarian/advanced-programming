#include "player.h"

using namespace std;



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