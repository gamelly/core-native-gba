#include "core_native_gba.h"

void native_draw_rect_flush()
{
    uint8_t mode = draw_queue[draw_index_erase++];
    uint16_t x = draw_queue[draw_index_erase++];
    uint16_t y = draw_queue[draw_index_erase++];
    uint16_t w = draw_queue[draw_index_erase++];
    uint16_t h = draw_queue[draw_index_erase++];

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            *(uint16_t *)(0x06000000 + ((y + i) * 240 + (x + j)) * 2) = color_current.pixel;
        }
    }
}
