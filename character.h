#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "frame.h"

#define SPRITE_SIZE 32

#define S_0 0
#define S_1 SPRITE_SIZE * 1
#define S_2 SPRITE_SIZE * 2
#define S_3 SPRITE_SIZE * 3
#define CHARACTER_WIDTH S_1

extern SDL_Rect gameRect;

extern void quit (int exitCode);

void initCharacter (SDL_Renderer *renderer);

void unloadCharacter ();

void updateCharacter ();

void drawCharacter (SDL_Renderer *renderer);