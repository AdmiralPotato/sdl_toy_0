#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "character.h"
#include "hex_editor.h"
#include "map.h"
#include "font.h"

SDL_Surface *characterSurface;
SDL_Texture *characterTexture;

SDL_Rect spriteRects[16] = {
        {.x =  S_0, .y =  S_0, .w =  S_1, .h =  S_1},
        {.x =  S_1, .y =  S_0, .w =  S_1, .h =  S_1},
        {.x =  S_0, .y =  S_1, .w =  S_1, .h =  S_1},
        {.x =  S_1, .y =  S_1, .w =  S_1, .h =  S_1},
        {.x =  S_0, .y =  S_2, .w =  S_1, .h =  S_1},
        {.x =  S_1, .y =  S_2, .w =  S_1, .h =  S_1},
        {.x =  S_0, .y =  S_3, .w =  S_1, .h =  S_1},
        {.x =  S_1, .y =  S_3, .w =  S_1, .h =  S_1},
};

SDL_Rect playerRect;
uint16_t playerX = 32;
uint16_t playerY = 32;

uint8_t sprite_frame = 0;
uint8_t sprite_direction = 0;
uint8_t movement_speed = 0;
uint8_t walk_speed = 2;
uint8_t run_speed = 4;
bool any_movement = false;

void initCharacter (SDL_Renderer *renderer) {
    playerRect.x = gameRect.w / 2 - S_05;
    playerRect.y = gameRect.h / 2 - S_05;
    playerRect.h = CHARACTER_WIDTH;
    playerRect.w = CHARACTER_WIDTH;
    characterSurface = IMG_Load("data/black_mage-tiny.png");
    if (!characterSurface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        quit(1);
    }
    characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    // don't need characterSurface after it's used as a texture, free it
    SDL_FreeSurface(characterSurface);
    characterSurface = NULL;
}

void unloadCharacter () {
    SDL_DestroyTexture(characterTexture);
    characterTexture = NULL;
}

tmx_tile* currentTile = NULL;
char currentTileType[36] = "none";
int movementX = 0;
int movementY = 0;
void updateCharacter () {
    any_movement = (
        key_up ||
        key_down ||
        key_left ||
        key_right
    );
    movement_speed = key_shift ? run_speed : walk_speed;
    movementX = 0;
    movementY = 0;
    if(key_left) {
        movementX = -movement_speed;
        sprite_direction = 4;
    }
    if(key_right) {
        movementX = movement_speed;
        sprite_direction = 6;
    }
    if(key_up) {
        movementY = -movement_speed;
        sprite_direction = 2;
    }
    if(key_down) {
        movementY = movement_speed;
        sprite_direction = 0;
    }
    if(any_movement) {
        sprite_frame = (sprite_frame + 1) % 2;
        uint16_t nextX = (playerX + movementX + mapRect.w) % mapRect.w;
        uint16_t nextY = (playerY + movementY + mapRect.h) % mapRect.h;
        currentTile = getTileByCoordinate(nextX, nextY);
        if(currentTile) {
            snprintf(
                currentTileType,
                36,
                "%s",
                currentTile->type ? currentTile->type : "none"
            );
            if (strcmp(currentTileType, "wall") != 0) {
                playerX = nextX;
                playerY = nextY;
            }
        }
    }
}

SDL_Rect currentTileTypeNameRect = {
    .x = 16,
    .y = 240 - 16,
    .w = 320 - 32,
    .h = 16
};
void drawCharacter (SDL_Renderer *renderer) {
    // printf(
    //     "x: %d | y: %d | sprite_frame: %d | sprite_direction: %d\n",
    //     playerX,
    //     playerY,
    //     sprite_frame,
    //     sprite_direction
    // );
    SDL_RenderCopy(
        renderer,
        characterTexture,
        &spriteRects[sprite_frame + sprite_direction],
        &playerRect
    );
    drawString(
        renderer,
        currentTileType,
        &currentTileTypeNameRect
    );
}
