#include "zeebo.h"

static uint8_t flush_mode;
static color_t tint = {0xFF00};
static color_t clear = {0x0FFF};

color_t draw_color = {0x00FF};
uint8_t draw_mode;

void draw_cmd_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (draw_mode != 3) {
        tint.c16.color.r = r;
        tint.c16.color.g = g;
        tint.c16.color.b = b;
    } else {
        clear.c16.color.r = r;
        clear.c16.color.g = g;
        clear.c16.color.b = b;
    }

    if (flush_mode) {
        static const uint32_t *end = (uint32_t *) (0x06000000 + (240 * 160 * 2));
        static uint16_t old_clear = 0;
        draw_color.arr[0] = clear.pixel;
        draw_color.arr[1] = clear.pixel;
        if (old_clear != clear.pixel) {
            uint32_t *i = (uint32_t *) 0x06000000;
            while (i < end) {
                *i++ = draw_color.pixel2;
            }
            old_clear = clear.pixel;
        }
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
