#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "visual_button.h"

#define SPRITE_SIZE 32
#define S_0 0
#define S_1 SPRITE_SIZE * 1
#define S_2 SPRITE_SIZE * 2
#define S_3 SPRITE_SIZE * 3

int width = 640;
int height = 480;
int bitDepth = 32;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *characterSurface;
SDL_Texture *characterTexture;

SDL_Rect spriteRects[16] = {
    {.x =  S_0, .y =  S_0, .w =  S_1, .h =  S_1},
    {.x =  S_1, .y =  S_0, .w =  S_1, .h =  S_1},
    {.x =  S_2, .y =  S_0, .w =  S_1, .h =  S_1},
    {.x =  S_3, .y =  S_0, .w =  S_1, .h =  S_1},
    {.x =  S_0, .y =  S_1, .w =  S_1, .h =  S_1},
    {.x =  S_1, .y =  S_1, .w =  S_1, .h =  S_1},
    {.x =  S_2, .y =  S_1, .w =  S_1, .h =  S_1},
    {.x =  S_3, .y =  S_1, .w =  S_1, .h =  S_1},
    {.x =  S_0, .y =  S_2, .w =  S_1, .h =  S_1},
    {.x =  S_1, .y =  S_2, .w =  S_1, .h =  S_1},
    {.x =  S_2, .y =  S_2, .w =  S_1, .h =  S_1},
    {.x =  S_3, .y =  S_2, .w =  S_1, .h =  S_1},
    {.x =  S_0, .y =  S_3, .w =  S_1, .h =  S_1},
    {.x =  S_1, .y =  S_3, .w =  S_1, .h =  S_1},
    {.x =  S_2, .y =  S_3, .w =  S_1, .h =  S_1},
    {.x =  S_3, .y =  S_3, .w =  S_1, .h =  S_1}
};

bool key_up = false;
bool key_down = false;
bool key_left = false;
bool key_right = false;
bool key_shift = false;

struct Button {
    SDL_Point point;
    bool *state;
};

#define BUTTONS 5
struct Button buttons[] = {
    {
        .point = { .x = 16, .y = 0 },
        .state = &key_up
    },
    {
        .point = { .x = 16, .y = 32 },
        .state = &key_down
    },
    {
        .point = { .x = 0, .y = 16 },
        .state = &key_left
    },
    {
        .point = { .x = 32, .y = 16 },
        .state = &key_right
    },
    {
        .point = { .x = 64, .y = 16 },
        .state = &key_shift
    }
};

void quit (int exitCode) {
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    window = NULL;
    characterTexture = NULL;
    exit(exitCode);
}

int main () {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    initButtons(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    IMG_Init(IMG_INIT_PNG);

    characterSurface = IMG_Load("sans_sprite_uniform.png");
    if(!characterSurface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        quit(1);
    }
    characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    // don't need characterSurface after it's used as a texture, free it
    SDL_FreeSurface(characterSurface);
    characterSurface = NULL;

    /* Filling the surface with red color. */
    SDL_SetRenderDrawColor(renderer, 31, 31, 31, SDL_ALPHA_OPAQUE);

    SDL_Rect playerRect = {
        .x =  width / 2,
        .y =  height / 2,
        .h =  S_1,
        .w =  S_1
    };

    SDL_Event event;
    uint8_t sprite_frame = 0;
    uint8_t sprite_direction = 0;
    uint8_t movement_speed = 0;
    uint8_t walk_speed = 4;
    uint8_t run_speed = 8;
    bool any_movement = false;
    while (1) {
        // Loop over all events that were pushed into the event queue since last tick
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                printf("SDL_QUIT\n");
                quit(0);
            }
            if (
                (
                    event.type == SDL_KEYDOWN ||
                    event.type == SDL_KEYUP
                ) &&
                !event.key.repeat
            ) {
                bool state = event.type == SDL_KEYDOWN;
                // printf("SDL_KEYDOWN, %d\n", event.key.timestamp);
                // printf("event.key.keysym.scancode, %d\n", event.key.keysym.scancode);
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    printf("SDL_SCANCODE_ESCAPE\n");
                    quit(0);
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    key_left = state;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    key_right = state;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    key_up = state;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    key_down = state;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
                    key_shift = state;
                }
            }
        }
        any_movement = (
            key_up ||
            key_down ||
            key_left ||
            key_right
        );
        movement_speed = key_shift ? run_speed : walk_speed;
        if(key_left) {
            playerRect.x = (playerRect.x - movement_speed + width) % width;
            sprite_direction = 8;
        }
        if(key_right) {
            playerRect.x = (playerRect.x + movement_speed) % width;
            sprite_direction = 12;
        }
        if(key_up) {
            playerRect.y = (playerRect.y - movement_speed + height) % height;
            sprite_direction = 4;
        }
        if(key_down) {
            playerRect.y = (playerRect.y + movement_speed) % height;
            sprite_direction = 0;
        }
        if(any_movement) {
            sprite_frame += 1;
            sprite_frame %= 4;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(
            renderer,
            characterTexture,
            &spriteRects[sprite_frame + sprite_direction],
            &playerRect
        );
        for (char i = 0; i < BUTTONS; i++) {
            drawButton(
                renderer,
                *buttons[i].state,
                &buttons[i].point
            );
        }
        SDL_RenderPresent(renderer);
        printf(
            "x: %d | y: %d | sprite_frame: %d | sprite_direction: %d\n",
            playerRect.x,
            playerRect.y,
            sprite_frame,
            sprite_direction
        );
        SDL_Delay(66);
    }

    return 0;
}
