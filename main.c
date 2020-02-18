#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"
#include "SDL_image.h"

int main () {
    int width = 640;
    int height = 480;
    int depth = 32;
    /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
       as expected by OpenGL for textures */
    SDL_Surface *surface;

    /* or using the default masks for the depth: */
    surface = SDL_CreateRGBSurface(0, width, height, depth, 0, 0, 0, 0);

    if (surface == NULL) {
        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
        exit(1);
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE
    );
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    /* Filling the surface with red color. */
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = {x: 128, y: 256, h: 32, w: 64};

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
        fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Event event;
    bool key_up = false;
    bool key_down = false;
    bool key_left = false;
    bool key_right = false;
    while (1) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                printf("SDL_QUIT\n");
                exit(0);
            }
            if (
                (
                    event.type == SDL_KEYDOWN ||
                    event.type == SDL_KEYUP
                ) &&
                !event.key.repeat
            ) {
                bool state = event.type == SDL_KEYDOWN;
                printf("SDL_KEYDOWN, %d\n", event.key.timestamp);
                printf("event.key.keysym.scancode, %d\n", event.key.keysym.scancode);
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    printf("SDL_SCANCODE_ESCAPE\n");
                    exit(0);
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    printf("left %d\n", state);
                    key_left = state;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    printf("right %d\n", state);
                    key_right = state;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    printf("up %d\n", state);
                    key_up = state;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    printf("down %d\n", state);
                    key_down = state;
                }
            }
        }

        if(key_left) {
            rect.x = (rect.x - 1 + width) % width;
        }
        if(key_right) {
            rect.x = (rect.x + 1) % width;
        }
        if(key_up) {
            rect.y = (rect.y - 1 + height) % height;
        }
        if(key_down) {
            rect.y = (rect.y + 1) % height;
        }
        
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        printf("x: %d\n", rect.x);
        // Get the next event
        SDL_Delay(24);
    }

    SDL_FreeSurface(surface);
    surface = NULL;
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
