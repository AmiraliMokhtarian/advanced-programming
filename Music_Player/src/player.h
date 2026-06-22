#pragma once
#include "song.h"
#include "playlist.h"
#include "miniaudio.h"
#include <string>

using namespace std;

enum playerState    {PLAYING, STOPPED, PAUSED};
enum playbackMode   {NO_REPEAT, REPEAT_ONE, REPEAT_ALL, SHUFFLE};

class player{
private:
    ma_engine engine_;
    ma_sound sound_;
    bool soundLoaded_;
    song* currentSong;
    playlist* currentPlaylist;
    int currentIndex;
    playerState state;
    playbackMode mode;

    void loadAndPlay(int index);

public:
    player();
    ~player();
    song* getCurrentSong() const;
    playerState getState() const;
    playbackMode getMode() const;
    string getStateString() const;
    string getModeString() const;
    float getCursorSec() const;

    void setMode(playbackMode m);

    void play();
    void pause();
    void resume();
    void stop();
    void next();
    void previous();
    void togglePause();

    void loadPlaylist(playlist* pl, int startIndex);
    void seekBy(int seconds);
    void tick();
};