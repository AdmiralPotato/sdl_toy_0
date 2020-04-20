#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "frame.h"

#define HEX_BYTES 64
#define HEX_COLUMNS 8
#define HEX_COLUMN_OFFSET_X 8
#define HEX_COLUMN_OFFSET_Y 8
#define HEX_CELL_W 19 * 2
#define HEX_CELL_H 14 * 2
#define HEX_CELL_TEXT_X 4 * 2
#define HEX_CELL_TEXT_Y 1.5f * 2
#define HEX_CELL_TEXT_W 12 * 2
#define HEX_CELL_TEXT_H 12 * 2
#define HEX_TICK_DELAY 1

#define HEX_OFFSET_PLAYERX HEX_BYTES - (int16Size * 2)
#define HEX_OFFSET_PLAYERY HEX_BYTES - (int16Size * 1)

extern SDL_Rect gameRect;
extern uint16_t playerX;
extern uint16_t playerY;

size_t intSize;
size_t int16Size;

extern uint8_t currentByteAddress;

extern char mutableString[129];

extern void intToStringOffset (int *n, uint8_t offset);
extern void stringOffsetToInt (int *n, uint8_t offset);
extern void int16ToStringOffset (uint16_t *n, uint8_t offset);
extern void stringOffsetToInt16 (uint16_t *n, uint8_t offset);

extern void runHex00000001 (bool state);
extern void runHex00000010 (bool state);
extern void runHex00000100 (bool state);
extern void runHex00001000 (bool state);
extern void runHex00010000 (bool state);
extern void runHex00100000 (bool state);
extern void runHex01000000 (bool state);
extern void runHex10000000 (bool state);
enum HEX_OPS {
    HEX_OPS_SET,
    HEX_OPS_ADD,
    HEX_OPS_SUB,
    HEX_OPS_XOR
};
char hexOpNames[4][12];
extern void opSet (bool state);
extern void opAdd (bool state);
extern void opSub (bool state);
extern void opXor (bool state);
extern void mem0 (bool state);
extern void mem1 (bool state);
extern void mem2 (bool state);
extern void mem3 (bool state);

void initHexEditor (SDL_Renderer *renderer);

extern void updateHexLights ();

void updateHexEditor ();

void drawHexEditor (SDL_Renderer *renderer, char* string);
