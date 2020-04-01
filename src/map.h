#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <tmx.h>

#define MAP_FILE "data/dungeon.tmx"

extern SDL_Rect gameRectInternal;
extern void quit (int exitCode);

void initMap (SDL_Renderer *renderer);

void unloadMap ();

void updateMap ();

void drawMap (SDL_Renderer *renderer);
