#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int mainWrapper();

bool init();

bool loadMedia();

void close();

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
Texture gModulatedTexture;
Texture gBackgroundTexture;

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

    Uint8 a = 255;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                //Increase alpha on w
                if (e.key.keysym.sym == SDLK_w) {
                    if (a + 32 > 255) {
                        a = 255;
                    }
                    else {
                        a += 32;
                    }
                }
                //Decrease alpha on s
                else if (e.key.keysym.sym == SDLK_s) {
                    if (a - 32 < 0) {
                        a = 0;
                    }
                    else {
                        a -= 32;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        gBackgroundTexture.render(0, 0);

        gModulatedTexture.setAlpha(a);
        gModulatedTexture.render(0, 0);

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

    return true;
}

bool loadMedia() {
    if (!gModulatedTexture.loadFromFile("assets/fadeout.png")) {
        printf("Failed to load fadeout.png texture!\n");
        return false;
    }
    gModulatedTexture.setBlendMode(SDL_BLENDMODE_BLEND);

    if (!gBackgroundTexture.loadFromFile("assets/fadein.png")) {
        printf("Failed to load fadein.png texture!\n");
        return false;
    }

    return true;
}

void close() {
    gModulatedTexture.free();
    gBackgroundTexture.free();

    if (gRenderer != nullptr) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }

    if (gWindow != nullptr) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}
