#include "core_native_gba.h"

static uint8_t flush_mode;
static union color_u tint = {0xFF00};
static union color_u clear = {0x0FFF};

union color_u draw_color = {0x00FF};
uint8_t draw_mode;

void draw_cmd_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (draw_mode != 3) {
        tint.c16.color.r = r;
        tint.c16.color.g = g;
        tint.c16.color.b = b;
        tint.c16.color.a = 0;
    } else {
        clear.c16.color.r = r;
        clear.c16.color.g = g;
        clear.c16.color.b = b;
        clear.c16.color.a = 0;
    }

    if (flush_mode) {
        draw_color.arr[0] = clear.pixel;
        draw_color.arr[1] = clear.pixel;
    } else {
        draw_color.arr[0] = tint.pixel;
        draw_color.arr[1] = tint.pixel;
    }
}

void draw_cmd_mode(uint8_t drawmode, uint8_t flushmode, uint8_t change_mode, uint8_t change_flush)
{
    if (change_mode) {
        draw_mode = drawmode;
    }
    if (change_flush) {
        flush_mode = flushmode;
    }
}
