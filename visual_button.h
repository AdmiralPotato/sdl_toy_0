#include <stdbool.h>
#include <SDL.h>

void initButtons (SDL_Renderer *renderer);

void drawButton (SDL_Renderer *renderer, bool state, SDL_Point *position);

void drawLED (SDL_Renderer *renderer, bool state, SDL_Point *position);
