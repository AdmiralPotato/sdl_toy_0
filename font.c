#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "font.h"

SDL_Surface *fontSurface;
SDL_Texture *fontTexture;

SDL_Rect fontRects[FONT_CHARS];

SDL_Rect fontWorkingRect = {
    .w = FONT_W,
    .h = FONT_H
};

void initFont (SDL_Renderer *renderer) {
    for (int i = 0; i < FONT_CHARS; ++i) {
        fontRects[i].x = (i % FONT_SHEET_WIDTH) * FONT_W;
        fontRects[i].y = ((int) (i / FONT_SHEET_WIDTH)) * FONT_H;
        fontRects[i].w = FONT_W;
        fontRects[i].h = FONT_H;
    }
    fontSurface = IMG_Load(FONT_NAME);
    if (!fontSurface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        quit(1);
    }
    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
    // don't need fontSurface after it's used as a texture, free it
    SDL_FreeSurface(fontSurface);
    fontSurface = NULL;
}

void unloadFont () {
    SDL_DestroyTexture(fontTexture);
    fontTexture = NULL;
}

void drawString (SDL_Renderer *renderer, char* string, SDL_Rect* bounds) {
    char c;
    char *stringPointer = string;
    int xMax = bounds->x + bounds->w;
    int yMax = bounds->y + bounds->h;
    // printf("drawString Bounds x:%d y:%d w:%d h:%d\n", bounds->x, bounds->y, bounds->w, bounds->h);
    fontWorkingRect.x = bounds->x;
    fontWorkingRect.y = bounds->y;
    while((c = *(stringPointer++))) {
        // printf("Next char is: %d; %c\n", c, c);
        if (
            c == '\n' ||
            fontWorkingRect.x + FONT_W > xMax
        ) {
            fontWorkingRect.x = bounds->x;
            fontWorkingRect.y += FONT_H;
        }
        if (
            fontWorkingRect.y + FONT_H > yMax
        ) {
            break;
        } else if (c != '\n') {
            SDL_RenderCopy(
                renderer,
                fontTexture,
                &fontRects[c - 32],
                &fontWorkingRect
            );
            fontWorkingRect.x += FONT_W;
        }
    }
}
