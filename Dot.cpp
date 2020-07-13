#include "Dot.h"
#include "Globals.h"
#include "Texture.h"

extern Texture gDotTexture;
extern bool checkCollision(SDL_Rect a, SDL_Rect b);

Dot::Dot() : mPosX(0), mPosY(0), mVelX(0), mVelY(0) {
    mCollider.w = DOT_WIDTH;
    mCollider.h = DOT_HEIGHT;
}

void Dot::handleEvent(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY -= DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY += DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX -= DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX += DOT_VEL;
                break;
        }
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY += DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY -= DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX += DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX -= DOT_VEL;
                break;
        }
    }
}

void Dot::move() {
    mPosX += mVelX;
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > LEVEL_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    mPosY += mVelY;
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > LEVEL_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Dot::render(int camX, int camY) {
    gDotTexture.render(mPosX - camX, mPosY - camY);
}

int Dot::getPosX() {
    return mPosX;
}

int Dot::getPosY() {
    return mPosY;
}
