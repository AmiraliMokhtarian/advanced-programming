#pragma once
#include <string>

using namespace std;

enum class componentState{PENDING , INSTALLED , FAILED};

string stateToString(componentState state);