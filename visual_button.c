#include <stdbool.h>
#include <SDL.h>

#define BUTTON_SIZE 16

SDL_Texture* buttonTexture = NULL;
SDL_Rect buttonStateOffRect = {.x = 0, .y = 0, .w = BUTTON_SIZE, .h = BUTTON_SIZE};
SDL_Rect buttonStateOnRect = {.x = BUTTON_SIZE, .y = 0, .w = BUTTON_SIZE, .h = BUTTON_SIZE};
SDL_Rect buttonTargetRect = {.x = 0, .y = 0, .w = BUTTON_SIZE, .h = BUTTON_SIZE};

void initButtons (SDL_Renderer *renderer) {
    SDL_Rect workingRect = {
        .x = BUTTON_SIZE / 4,
        .y = BUTTON_SIZE / 4,
        .w = BUTTON_SIZE / 2,
        .h = BUTTON_SIZE / 2
    };
    buttonTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        BUTTON_SIZE * 2,
        BUTTON_SIZE
    );
    SDL_SetRenderTarget(renderer, buttonTexture);

    SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0x66, 0xFF);
    SDL_RenderFillRect(renderer, &buttonStateOffRect);
    SDL_RenderFillRect(renderer, &buttonStateOnRect);

    SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderFillRect(renderer, &workingRect);
    workingRect.x += BUTTON_SIZE;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &workingRect);

    SDL_SetRenderTarget(renderer, NULL);
}

void drawButton (SDL_Renderer *renderer, bool state, SDL_Point *position) {
    if (buttonTexture == NULL) {
        printf("visual_button.c: initButton MUST be called before drawButton!\nEXITING ANGRILY!\n");
        exit(1);
    }

    buttonTargetRect.x = position->x;
    buttonTargetRect.y = position->y;

    SDL_RenderCopy(
        renderer,
        buttonTexture,
        state ? &buttonStateOnRect : &buttonStateOffRect,
        &buttonTargetRect
    );
}