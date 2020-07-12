#pragma LAZYFOO_SDL2_TEXTURE_H

#include <string>
#include <SDL.h>

class Texture {
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string &path);
    bool loadFromRenderedText(const std::string &text, SDL_Color color);

    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setAlpha(Uint8 alpha);
    void setBlendMode(SDL_BlendMode blendMode);
    void render(int x, int y, SDL_Rect* clip = nullptr,
            double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth() const;
    int getHeight() const;

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};
