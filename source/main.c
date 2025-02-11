
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* custom types */
typedef enum E_MessageBoxResult
{
    MESSAGE_BOX_RESULT_NULL,
    MESSAGE_BOX_RESULT_CONFIRM,
    MESSAGE_BOX_RESULT_CANCEL,
} E_MessageBoxResult;

typedef struct S_AppState
{
    int screenWidth;
    int screenHeight;
    E_MessageBoxResult quitMessageBoxResult;
} S_AppState;

/* global variables */
static S_AppState G_AppState = {
    .screenWidth = 0,
    .screenHeight = 0,
    .quitMessageBoxResult = MESSAGE_BOX_RESULT_NULL,
};

static SDL_Window *G_Window = NULL;
static SDL_GPUDevice *G_GPUDevice = NULL; /* idk why I want to use GPU API instead of Renderer API, but I do */

/* startup callback */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* metadata */
    SDL_SetAppMetadata("Per", "0.1", "com.malgreen.Per");

    /* initialize SDL */
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* initialize window */
    Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    G_Window = SDL_CreateWindow("Per", 1280, 720, window_flags);
    if (G_Window == NULL)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowPosition(G_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    /* initialize GPU device */
    bool useDebugGPU = true;
    G_GPUDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_METALLIB, useDebugGPU, NULL);
    if (G_GPUDevice == NULL)
    {
        SDL_Log("Couldn't create GPU device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_ClaimWindowForGPUDevice(G_GPUDevice, G_Window))
    {
        SDL_Log("Couldn't claim window for GPU device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    // i think VSYNC present mode is better than SDL_GPU_PRESENTMODE_MAILBOX (for desktop app), because it limits FPS
    SDL_SetGPUSwapchainParameters(G_GPUDevice, G_Window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

    SDL_ShowWindow(G_Window);
    return SDL_APP_CONTINUE;
}

/* input event callback */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.scancode)
        {
        case SDL_SCANCODE_ESCAPE:
        { // show "quit" message box, could go in the SDL_EVENT_QUIT if statement
            const SDL_MessageBoxButtonData messageBoxButtons[] = {
                {.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
                 .buttonID = MESSAGE_BOX_RESULT_CONFIRM,
                 .text = "Yes"},
                {.flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
                 .buttonID = MESSAGE_BOX_RESULT_CANCEL,
                 .text = "No"},
            };
            SDL_MessageBoxData messageBoxData = {
                .title = "Quit",
                .message = "Are you sure you want to quit?",
                .flags = SDL_MESSAGEBOX_INFORMATION,
                .window = G_Window,
                .numbuttons = 2,
                .buttons = messageBoxButtons,
            };
            SDL_ShowMessageBox(&messageBoxData, (int *)&G_AppState.quitMessageBoxResult);
        }
        break;

        default:
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

/* frame/tick callback */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    /* act on state */
    SDL_GetWindowSize(G_Window, &G_AppState.screenWidth, &G_AppState.screenHeight);
    if (G_AppState.quitMessageBoxResult != MESSAGE_BOX_RESULT_NULL)
    {
        if (G_AppState.quitMessageBoxResult == MESSAGE_BOX_RESULT_CONFIRM) // "Yes" button
        {
            return SDL_APP_SUCCESS;
        }
        G_AppState.quitMessageBoxResult = MESSAGE_BOX_RESULT_NULL;
    }

    /* build UI */

    /* draw using GPU */
    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(G_GPUDevice);
    if (commandBuffer == NULL)
    {
        SDL_Log("Couldn't acquire GPU command buffer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GPUTexture *swapchainTexture;
    SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, G_Window, &swapchainTexture, NULL, NULL);
    if (swapchainTexture == NULL)
    {
        SDL_Log("Couldn't acquire GPU swapchain texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (swapchainTexture != NULL)
    {
        SDL_GPUColorTargetInfo targetInfo = {
            .texture = swapchainTexture,
            .clear_color = (SDL_FColor){0.3f, 0.4f, 0.5f, 1.0f},
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
            .mip_level = 0,
            .layer_or_depth_plane = 0,
            .cycle = false,
        };

        SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, NULL);
        SDL_EndGPURenderPass(renderPass);
    }
    SDL_SubmitGPUCommandBuffer(commandBuffer);

    return SDL_APP_CONTINUE;
}

/*  exit callback */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_WaitForGPUIdle(G_GPUDevice);

    SDL_ReleaseWindowFromGPUDevice(G_GPUDevice, G_Window);
    SDL_DestroyGPUDevice(G_GPUDevice);
    SDL_DestroyWindow(G_Window);
    SDL_Quit();
}
