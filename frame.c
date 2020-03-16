#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "frame.h"
#include "visual_button.h"
#include "hex_editor.h"

SDL_Surface *frameSurface;
SDL_Texture *frameTexture;

bool key_up = false;
bool key_down = false;
bool key_left = false;
bool key_right = false;
bool key_shift = false;

bool key_hex_toggle = false;
bool led_hex_toggle = false;

bool key_hex_00000001 = false;
bool led_hex_00000001 = false;
bool key_hex_00000010 = false;
bool led_hex_00000010 = false;
bool key_hex_00000100 = false;
bool led_hex_00000100 = false;
bool key_hex_00001000 = false;
bool led_hex_00001000 = false;
bool key_hex_00010000 = false;
bool led_hex_00010000 = false;
bool key_hex_00100000 = false;
bool led_hex_00100000 = false;
bool key_hex_01000000 = false;
bool led_hex_01000000 = false;
bool key_hex_10000000 = false;
bool led_hex_10000000 = false;

bool key_op_set = false;
bool led_op_set = true;
bool key_op_add = false;
bool led_op_add = false;
bool key_op_sub = false;
bool led_op_sub = false;
bool key_op_xor = false;
bool led_op_xor = false;

bool key_mem_0 = false;
bool led_mem_0 = true;
bool key_mem_1 = false;
bool led_mem_1 = false;
bool key_mem_2 = false;
bool led_mem_2 = false;
bool key_mem_3 = false;
bool led_mem_3 = false;

