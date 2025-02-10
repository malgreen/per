
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

typedef struct APP_STATE
{
    int screenWidth;
    int screenHeight;

} APP_STATE;

static APP_STATE appState = {
    .screenWidth = 0,
    .screenHeight = 0,
};
static APP_STATE prevAppState;

static SDL_Window *window = NULL;
static SDL_GLContext open_gl_context = NULL;

// startup callback
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

    // OpenGL renderer and window initialization
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("Per", 1280, 720, window_flags);
    if (window == NULL)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Context initialization
    open_gl_context = SDL_GL_CreateContext(window);
    if (open_gl_context == NULL)
    {
        SDL_Log("Couldn't create context: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_MakeCurrent(window, open_gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_ShowWindow(window);

    // Application state initialization
    prevAppState = appState;

    return SDL_APP_CONTINUE;
}

// input event callback
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// frame/tick callback
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // act
    SDL_GetWindowSize(window, &appState.screenWidth, &appState.screenHeight);

    // render
    // weird retained experimentation
    if (memcmp(&appState, &prevAppState, sizeof(appState)) == 0)
    {
        // nothing changed
        return SDL_APP_CONTINUE;
    }
    prevAppState = appState;
    SDL_Log("Height: %d", appState.screenHeight);
    SDL_Log("Width: %d", appState.screenWidth);

    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

// exit callback
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
