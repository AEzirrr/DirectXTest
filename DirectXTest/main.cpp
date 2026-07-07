#include <iostream>
#include <Windows.h> // Required for WinMain
#include "AppWindow.h"

// Your existing main
int main() {
    AppWindow app;
    if (app.init()) {
        while (app.isRunning()) {
            app.broadcast();
        }
    }
    return 0;
}

// The entry point the Windows SubSystem is looking for
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(); // Route it back to your standard main
}