#define ARROWS_X 24
#define ARROWS_Y 359
struct Button *button = NULL;
struct Button buttons[] = {
    {
        .is_renderable = true,
        .point = { .x = ARROWS_X + 16, .y = ARROWS_Y - 16 },
        .state = &key_up,
        .led = NULL,
        .key = SDL_SCANCODE_UP,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = ARROWS_X + 16, .y = ARROWS_Y + 16 },
        .state = &key_down,
        .led = NULL,
        .key = SDL_SCANCODE_DOWN,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = ARROWS_X, .y = ARROWS_Y },
        .state = &key_left,
        .led = NULL,
        .key = SDL_SCANCODE_LEFT,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = ARROWS_X + 32, .y = ARROWS_Y },
        .state = &key_right,
        .led = NULL,
        .key = SDL_SCANCODE_RIGHT,
        .handler = NULL
    },
    {
        .is_renderable = true,
        .point = { .x = ARROWS_X + 64, .y = ARROWS_Y },
        .state = &key_shift,
        .led = NULL,
        .key = SDL_SCANCODE_LSHIFT,
        .handler = NULL
    },
    {
        .is_renderable = false,
        .point = NO_POINT,
        .state = NULL,
        .led = NULL,
        .key = SDL_SCANCODE_ESCAPE,
        .handler = sdl_quit
    },
    {
        .is_renderable = true,
        .point = { .x = 250, .y = 28 },
        .state = &key_hex_toggle,
        .key = SDL_SCANCODE_TAB,
        .handler = toggleHexEditor,
        .has_led = true,
        .led = {
            .state = &led_hex_toggle,
            .point = { .x = 290, .y = 28 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 415, .y = 359 },
        .state = &key_hex_00000001,
        .key = SDL_SCANCODE_8,
        .handler = runHex00000001,
        .has_led = true,
        .led = {
            .state = &led_hex_00000001,
            .point = { .x = 415, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 375, .y = 359 },
        .state = &key_hex_00000010,
        .key = SDL_SCANCODE_7,
        .handler = runHex00000010,
        .has_led = true,
        .led = {
            .state = &led_hex_00000010,
            .point = { .x = 375, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 335, .y = 359 },
        .state = &key_hex_00000100,
        .key = SDL_SCANCODE_6,
        .handler = runHex00000100,
        .has_led = true,
        .led = {
            .state = &led_hex_00000100,
            .point = { .x = 335, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 295, .y = 359 },
        .state = &key_hex_00001000,
        .key = SDL_SCANCODE_5,
        .handler = runHex00001000,
        .has_led = true,
        .led = {
            .state = &led_hex_00001000,
            .point = { .x = 295, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 255, .y = 359 },
        .state = &key_hex_00010000,
        .key = SDL_SCANCODE_4,
        .handler = runHex00010000,
        .has_led = true,
        .led = {
            .state = &led_hex_00010000,
            .point = { .x = 255, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 215, .y = 359 },
        .state = &key_hex_00100000,
        .key = SDL_SCANCODE_3,
        .handler = runHex00100000,
        .has_led = true,
        .led = {
            .state = &led_hex_00100000,
            .point = { .x = 215, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 175, .y = 359 },
        .state = &key_hex_01000000,
        .key = SDL_SCANCODE_2,
        .handler = runHex01000000,
        .has_led = true,
        .led = {
            .state = &led_hex_01000000,
            .point = { .x = 175, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 135, .y = 359 },
        .state = &key_hex_10000000,
        .key = SDL_SCANCODE_1,
        .handler = runHex10000000,
        .has_led = true,
        .led = {
            .state = &led_hex_10000000,
            .point = { .x = 135, .y = 319 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 42, .y = 116 },
        .state = &key_op_set,
        .key = SDL_SCANCODE_9,
        .handler = opSet,
        .has_led = true,
        .led = {
            .state = &led_op_set,
            .point = { .x = 82, .y = 116 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 42, .y = 156 },
        .state = &key_op_add,
        .key = SDL_SCANCODE_0,
        .handler = opAdd,
        .has_led = true,
        .led = {
            .state = &led_op_add,
            .point = { .x = 82, .y = 156 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 42, .y = 196 },
        .state = &key_op_sub,
        .key = SDL_SCANCODE_MINUS,
        .handler = opSub,
        .has_led = true,
        .led = {
            .state = &led_op_sub,
            .point = { .x = 82, .y = 196 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 42, .y = 236 },
        .state = &key_op_xor,
        .key = SDL_SCANCODE_EQUALS,
        .handler = opXor,
        .has_led = true,
        .led = {
            .state = &led_op_xor,
            .point = { .x = 82, .y = 236 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 499, .y = 116 },
        .state = &key_mem_0,
        .key = SDL_SCANCODE_O,
        .handler = mem0,
        .has_led = true,
        .led = {
            .state = &led_mem_0,
            .point = { .x = 459, .y = 116 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 499, .y = 156 },
        .state = &key_mem_1,
        .key = SDL_SCANCODE_P,
        .handler = mem1,
        .has_led = true,
        .led = {
            .state = &led_mem_1,
            .point = { .x = 459, .y = 156 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 499, .y = 196 },
        .state = &key_mem_2,
        .key = SDL_SCANCODE_LEFTBRACKET,
        .handler = mem2,
        .has_led = true,
        .led = {
            .state = &led_mem_2,
            .point = { .x = 459, .y = 196 }
        }
    },
    {
        .is_renderable = true,
        .point = { .x = 499, .y = 236 },
        .state = &key_mem_3,
        .key = SDL_SCANCODE_RIGHTBRACKET,
        .handler = mem3,
        .has_led = true,
        .led = {
            .state = &led_mem_3,
            .point = { .x = 459, .y = 236 }
        }
    }
};

void sdl_quit (bool state) {
    quit(0);
}

void toggleHexEditor (bool state) {
    if (state) {
        printf("-HexEditor should be toggled\n");
        led_hex_toggle = !led_hex_toggle;
    }
}

void initFrame (SDL_Renderer *renderer) {
    frameSurface = IMG_Load("frame.png");
    if (!frameSurface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        quit(1);
    }
    frameTexture = SDL_CreateTextureFromSurface(renderer, frameSurface);
}

void unloadFrame () {
    SDL_DestroyTexture(frameTexture);
    frameTexture = NULL;
    SDL_FreeSurface(frameSurface);
    frameSurface = NULL;
}

void drawFrame (SDL_Renderer *renderer) {
    SDL_RenderCopy(
            renderer,
            frameTexture,
            &frameSurface->clip_rect,
            &frameSurface->clip_rect
    );
    for (int i = 0; i < BUTTONS; i++) {
        button = &buttons[i];
        if (button->is_renderable) {
            drawButton(
                renderer,
                *buttons[i].state,
                &buttons[i].point
            );
            if (button->has_led) {
                drawLED(
                    renderer,
                    *buttons[i].led.state,
                    &buttons[i].led.point
                );
            }
        }
    }
}
