#include <cstdio>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Timer.h"
#include "Globals.h"

int mainWrapper();

bool init();

bool loadMedia();

void close();

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
TTF_Font *gFont = nullptr;

Texture gPromptTextTexture;
Texture gTimeTextTexture;

Texture gButtonSpriteSheetTexture;
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

int main(int argc, char *args[]) {
    int response = mainWrapper();
    close();

    return response;
}

int mainWrapper() {
    if (!init()) {
        printf("Failed to initialize!\n");
        return -1;
    }

    if (!loadMedia()) {
        printf("Failed to load media!\n");
        return -2;
    }

    bool quit = false;
    SDL_Event e;
    SDL_Color textColor = {0, 0, 0, 255};
    Timer timer;
    std::stringstream timeText;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                //Start/stop
                if (e.key.keysym.sym == SDLK_s) {
                    if (timer.isStarted()) {
                        timer.stop();
                    } else {
                        timer.start();
                    }
                }
                //Pause/unpause
                else if (e.key.keysym.sym == SDLK_p) {
                    if (timer.isPaused()) {
                        timer.resume();
                    } else {
                        timer.pause();
                    }
                }
            }
        }


        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        timeText.str("");
        timeText << "Seconds since start time " << ( timer.getTicks() / 1000.f ) ;
        if (!gTimeTextTexture.loadFromRenderedText(timeText.str(), textColor)) {
            printf("Unable to render time texture!\n");
            return -3;
        }

        gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
        gTimeTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2,
                                (SCREEN_HEIGHT - gPromptTextTexture.getHeight()) / 2);

        SDL_RenderPresent(gRenderer);
        SDL_Delay(16);
    }

    return 0;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow(
            "SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

bool loadMedia() {
    gFont = TTF_OpenFont("assets/lazy.ttf", 28);
    if (gFont == nullptr) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    SDL_Color textColor = {0, 0, 0, 255};
    if (!gPromptTextTexture.loadFromRenderedText("Please Enter to Reset Start Time.", textColor)) {
        printf("Failed to render text texture!\n");
        return false;
    }

    return true;
}

void close() {
    gPromptTextTexture.free();
    gTimeTextTexture.free();

    TTF_CloseFont(gFont);
    gFont = nullptr;

    if (gRenderer != nullptr) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }

    if (gWindow != nullptr) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
