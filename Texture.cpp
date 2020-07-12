#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Texture.h"

extern SDL_Renderer *gRenderer;
extern TTF_Font *gFont;

Texture::Texture(): mTexture(nullptr), mWidth(0), mHeight(0) {}

Texture::~Texture() {
    free();
}

bool Texture::loadFromFile(const std::string &path) {
    free();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return false;
    }

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

    mTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    mWidth = surface->w;
    mHeight = surface->h;
    SDL_FreeSurface(surface);
    if (mTexture == nullptr) {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        return false;
    }

    return true;
}

bool Texture::loadFromRenderedText(const std::string &text, SDL_Color color) {
    free();

    SDL_Surface* surface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (surface == nullptr) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }

    mTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    mWidth = surface->w;
    mHeight = surface->h;
    SDL_FreeSurface(surface);
    if (mTexture == nullptr) {
        printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    return true;
}

void Texture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blendMode) {
    SDL_SetTextureBlendMode(mTexture, blendMode);
}

void Texture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect *clip,
        double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderRect = {x, y, mWidth, mHeight};

    if (clip != nullptr) {
        renderRect.w = clip->w;
        renderRect.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderRect, angle, center, flip);
}

int Texture::getWidth() const {
    return mWidth;
}

int Texture::getHeight() const {
    return mHeight;
}
