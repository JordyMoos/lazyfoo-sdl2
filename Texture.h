#pragma LAZYFOO_SDL2_TEXTURE_H

#include <string>
#include <SDL.h>

class Texture {
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string &path);

    void free();

    void render(int x, int y, SDL_Rect* clip = nullptr);

    int getWidth() const;
    int getHeight() const;

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};
