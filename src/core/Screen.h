#ifndef SCREEN_H
#define SCREEN_H
#include <SDL3/SDL.h>

#pragma once


class Screen
{
public:
    Screen();
    ~Screen();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height, x, y;
};

#endif