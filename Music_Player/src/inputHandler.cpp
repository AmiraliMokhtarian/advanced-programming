#include "inputHandler.h"
#include <iostream>
#include <limits>
#include <conio.h>

using namespace std;

string InputHandler::readLine() 
{
    string line;
    getline(cin, line);
    return line;
}

int InputHandler::readInt(int min, int max) 
{
    int value;
    while (true) {
        if (cin >> value && value >= min && value <= max) {
            //remove leftover characters from input -> like: 2aaa = 2
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "The number shoud be between " << min << "and " << max;
    }
}

char InputHandler::readKey() 
{
    char c;
    cin.get(c);
    return c;
}

bool InputHandler::keyAvailable()
{
    return _kbhit() != 0;
}