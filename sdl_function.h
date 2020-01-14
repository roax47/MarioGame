#pragma once

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#ifdef BIT64
#include"./sdl64/include/SDL.h"
#include"./sdl64/include/SDL_main.h"
#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
#endif
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480



#ifndef SDL_FUNCTION_H
#define SDLFUNCTION_H

void DrawString(SDL_Surface *screen, int x, int y, const char *text,SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,Uint32 outlineColor, Uint32 fillColor);

#endif