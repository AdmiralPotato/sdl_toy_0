#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "visual_button.h"
#include "font.h"
#include "character.h"
#include "map.h"
#include "frame.h"
#include "hex_editor.h"

#define GAME_WIDTH 320
#define GAME_HEIGHT 240

int widthFrame = 544;
int heightFrame = 400;
bool loop = true;
char mutableString[129] = "Hello Dankness My Old Friend";
SDL_Rect stringDestinationRect = {
    .x = 16,
    .y = 16,
    .w = GAME_WIDTH - 32,
    .h = GAME_HEIGHT - 32
};

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect gameRectExternal = {
    .x = 112,
    .y = 56,
    .w = GAME_WIDTH,
    .h = GAME_HEIGHT
};
SDL_Rect gameRect = {
        .x = 0,
        .y = 0,
        .w = GAME_WIDTH,
        .h = GAME_HEIGHT
};
SDL_Rect gameRectHalf = {
        .x = 0,
        .y = 0,
        .w = GAME_WIDTH / 2,
        .h = GAME_HEIGHT / 2
};
SDL_Texture *gameTexture;

void quit (int exitCode) {
    loop = false;
    unloadButtons();
    unloadFont();
    unloadFrame();
    unloadMap();
    unloadCharacter();
    SDL_DestroyWindow(window);
    SDL_Quit();
    window = NULL;
    exit(exitCode);
}

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
        widthFrame,
        heightFrame,
        0
    );

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE
    );

    gameTexture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            gameRectExternal.w,
            gameRectExternal.h
    );

    initButtons(renderer);
    initFont(renderer);
    initFrame(renderer);
    initMap(renderer);
    initCharacter(renderer);
    initHexEditor(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    while (loop) {
        handleEventsSinceLastTick();

        stringOffsetToInt16(&playerX, HEX_OFFSET_PLAYERX);
        stringOffsetToInt16(&playerY, HEX_OFFSET_PLAYERY);

        if (led_hex_toggle) {
            updateHexEditor();
        } else {
            updateCharacter();
            updateMap();
        }

        int16ToStringOffset(&playerX, HEX_OFFSET_PLAYERX);
        int16ToStringOffset(&playerY, HEX_OFFSET_PLAYERY);

        updateHexLights();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        drawFrame(renderer);

        // use isolated gameTexture for rendering
        SDL_SetRenderTarget(renderer, gameTexture);
        SDL_RenderClear(renderer);

        drawMap(renderer);
        drawCharacter(renderer);

        drawString(
            renderer,
            mutableString,
            &stringDestinationRect
        );

        if (led_hex_toggle) {
            drawHexEditor(renderer, mutableString);
        }

        // draw isolated gameTexture back on to window texture
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(
            renderer,
            gameTexture,
            &gameRect,
            &gameRectExternal
        );

        SDL_RenderPresent(renderer);

        fflush(stdout); // Smack Windows/CLion console to actually output all printf's this tick
        SDL_Delay(48);
    }

    return 0;
}
