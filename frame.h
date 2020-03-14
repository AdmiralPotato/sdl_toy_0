#ifndef FRAME_DEF
#define FRAME_DEF

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#define NO_POINT { .x = 0, .y = 0 }

extern int widthGame;
extern int heightGame;

extern void quit (int exitCode);

extern bool key_up;
extern bool key_down;
extern bool key_left;
extern bool key_right;
extern bool key_shift;

extern bool key_hex_toggle;
extern bool led_hex_toggle;

extern bool key_hex_00000001;
extern bool led_hex_00000001;
extern bool key_hex_00000010;
extern bool led_hex_00000010;
extern bool key_hex_00000100;
extern bool led_hex_00000100;
extern bool key_hex_00001000;
extern bool led_hex_00001000;
extern bool key_hex_00010000;
extern bool led_hex_00010000;
extern bool key_hex_00100000;
extern bool led_hex_00100000;
extern bool key_hex_01000000;
extern bool led_hex_01000000;
extern bool key_hex_10000000;
extern bool led_hex_10000000;

extern bool key_op_set;
extern bool led_op_set;
extern bool key_op_add;
extern bool led_op_add;
extern bool key_op_sub;
extern bool led_op_sub;
extern bool key_op_xor;
extern bool led_op_xor;

extern bool key_mem_0;
extern bool led_mem_0;
extern bool key_mem_1;
extern bool led_mem_1;
extern bool key_mem_2;
extern bool led_mem_2;
extern bool key_mem_3;
extern bool led_mem_3;

void sdl_quit (bool state);

void toggleHexEditor (bool state);

void runHex00000001 (bool state);
void runHex00000010 (bool state);
void runHex00000100 (bool state);
void runHex00001000 (bool state);
void runHex00010000 (bool state);
void runHex00100000 (bool state);
void runHex01000000 (bool state);
void runHex10000000 (bool state);
enum HEX_OPS {
    HEX_OPS_SET,
    HEX_OPS_ADD,
    HEX_OPS_SUB,
    HEX_OPS_XOR
};
void opSet (bool state);
void opAdd (bool state);
void opSub (bool state);
void opXor (bool state);
void mem0 (bool state);
void mem1 (bool state);
void mem2 (bool state);
void mem3 (bool state);

struct LED {
    bool *state;
    SDL_Point point;
};
struct Button {
    bool *state;
    void (*handler)(bool state);
    bool is_renderable;
    bool has_led;
    struct LED led;
    SDL_Scancode key;
    SDL_Point point;
};

#define BUTTONS 23
struct Button *button;
struct Button buttons[BUTTONS];

void initFrame (SDL_Renderer *renderer);

void unloadFrame ();

void drawFrame (SDL_Renderer *renderer);

#endif