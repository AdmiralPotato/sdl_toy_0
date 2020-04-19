#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "frame.h"

#define SPRITE_SIZE 32

#define S_0 0
#define S_05 8
#define S_1 SPRITE_SIZE * 1
#define S_2 SPRITE_SIZE * 2
#define S_3 SPRITE_SIZE * 3
#define S_4 SPRITE_SIZE * 4
#define S_5 SPRITE_SIZE * 5
#define CHARACTER_WIDTH S_1

#define CHARACTER_DIRECTIONS 4

#define CHARACTER_BLINK_INDEX 0
#define CHARACTER_BLINK_ROW 0
#define CHARACTER_BLINK_ROWS 2

#define CHARACTER_WALK_INDEX 1
#define CHARACTER_WALK_ROW 2
#define CHARACTER_WALK_ROWS 4

extern SDL_Rect gameRect;
extern SDL_Rect mapRect;
extern SDL_Rect playerRect;
extern uint16_t playerX;
extern uint16_t playerY;

extern void quit (int exitCode);

void initCharacter (SDL_Renderer *renderer);

void unloadCharacter ();

void updateCharacter ();

void drawCharacter (SDL_Renderer *renderer);