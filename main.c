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

    IMG_Init(IMG_INIT_PNG);
    // load sample.png into image
    SDL_Surface *image;
    image = IMG_Load("sans_sprite_uniform.png");
    if(!image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    int s = 32;
    SDL_Rect spriteRects[16] = {
        {.x =  00, .y =  00, .w =  s, .h =  s},
        {.x =  32, .y =  00, .w =  s, .h =  s},
        {.x =  64, .y =  00, .w =  s, .h =  s},
        {.x =  96, .y =  00, .w =  s, .h =  s},
        {.x =  00, .y =  32, .w =  s, .h =  s},
        {.x =  32, .y =  32, .w =  s, .h =  s},
        {.x =  64, .y =  32, .w =  s, .h =  s},
        {.x =  96, .y =  32, .w =  s, .h =  s},
        {.x =  00, .y =  64, .w =  s, .h =  s},
        {.x =  32, .y =  64, .w =  s, .h =  s},
        {.x =  64, .y =  64, .w =  s, .h =  s},
        {.x =  96, .y =  64, .w =  s, .h =  s},
        {.x =  00, .y =  96, .w =  s, .h =  s},
        {.x =  32, .y =  96, .w =  s, .h =  s},
        {.x =  64, .y =  96, .w =  s, .h =  s},
        {.x =  96, .y =  96, .w =  s, .h =  s}
    };
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, image);
    

    /* Filling the surface with red color. */
    SDL_SetRenderDrawColor(renderer, 31, 31, 31, SDL_ALPHA_OPAQUE);

    SDL_Rect rect = {x: 128, y: 256, h: 32, w: 32};

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
    bool key_shift = false;
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
                if(event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
                    printf("down %d\n", state);
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
            rect.x = (rect.x - movement_speed + width) % width;
            sprite_direction = 8;
        }
        if(key_right) {
            rect.x = (rect.x + movement_speed) % width;
            sprite_direction = 12;
        }
        if(key_up) {
            rect.y = (rect.y - movement_speed + height) % height;
            sprite_direction = 4;
        }
        if(key_down) {
            rect.y = (rect.y + movement_speed) % height;
            sprite_direction = 0;
        }
        if(any_movement) {
            sprite_frame += 1;
            sprite_frame %= 4;
        }
        // SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(
            renderer,
            imageTexture,
            &spriteRects[sprite_frame + sprite_direction],
            &rect
        );
        SDL_RenderPresent(renderer);
        printf(
            "x: %d | y: %d | sprite_frame: %d | sprite_direction: %d\n",
            rect.x,
            rect.y,
            sprite_frame,
            sprite_direction
        );
        SDL_Delay(45);
    }
    SDL_FreeSurface(image);
    SDL_FreeSurface(surface);
    surface = NULL;
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
