#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"

extern SDL_Renderer *gRenderer;

Texture::Texture(): mTexture(nullptr), mWidth(0), mHeight(0) {}

Texture::~Texture() {
    free();
}

bool Texture::loadFromFile(const std::string &path) {
    free();

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return false;
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (newTexture == nullptr) {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        return false;
    }

    mTexture = newTexture;
    return true;
}

void Texture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::render(int x, int y, SDL_Rect *clip) {
    SDL_Rect renderRect = {x, y, mWidth, mHeight};

    if (clip != nullptr) {
        renderRect.w = clip->w;
        renderRect.h = clip->h;
    }

    SDL_RenderCopy(gRenderer, mTexture, clip, &renderRect);
}

int Texture::getWidth() const {
    return mWidth;
}

int Texture::getHeight() const {
    return mHeight;
}
