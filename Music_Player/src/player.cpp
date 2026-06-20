#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "player.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

using namespace std;

player::player()
    : soundLoaded_(false),
      currentSong(nullptr),
      currentPlaylist(nullptr),
      currentIndex(0),
      state(STOPPED),
      mode(NO_REPEAT)
{
    if (ma_engine_init(NULL, &engine_) != MA_SUCCESS)
        throw runtime_error("Failed to initialize audio engine");

    srand((unsigned)time(nullptr));
}


player::~player()
{
    if (soundLoaded_) ma_sound_uninit(&sound_);
    ma_engine_uninit(&engine_);
}


void player::loadAndPlay(int index)
{
    if (!currentPlaylist) return;

    if (soundLoaded_){
        ma_sound_uninit(&sound_);
        soundLoaded_ = false;
    }

    currentIndex = index;
    currentSong  = currentPlaylist->getSongs()[index];

    ma_result r = ma_sound_init_from_file(
        &engine_, currentSong->getFilePath().c_str(),
        0, NULL, NULL, &sound_
    );
    if (r != MA_SUCCESS) {
        state = STOPPED; return;
    }

    soundLoaded_ = true;
    ma_sound_start(&sound_);
    state = PLAYING;
}

void player::play()
{
    if (!currentSong || !currentPlaylist) return;
    loadAndPlay(currentIndex);
}

void player::pause()
{
    if (!soundLoaded_) return;
    ma_sound_stop(&sound_); //position preserved
    state = PAUSED;
}

void player::resume()
{
    if (!soundLoaded_) return;
    ma_sound_start(&sound_);
    state = PLAYING;
}

void player::stop()
{
    if (!soundLoaded_) return;
    ma_sound_stop(&sound_);
    ma_sound_seek_to_pcm_frame(&sound_, 0); //reset to start
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
    if (!currentPlaylist) return;
    int count = (int)currentPlaylist->getSongs().size();
    if (count == 0) return;

    switch (mode) {
        case NO_REPEAT:
            if (currentIndex + 1 >= count) { stop(); return; }
            loadAndPlay(currentIndex + 1);
            break;

        case REPEAT_ONE:
            loadAndPlay(currentIndex);
            break;

        case REPEAT_ALL:
            loadAndPlay((currentIndex + 1) % count);  //wrap to start
            break;

        case SHUFFLE: {
            if (count == 1) { loadAndPlay(0); break; }
            int newIndex;
            do {
                newIndex = rand() % count;
            } while (newIndex == currentIndex);  // no immediate repeat
            loadAndPlay(newIndex);
            break;
        }
    }
}

void player::previous()
{
    if (!currentPlaylist) return;
    int count = (int)currentPlaylist->getSongs().size();
    if (count == 0) return;
    int prevIndex = (currentIndex - 1 + count) % count;
    loadAndPlay(prevIndex);
}

void player::loadPlaylist(playlist* pl, int startIndex)
{
    currentPlaylist = pl;
    loadAndPlay(startIndex);
}

void player::seekBy(int seconds)
{
    if (!soundLoaded_) return;
    ma_uint64 cursor, length;
    ma_sound_get_cursor_in_pcm_frames(&sound_, &cursor);
    ma_sound_get_length_in_pcm_frames(&sound_, &length);
    ma_uint32 rate = ma_engine_get_sample_rate(&engine_);

    ma_int64 newFrame = (ma_int64)cursor + (ma_int64)seconds * rate;
    if (newFrame < 0) newFrame = 0;
    if ((ma_uint64)newFrame >= length) { next(); return; }
    ma_sound_seek_to_pcm_frame(&sound_, (ma_uint64)newFrame);
}

void player::tick()
{
    if (state != PLAYING || !soundLoaded_) return;
    if (ma_sound_at_end(&sound_)) next();
}

void player::setCurrentSong(song* s) { currentSong = s; }
void player::setMode(playbackMode m) { mode = m; }

song* player::getCurrentSong() const { return currentSong; }
playerState player::getState() const { return state; }
playbackMode player::getMode() const { return mode; }

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

float player::getCursorSec() const
{
    if (!soundLoaded_) return 0.0f;
    ma_uint64 frames = 0;
    ma_sound_get_cursor_in_pcm_frames(const_cast<ma_sound*>(&sound_), &frames);
    return (float)frames / (float)ma_engine_get_sample_rate(const_cast<ma_engine*>(&engine_));
}