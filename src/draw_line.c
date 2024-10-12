#include "core_native_gba.h"

void native_draw_line_func(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{  
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xstep = (x1 < x2) ? 1 : -1;
    int ystep = (y1 < y2) ? 1 : -1;
    int dd;

    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }

    uint16_t *dst = (uint16_t *)(0x06000000 + (y1 * 240 + x1) * 2);
    int dstPitch = 240;

    if (dx >= dy) {
        dd = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            *dst = color_current.pixel;
            if (dd >= 0) {
                dd -= 2 * dx;
                dst += ystep * dstPitch;
            }
            dd += 2 * dy;
            dst += xstep;
        }
    } else {
        dd = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            *dst = color_current.pixel;
            if (dd >= 0) {
                dd -= 2 * dy;
                dst += xstep;
            }
            dd += 2 * dx;
            dst += ystep * dstPitch;
        }
    }
}

/**
 * @todo refactor names
 * @li https://www.coranac.com/tonc/text/bitmaps.htm
 */
void native_draw_line_flush()
{
    uint16_t x1 = draw_queue[draw_index_erase++];
    uint16_t y1 = draw_queue[draw_index_erase++];
    uint16_t x2 = draw_queue[draw_index_erase++];
    uint16_t y2 = draw_queue[draw_index_erase++];
    native_draw_line_func(x1, y1, x2, y2);
}
