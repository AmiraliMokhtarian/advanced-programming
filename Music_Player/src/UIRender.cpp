#include <iostream>
#include <iomanip>
#include <sstream>
#include "UIRender.h"

using namespace std;

const string UIRender::RESET  = "\033[0m";
const string UIRender::BOLD   = "\033[1m";
const string UIRender::CYAN   = "\033[36m";
const string UIRender::GREEN  = "\033[32m";
const string UIRender::YELLOW = "\033[33m";
const string UIRender::WHITE  = "\033[37m";

//helper function for printing =
static string repeatUtf8(const string& ch, int count) {
    string result;
    for (int i = 0; i < count; i++) result += ch;
    return result;
}

void UIRender::clearScreen()
{
    cout << "\033[2J\033[1;1H" << flush;
}

void UIRender::printTopBorder(const string &title, int width)
{
    cout << "╔" << repeatUtf8("═", width - 2) << "╗" << endl;

    int visibleLen = 0;
    bool inEscape = false;
    for (size_t i = 0; i < title.size(); i++) {
        unsigned char c = title[i];
        if (c == '\033') { inEscape = true; continue; }
        if (inEscape)    { if (c == 'm') inEscape = false; continue; }
        if ((c & 0xC0) == 0x80) continue;
        visibleLen++;
    }

    int pad  = (width - 2 - visibleLen) / 2;
    int rpad = width - 2 - pad - visibleLen;
    cout << "║" << string(pad, ' ') << title << string(rpad, ' ') << "║" << endl;
}

void UIRender::printSection(const vector<string> &rows, int width)
{
    cout << "╠" << repeatUtf8("═", width - 2) << "╣" << endl;
    for (const string& text : rows) {
        string row = " " + text;

        int visibleLen = 0;
        bool inEscape   = false;
        for (size_t i = 0; i < row.size(); i++) {
            unsigned char c = row[i];

            if (c == '\033') { inEscape = true; continue; }
            if (inEscape)    { if (c == 'm') inEscape = false; continue; }

            // skip UTF-8 continuation bytes (10xxxxxx) — they're part of
            // the previous character, not new visible characters
            if ((c & 0xC0) == 0x80) continue;

            visibleLen++;
        }

        int rpad = width - 2 - visibleLen;
        if (rpad < 0) rpad = 0;
        cout << "║" << row << string(rpad, ' ') << "║" << endl;
    }
}

void UIRender::printSeparator(int width) 
{
    cout << "╠" << repeatUtf8("═", width - 2) << "╣\n";
}

void UIRender::printBottomBorder(int width)
{
    cout << "╚" << repeatUtf8("═", width - 2) << "╝" << endl;
}

void UIRender::printSongRow(int index, const song* s, int width, bool active) 
{
    string marker = active ? " ▶" : "  ";
    string title  = s->getTitle();
    string artist = s->getArtist();
    string dur    = formatDuration(s->getDuration());

    if ((int)title.size()  > 22) title  = title.substr(0, 19)  + "...";
    if ((int)artist.size() > 14) artist = artist.substr(0, 11) + "...";

    ostringstream oss;
    oss << left
        << setw(3)  << index
        << marker
        << setw(24) << title
        << setw(16) << artist
        << right    << dur;

    string row = " " + oss.str();

    int visLen = 0;
    for (unsigned char c : row) {
        if ((c & 0xC0) == 0x80) continue;
        visLen++;
    }

    int rpad = width - 2 - visLen;
    if (rpad < 0) rpad = 0;

    cout << "║" << row << string(rpad, ' ') << "║" << endl;
}

string UIRender::formatDuration(int seconds)
{
    ostringstream oss;
    oss << setfill('0')
        << setw(2) << seconds / 60 << ":"
        << setw(2) << seconds % 60;
    return oss.str();
}