#include "zeebo.h"

void draw_cmd_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    if (draw_mode) {
        for (int i = 0; i < w; i++) {
            *(uint16_t *)(0x06000000 + ((y) * 240 + (x + i)) * 2) = draw_color.pixel;
        }
        for (int i = 0; i <= w; i++) {
            *(uint16_t *)(0x06000000 + ((y + h) * 240 + (x + i)) * 2) = draw_color.pixel;
        }
        for (int i = 0; i < h; i++) {
            *(uint16_t *)(0x06000000 + ((y + i) * 240 + (x)) * 2) = draw_color.pixel;
        }
        for (int i = 0; i < h; i++) {
            *(uint16_t *)(0x06000000 + ((y + i) * 240 + (x + w)) * 2) = draw_color.pixel;
        }
        return;
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            *(uint16_t *)(0x06000000 + ((y + i) * 240 + (x + j)) * 2) = draw_color.pixel;
        }
    }
}
