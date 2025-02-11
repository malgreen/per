
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_render.h"

typedef struct APP_STATE
{
    int screenWidth;
    int screenHeight;

} APP_STATE;

static APP_STATE appState = {
    .screenWidth = 0,
    .screenHeight = 0,
};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* startup callback */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // Metadata
    SDL_SetAppMetadata("Per", "0.1", "com.malgreen.per");

    // SDL initialization
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Window initialization
    Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("Per", 1280, 720, window_flags);
    if (window == NULL)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Renderer initialization
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL)
    {
        SDL_Log("Couldn't create renderer: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }
    SDL_SetRenderVSync(renderer, true);

    SDL_ShowWindow(window);
    return SDL_APP_CONTINUE;
}

/* input event callback */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

/* frame/tick callback */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // act
    SDL_GetWindowSize(window, &appState.screenWidth, &appState.screenHeight);

    // render
    Uint8 r;

    r = (Uint8)((((float)(SDL_GetTicks() % 3000)) / 3000.0f) * 255.0f);
    SDL_SetRenderDrawColor(renderer, r, 0, 0, 255);
    SDL_RenderClear(renderer); /* clear whole window to that fade color. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/*  exit callback */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
