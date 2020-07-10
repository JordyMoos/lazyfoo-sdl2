#include <cstdio>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int mainWrapper();

bool init();

bool loadMedia();

void close();

SDL_Texture* loadTexture(const std::string &path);

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
SDL_Texture *gTexture = nullptr;


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

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_Rect topLeftViewport;
        topLeftViewport.x = 0;
        topLeftViewport.y = 0;
        topLeftViewport.w = SCREEN_WIDTH / 2;
        topLeftViewport.h = SCREEN_HEIGHT / 2;
        SDL_RenderSetViewport(gRenderer, &topLeftViewport);
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        SDL_Rect topRightViewport;
        topRightViewport.x = SCREEN_WIDTH / 2;
        topRightViewport.y = 0;
        topRightViewport.w = SCREEN_WIDTH / 2;
        topRightViewport.h = SCREEN_HEIGHT / 2;
        SDL_RenderSetViewport(gRenderer, &topRightViewport);
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        SDL_Rect bottomViewport;
        bottomViewport.x = 0;
        bottomViewport.y = SCREEN_HEIGHT / 2;
        bottomViewport.w = SCREEN_WIDTH;
        bottomViewport.h = SCREEN_HEIGHT / 2;
        SDL_RenderSetViewport(gRenderer, &bottomViewport);
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

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
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if ( ! (IMG_Init(imgFlags) & imgFlags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    return true;
}

bool loadMedia() {
    gTexture = loadTexture("assets/loaded.png");
    if (gTexture == nullptr) {
        printf("Failed to load loaded.png!\n");
        return false;
    }

    return true;
}

void close() {
    if (gTexture != nullptr) {
        SDL_DestroyTexture(gTexture);
        gTexture = nullptr;
    }

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

SDL_Texture* loadTexture(const std::string &path) {
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }

    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (newTexture == nullptr) {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        return nullptr;
    }

    return newTexture;
}
