#include <stdbool.h>
#include <SDL.h>

#define BUTTON_SIZE 16
#define BUTTON_SIZE_HALF 8
#define LED_SIZE 4
#define LED_SIZE_HALF 2

SDL_Texture* buttonTexture = NULL;
SDL_Rect buttonStateOffRect = {.x = 0, .y = 0, .w = BUTTON_SIZE, .h = BUTTON_SIZE};
SDL_Rect buttonStateOnRect = {.x = BUTTON_SIZE, .y = 0, .w = BUTTON_SIZE, .h = BUTTON_SIZE};
SDL_Rect buttonTargetRect = {.x = 0, .y = 0, .w = BUTTON_SIZE, .h = BUTTON_SIZE};
SDL_Texture* ledTexture = NULL;
SDL_Rect ledStateOffRect = {.x = 0, .y = 0, .w = LED_SIZE, .h = LED_SIZE};
SDL_Rect ledStateOnRect = {.x = LED_SIZE, .y = 0, .w = LED_SIZE, .h = LED_SIZE};
SDL_Rect ledTargetRect = {.x = 0, .y = 0, .w = LED_SIZE, .h = LED_SIZE};

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
    ledTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        LED_SIZE * 2,
        LED_SIZE
    );

    SDL_SetRenderTarget(renderer, buttonTexture);
    SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderFillRect(renderer, &buttonStateOffRect);
    SDL_RenderFillRect(renderer, &buttonStateOnRect);

    SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderFillRect(renderer, &workingRect);
    workingRect.x += BUTTON_SIZE;
    SDL_SetRenderDrawColor(renderer, 0xaa, 0xaa, 0xaa, 0xFF);
    SDL_RenderFillRect(renderer, &workingRect);

    SDL_SetRenderTarget(renderer, ledTexture);
    SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderFillRect(renderer, &ledStateOffRect);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0xFF);
    SDL_RenderFillRect(renderer, &ledStateOnRect);

    SDL_SetRenderTarget(renderer, NULL);
}

void unloadButtons () {
    SDL_DestroyTexture(buttonTexture);
    SDL_DestroyTexture(ledTexture);
    buttonTexture = NULL;
    ledTexture = NULL;
}

void drawButton (SDL_Renderer *renderer, bool state, SDL_Point *position) {
    if (buttonTexture == NULL) {
        printf("visual_button.c: initButton MUST be called before drawButton!\nEXITING ANGRILY!\n");
        exit(1);
    }

    buttonTargetRect.x = position->x - BUTTON_SIZE_HALF;
    buttonTargetRect.y = position->y - BUTTON_SIZE_HALF;

    SDL_RenderCopy(
        renderer,
        buttonTexture,
        state ? &buttonStateOnRect : &buttonStateOffRect,
        &buttonTargetRect
    );
}

void drawLED (SDL_Renderer *renderer, bool state, SDL_Point *position) {
    if (ledTexture == NULL) {
        printf("visual_button.c: initButton MUST be called before drawLED!\nEXITING ANGRILY!\n");
        exit(1);
    }

    ledTargetRect.x = position->x - LED_SIZE_HALF;
    ledTargetRect.y = position->y - LED_SIZE_HALF;

    SDL_RenderCopy(
        renderer,
        ledTexture,
        state ? &ledStateOnRect : &ledStateOffRect,
        &ledTargetRect
    );
}
