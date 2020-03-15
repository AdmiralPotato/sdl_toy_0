#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "visual_button.h"
#include "font.h"
#include "character.h"
#include "frame.h"

int widthFrame = 544;
int heightFrame = 400;
bool loop = true;
char mutableString[] = "!@#$%^&*()_+{}:\"<>?\\|0123456789\nWhat's up muh goats!!!\nI hope you are all having a great night!YOU WILL BE LICKED BY A GOAT TODAY\n\nso watch out\n\nor don't, your choice\nBut this line won't be rendered at all";
SDL_Rect stringDestinationRect = {
    .x = 16,
    .y = 32,
    .w = 250,
    .h = 96
};

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect gameRect = {
    .x = 112,
    .y = 56,
    .w = 320,
    .h = 240,
};
SDL_Rect gameRectInternal = {
        .x = 0,
        .y = 0,
        .w = 320,
        .h = 240,
};
SDL_Texture *gameTexture;

void quit (int exitCode) {
    loop = false;
    unloadButtons();
    unloadFont();
    unloadFrame();
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
        gameRect.w,
        gameRect.h
    );

    initButtons(renderer);
    initFont(renderer);
    initFrame(renderer);
    initCharacter(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    while (loop) {
        handleEventsSinceLastTick();

        SDL_RenderClear(renderer);
        drawFrame(renderer);

        // use isolated gameTexture for rendering
        SDL_SetRenderTarget(renderer, gameTexture);
        SDL_RenderClear(renderer);

        drawCharacter(renderer);

        drawString(
            renderer,
            mutableString,
            &stringDestinationRect
        );

        // draw isolated gameTexture back on to window texture
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(
            renderer,
            gameTexture,
            &gameRectInternal,
            &gameRect
        );

        SDL_RenderPresent(renderer);

        SDL_Delay(48);
    }

    return 0;
}
