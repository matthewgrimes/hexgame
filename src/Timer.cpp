#include "Timer.h"
#include <SDL2/SDL.h>
Timer::Timer()
{
    //ctor
}

Timer::~Timer()
{
    //dtor
}

void Timer::Start()
{
    startTime = SDL_GetTicks();
}

void Timer::Reset()
{
    startTime = SDL_GetTicks();
}

float Timer::getTicksElapsed()
{
    return SDL_GetTicks() - startTime;
}

float Timer::getSecondsElapsed()
{
    return getTicksElapsed() * 1.f/1000.f;
}
