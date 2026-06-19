#pragma once
#include <vector>
#include <string>
#include "song.h"

using namespace std;

class UIRender{
public:
    static const string RESET;
    static const string BOLD;
    static const string CYAN;
    static const string GREEN;
    static const string YELLOW;
    static const string WHITE;
public:
    void clearScreen();
    void printTopBorder(const string& title="", int width = 56);
    void printSection(const vector<string>& rows, int width = 56);
    void printSeparator(int width = 56);
    void printBottomBorder(int width = 56);
    void printSongRow(int index, const song* s, int width = 56, bool active = false);
    string formatDuration(int seconds);
};