#include "Timer.h"

Timer::Timer() : mStartTicks(0), mPausedTicks(0), mPaused(false), mStarted(false) {

}

void Timer::start() {
    mStarted = true;
    mPaused = false;
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::stop() {
    mStarted = false;
    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void Timer::pause() {
    if (!mStarted || mPaused) {
        return;
    }

    mPaused = true;
    mPausedTicks = SDL_GetTicks() - mStartTicks;
    mStartTicks = 0;
}

void Timer::resume() {
    if (!mStarted && !mPaused) {
        return;
    }

    mPaused = false;
    mStartTicks = SDL_GetTicks() - mPausedTicks;
    mPausedTicks = 0;
}

Uint32 Timer::getTicks() {
    if (!mStarted) {
        return 0;
    }

    if (mPaused) {
        return mPausedTicks;
    }

    return SDL_GetTicks() - mStartTicks;
}

bool Timer::isStarted() {
    return mStarted;
}

bool Timer::isPaused() {
    return mStarted && mPaused;
}
