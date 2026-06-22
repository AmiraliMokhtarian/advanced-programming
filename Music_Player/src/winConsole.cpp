#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void enableWindowsConsole() {
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void platformSleep(int ms) 
{
#ifdef _WIN32
    Sleep(ms);
#endif
}
#else
void enableWindowsConsole() {}
#endif