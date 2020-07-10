#include <stdio.h>
#include <SDL.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

int mainWrapper();

bool init();

bool loadMedia();

void close();

SDL_Surface* loadSurface(const std::string &path);

SDL_Window *gWindow = nullptr;
SDL_Surface *gScreenSurface = nullptr;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = nullptr;


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
    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                        break;
                    case SDLK_DOWN:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                        break;
                    case SDLK_LEFT:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                        break;
                    case SDLK_RIGHT:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                        break;
                    default:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                        break;
                }
            }
        }

        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;

        SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);
        SDL_UpdateWindowSurface(gWindow);

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

    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return true;
}

bool loadMedia() {
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("assets/stretch.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == nullptr) {
        printf("Failed to load stretch image!\n");
        return false;
    }

    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("assets/up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == nullptr) {
        printf("Failed to load up image!\n");
        return false;
    }

    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("assets/down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == nullptr) {
        printf("Failed to load down image!\n");
        return false;
    }

    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("assets/left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == nullptr) {
        printf("Failed to load left image!\n");
        return false;
    }

    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("assets/right.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == nullptr) {
        printf("Failed to load right image!\n");
        return false;
    }

    return true;
}

void close() {
    for (auto &gKeyPressSurface : gKeyPressSurfaces) {
        SDL_FreeSurface(gKeyPressSurface);
        gKeyPressSurface = nullptr;
    }

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}

SDL_Surface* loadSurface(const std::string &path) {
    SDL_Surface* optimizedSurface = nullptr;

    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return optimizedSurface;
    }

    // Convert surface to screen format
    optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
    SDL_FreeSurface(loadedSurface);

    if (optimizedSurface == nullptr) {
        printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    return optimizedSurface;
}
