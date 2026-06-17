#pragma once
#include <string>

using namespace std;

class InputHandler {
public:
    string readLine();
    
    int readInt(int min, int max); //checking validation
    
    char readKey();
};