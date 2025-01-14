#include "zeebo.h"

void draw_cmd_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
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

    uint16_t *dst = (uint16_t *)(((uint16_t*) 0x6800000) + (y1 * 256 + x1));
    int dstPitch = 256;

    if (dx >= dy) {
        dd = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            *dst = draw_color.pixel;
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
            *dst = draw_color.pixel;
            if (dd >= 0) {
                dd -= 2 * dy;
                dst += xstep;
            }
            dd += 2 * dx;
            dst += ystep * dstPitch;
        }
    }
}
