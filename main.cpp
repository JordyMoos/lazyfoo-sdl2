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

    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    //Increase red
                    case SDLK_q:
                        r += 32;
                        break;

                        //Increase green
                    case SDLK_w:
                        g += 32;
                        break;

                        //Increase blue
                    case SDLK_e:
                        b += 32;
                        break;

                        //Decrease red
                    case SDLK_a:
                        r -= 32;
                        break;

                        //Decrease green
                    case SDLK_s:
                        g -= 32;
                        break;

                        //Decrease blue
                    case SDLK_d:
                        b -= 32;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        gModulatedTexture.setColor(r, g, b);
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
    //Load sprite sheet texture
    if (!gModulatedTexture.loadFromFile("assets/colors.png")) {
        printf("Failed to load colors.png texture!\n");
        return false;
    }

    return true;
}

void close() {
    gModulatedTexture.free();

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
