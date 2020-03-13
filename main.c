#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "visual_button.h"
#include "character.h"

#define NO_POINT { .x = 0, .y = 0 }

int width = 640;
int height = 480;
int bitDepth = 32;
bool loop = true;

SDL_Window *window;
SDL_Renderer *renderer;

bool key_up = false;
bool key_down = false;
bool key_left = false;
bool key_right = false;
bool key_shift = false;

void quit (int exitCode) {
    loop = false;
    unloadCharacter();
    SDL_DestroyWindow(window);
    SDL_Quit();
    window = NULL;
    exit(exitCode);
}

void sdl_quit (bool state) {
    quit(0);
}

struct Button {
    bool *state;
    void (*handler)(bool state);
    bool is_renderable;
    SDL_Scancode key;
    SDL_Point point;
};

#define BUTTONS 6
struct Button *button = NULL;
struct Button buttons[] = {
    {
        .is_renderable = true,
        .point = { .x = 32, .y = 16 },
        .state = &key_up,
        .key = SDL_SCANCODE_UP,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = 32, .y = 48 },
        .state = &key_down,
        .key = SDL_SCANCODE_DOWN,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = 16, .y = 32 },
        .state = &key_left,
        .key = SDL_SCANCODE_LEFT,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = 48, .y = 32 },
        .state = &key_right,
        .key = SDL_SCANCODE_RIGHT,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = 76, .y = 32 },
        .state = &key_shift,
        .key = SDL_SCANCODE_LSHIFT,
        .handler = NULL
    },
    {
        .is_renderable = false,
        .point = NO_POINT,
        .state = NULL,
        .key = SDL_SCANCODE_ESCAPE,
        .handler = sdl_quit
    }
};

SDL_Event event;
SDL_Scancode key;
void handleEventsSinceLastTick () {
    // Loop over all events that were pushed into the event queue since last tick
    while (SDL_PollEvent(&event)) {
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
            key = event.key.keysym.scancode;
            bool state = event.type == SDL_KEYDOWN;
            // printf("SDL_KEYDOWN, %d\n", event.key.timestamp);
            // printf("event.key.keysym.scancode, %d\n", event.key.keysym.scancode);
            for (int i = 0; i < BUTTONS; i++) {
                button = &buttons[i];
                if (button->key == key) {
                    if (button->state) {
                        *button->state = state;
                    }
                    if (button->handler) {
                        button->handler(state);
                    }
                }
            }
        }
    }
}

int main () {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

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
        SDL_RENDERER_SOFTWARE
    );

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    initButtons(renderer);
    initCharacter(renderer);

    SDL_SetRenderDrawColor(renderer, 31, 31, 31, SDL_ALPHA_OPAQUE);

    while (loop) {
        handleEventsSinceLastTick();

        SDL_RenderClear(renderer);

        drawCharacter(renderer);

        for (int i = 0; i < BUTTONS; i++) {
            button = &buttons[i];
            if (button->is_renderable) {
                drawButton(
                    renderer,
                    *buttons[i].state,
                    &buttons[i].point
                );
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(32);
    }

    return 0;
}
