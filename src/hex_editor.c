#include <stdio.h>
#include <SDL.h>

#include "hex_editor.h"
#include "font.h"
#include "map.h"

uint8_t currentByteAddress = 0;

size_t intSize = sizeof(int);
size_t int16Size = sizeof(uint16_t);

SDL_Rect byteHighlightRects[HEX_BYTES];
SDL_Rect byteFontRects[HEX_BYTES];

char hexByteString[3];
char hexOpNames[4][12] = {
    "HEX_OPS_SET",
    "HEX_OPS_ADD",
    "HEX_OPS_SUB",
    "HEX_OPS_XOR"
};
enum HEX_OPS currentOp = HEX_OPS_SET;
uint8_t currentMem = 0;
uint8_t mems[] = { 0, 0, 0, 0 };

extern void intToStringOffset (int *n, uint8_t offset) {
    memcpy(
        mutableString + offset,
        n,
        intSize
    );
}
extern void stringOffsetToInt (int *n, uint8_t offset) {
    memcpy(
        n,
        mutableString + offset,
        intSize
    );
}
extern void int16ToStringOffset (uint16_t *n, uint8_t offset) {
    memcpy(
        mutableString + offset,
        n,
        int16Size
    );
}
extern void stringOffsetToInt16 (uint16_t *n, uint8_t offset) {
    memcpy(
        n,
        mutableString + offset,
        int16Size
    );
}

void updateHexLights () {
    uint8_t value = mutableString[currentByteAddress];
    led_hex_00000001 = (uint8_t) 0b00000001 & value;
    led_hex_00000010 = (uint8_t) 0b00000010 & value;
    led_hex_00000100 = (uint8_t) 0b00000100 & value;
    led_hex_00001000 = (uint8_t) 0b00001000 & value;
    led_hex_00010000 = (uint8_t) 0b00010000 & value;
    led_hex_00100000 = (uint8_t) 0b00100000 & value;
    led_hex_01000000 = (uint8_t) 0b01000000 & value;
    led_hex_10000000 = (uint8_t) 0b10000000 & value;
}

void runHex (bool state, char value) {
    if (state) {
        printf("runHex called with: %d op: %s\n", value, hexOpNames[currentOp]);
        switch (currentOp) {
            case HEX_OPS_SET: mutableString[currentByteAddress] ^= value; break;
            case HEX_OPS_ADD: mutableString[currentByteAddress] += value; break;
            case HEX_OPS_SUB: mutableString[currentByteAddress] -= value; break;
            case HEX_OPS_XOR: mutableString[currentByteAddress] ^= value - mutableString[currentByteAddress]; break;
        }
        updateHexLights();
    }
}
void runHex00000001 (bool state) { runHex(state, (char) 0b00000001); }
void runHex00000010 (bool state) { runHex(state, (char) 0b00000010); }
void runHex00000100 (bool state) { runHex(state, (char) 0b00000100); }
void runHex00001000 (bool state) { runHex(state, (char) 0b00001000); }
void runHex00010000 (bool state) { runHex(state, (char) 0b00010000); }
void runHex00100000 (bool state) { runHex(state, (char) 0b00100000); }
void runHex01000000 (bool state) { runHex(state, (char) 0b01000000); }
void runHex10000000 (bool state) { runHex(state, (char) 0b10000000); }

void setOp (bool state, enum HEX_OPS op) {
    if (state) {
        currentOp = op;
        led_op_set = false;
        led_op_add = false;
        led_op_sub = false;
        led_op_xor = false;
        switch (op) {
            case HEX_OPS_SET: led_op_set = true; break;
            case HEX_OPS_ADD: led_op_add = true; break;
            case HEX_OPS_SUB: led_op_sub = true; break;
            case HEX_OPS_XOR: led_op_xor = true; break;
            default: break;
        }
        printf("setOp called with: %s\n", hexOpNames[currentOp]);
    }
}
void opSet (bool state) { setOp(state, HEX_OPS_SET); }
void opAdd (bool state) { setOp(state, HEX_OPS_ADD); }
void opSub (bool state) { setOp(state, HEX_OPS_SUB); }
void opXor (bool state) { setOp(state, HEX_OPS_XOR); }

