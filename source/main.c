
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

typedef struct APP_STATE
{
    int screenWidth;
    int screenHeight;

} APP_STATE;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------

    // State
    APP_STATE appState = {
        .screenWidth = 800,
        .screenHeight = 450,
    };
    APP_STATE prevAppState; // TODO: should we allocate memory?

    // Window
    InitWindow(appState.screenWidth, appState.screenHeight, "Per");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Refresh rate
    int monitorRefreshRate = GetMonitorRefreshRate(GetCurrentMonitor()); // TODO: Change when changing monitor
    SetTargetFPS(monitorRefreshRate);

    //--------------------------------------------------------------------------------------

    // Main Loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // UPDATE
        //----------------------------------------------------------------------------------
        appState.screenWidth = GetScreenWidth();
        appState.screenHeight = GetScreenHeight();

        //----------------------------------------------------------------------------------

        // RENDER
        //----------------------------------------------------------------------------------
        // TODO: retained-mode experimentation, just freezes the program right now?
        if (&prevAppState != NULL && (&appState, &prevAppState, sizeof(appState)) == 0)
        {
            continue;
        }
        prevAppState = appState; // copies memory

        BeginDrawing();

        ClearBackground(WHITE);

        DrawText("text goes here", appState.screenWidth / 2, appState.screenHeight / 2, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
