#include "zeebo.h"

void draw_cmd_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    if (draw_mode) {
        for (int i = 0; i < w; i++) {
            *(uint16_t *)(((uint16_t*) 0x6800000) + ((y) * 256 + (x + i))) = draw_color.pixel;
        }
        for (int i = 0; i <= w; i++) {
            *(uint16_t *)(((uint16_t*) 0x6800000) + ((y + h) * 256 + (x + i))) = draw_color.pixel;
        }
        for (int i = 0; i < h; i++) {
            *(uint16_t *)(((uint16_t*) 0x6800000) + ((y + i) * 256 + (x))) = draw_color.pixel;
        }
        for (int i = 0; i < h; i++) {
            *(uint16_t *)(((uint16_t*) 0x6800000) + ((y + i) * 256 + (x + w))) = draw_color.pixel;
        }
        return;
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            *(uint16_t *)(((uint16_t*) 0x6800000) + ((y + i) * 256 + (x + j))) = draw_color.pixel;
        }
    }
}
