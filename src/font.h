#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "frame.h"


#define FONT_NAME "data/font-monaco_9-6x12-transparent.png"
#define FONT_W 6
#define FONT_H 12
#define FONT_SHEET_WIDTH 32
#define FONT_CHARS 96

extern void quit (int exitCode);

void initFont (SDL_Renderer *renderer);

void unloadFont ();

void drawString (SDL_Renderer *renderer, char* string, SDL_Rect* bounds, float scale);