void setMem (bool state, uint8_t index) {
    if (state) {
        currentMem = index;
        led_mem_0 = false;
        led_mem_1 = false;
        led_mem_2 = false;
        led_mem_3 = false;
        switch (index) {
            case 0: led_mem_0 = true; break;
            case 1: led_mem_1 = true; break;
            case 2: led_mem_2 = true; break;
            case 3: led_mem_3 = true; break;
            default: break;
        }
        if (led_hex_toggle) {
            mems[currentMem] = currentByteAddress;
        } else {
            currentByteAddress = mems[currentMem];
        }
        printf(
            "setMem called to %s: %d on address: %d\n",
            led_hex_toggle ? "set" : "get",
            index,
            currentByteAddress
        );
        updateHexLights();
    }
}
void mem0 (bool state) { setMem(state, 0); }
void mem1 (bool state) { setMem(state, 1); }
void mem2 (bool state) { setMem(state, 2); }
void mem3 (bool state) { setMem(state, 3); }

void initHexEditor (SDL_Renderer *renderer) {
    for (int i = 0; i < HEX_BYTES; ++i) {
        byteHighlightRects[i].x = (i % HEX_COLUMNS) * HEX_CELL_W + HEX_COLUMN_OFFSET_X;
        byteHighlightRects[i].y = ((int) (i / HEX_COLUMNS)) * HEX_CELL_H + HEX_COLUMN_OFFSET_Y;
        byteHighlightRects[i].w = HEX_CELL_W;
        byteHighlightRects[i].h = HEX_CELL_H;
        byteFontRects[i].x = byteHighlightRects[i].x + HEX_CELL_TEXT_X;
        byteFontRects[i].y = byteHighlightRects[i].y + HEX_CELL_TEXT_Y;
        byteFontRects[i].w = HEX_CELL_TEXT_W;
        byteFontRects[i].h = HEX_CELL_TEXT_H;
    }
    int16ToStringOffset(&playerX, HEX_OFFSET_PLAYERX);
    int16ToStringOffset(&playerY, HEX_OFFSET_PLAYERY);
    updateHexLights();
}

bool anyHexMovement = false;
uint8_t delay = 0;
void updateHexEditor () {
    if (!delay) {
        anyHexMovement = (
            key_left ||
            key_right ||
            key_up ||
            key_down
        );
        if (key_left) {
            currentByteAddress = (currentByteAddress + HEX_BYTES - 1) % HEX_BYTES;
        }
        if (key_right) {
            currentByteAddress = (currentByteAddress + 1) % HEX_BYTES;
        }
        if (key_up) {
            currentByteAddress = (currentByteAddress + HEX_BYTES - HEX_COLUMNS) % HEX_BYTES;
        }
        if (key_down) {
            currentByteAddress = (currentByteAddress + HEX_COLUMNS) % HEX_BYTES;
        }
        if (anyHexMovement) {
            delay = HEX_TICK_DELAY;
            updateHexLights();
        }
    } else {
        delay--;
    }
}

void getHexStringForByte (uint8_t byte, char* outputString) {
    sprintf(outputString,"%02X", byte);
}

void drawHexEditor (SDL_Renderer *renderer, char* string) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_RenderFillRect(renderer, &gameRect);

    SDL_SetRenderDrawColor(renderer, 64, 64, 255, 255);
    SDL_RenderFillRect(
            renderer,
            &byteHighlightRects[currentByteAddress]
    );

    for (int i = 0; i < HEX_BYTES; ++i) {
        getHexStringForByte(string[i], hexByteString);
        drawString(
            renderer,
            hexByteString,
            &byteFontRects[i],
            2.0f
        );
    }
}
