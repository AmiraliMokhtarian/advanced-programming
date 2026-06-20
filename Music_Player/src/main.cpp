#include "Application.h"
#include "winConsole.h"

int main() {
    enableWindowsConsole();
    Application app;
    app.run();
    return 0;
}