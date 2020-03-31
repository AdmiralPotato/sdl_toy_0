#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "character.h"

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

uint8_t sprite_frame = 0;
uint8_t sprite_direction = 0;
uint8_t movement_speed = 0;
uint8_t walk_speed = 2;
uint8_t run_speed = 4;
bool any_movement = false;

void initCharacter (SDL_Renderer *renderer) {
    playerRect.x = gameRect.w / 2;
    playerRect.y = gameRect.h / 2;
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

void updateCharacter () {
    any_movement = (
        key_up ||
        key_down ||
        key_left ||
        key_right
    );
    movement_speed = key_shift ? run_speed : walk_speed;
    if(key_left) {
        playerRect.x = (playerRect.x - movement_speed + gameRect.w) % gameRect.w;
        sprite_direction = 8;
    }
    if(key_right) {
        playerRect.x = (playerRect.x + movement_speed) % gameRect.w;
        sprite_direction = 12;
    }
    if(key_up) {
        playerRect.y = (playerRect.y - movement_speed + gameRect.h) % gameRect.h;
        sprite_direction = 4;
    }
    if(key_down) {
        playerRect.y = (playerRect.y + movement_speed) % gameRect.h;
        sprite_direction = 0;
    }
    if(any_movement) {
        sprite_frame += 1;
        sprite_frame %= 2;
    }
}

void drawCharacter (SDL_Renderer *renderer) {
    // printf(
    //     "x: %d | y: %d | sprite_frame: %d | sprite_direction: %d\n",
    //     playerRect.x,
    //     playerRect.y,
    //     sprite_frame,
    //     sprite_direction
    // );
    SDL_RenderCopy(
        renderer,
        characterTexture,
        &spriteRects[sprite_frame + sprite_direction],
        &playerRect
    );
}
