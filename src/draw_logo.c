#include "zeebo.h"

#ifndef ENGINE_LOGO
#error invalid ENGINE_LOGO
#endif

extern void gly_type_render(uint8_t, uint8_t, uint8_t, char*, void*);

void draw_logo()
{
#if ENGINE_LOGO > 0
    draw_queue_page(1);
    draw_queue_push(48, 3, 0, 1, 0);
    draw_queue_push(49, 0, 0, 0, 0);
#if ENGINE_LOGO == 1
    static const uint8_t ver_x = 97;
    static const uint8_t ver_y = 80;
    draw_queue_push(48, 0, 0, 1, 0);
    draw_queue_push(49, 0x00, 0x00, 0xFF, 0);
    draw_queue_push(50, 90, 45, 50, 15);
    draw_queue_push(49, 0x00, 0xFF, 0x67, 0);
    draw_queue_push(50, 90, 45+15, 50, 5);
    draw_queue_push(49, 0xFF, 0xe7, 0x00, 0);
    draw_queue_push(50, 90, 45+20, 50, 5);
    draw_queue_push(49, 0xFF, 0x76, 0x00, 0);
    draw_queue_push(50, 90, 45+25, 50, 5);
    draw_queue_push(49, 0xFF, 0x00, 0x00, 0);
    draw_queue_push(50, 90, 45+30, 50, 5);
    draw_queue_push(49, 0xFF, 0x00, 0xFF, 0);
    draw_queue_push(50, 90, 45+35, 50, 10);
    draw_queue_push(48, 1, 0, 1, 0);
    draw_queue_push(49, 0xFF, 0xFF, 0xFF, 0);
    gly_type_render(95, 56, 12, "gly", draw_queue_clojure(51));
    gly_type_render(95, 72, 5, "engine", draw_queue_clojure(51));
    draw_queue_push(50, 90, 45, 50, 45);
#elif ENGINE_LOGO == 2
    static const uint8_t ver_x = 97;
    static const uint8_t ver_y = 84;
    draw_queue_push(48, 0, 0, 1, 0);
    draw_queue_push(49, 0x80, 0xC0, 0xFF, 0);
    draw_queue_push(50, 90, 45, 50, 22);
    draw_queue_push(49, 0xFF, 0x80, 0x80, 0);
    draw_queue_push(50, 90, 45+22, 50, 23);
    draw_queue_push(48, 1, 0, 1, 0);
    draw_queue_push(49, 0xFF, 0xFF, 0xFF, 0);
    gly_type_render(95, 67-5, 9, "LOVE", draw_queue_clojure(51));
    draw_queue_push(51, 90+40, 45+2, 90+48, 45+2);
    draw_queue_push(51, 90+48, 45+2, 90+48, 45+6);
    draw_queue_push(50, 90, 45, 50, 45);
#elif ENGINE_LOGO == 3
    static const uint8_t ver_x = 107;
    static const uint8_t ver_y = 80;
    gly_type_render(95, 50, 5, "GameBoy", draw_queue_clojure(51));
    gly_type_render(95, 56, 4, "advanced", draw_queue_clojure(51));
    gly_type_render(95, 62, 10, "CORE", draw_queue_clojure(51));
    gly_type_render(95, 74, 5, "native", draw_queue_clojure(51));
#endif
    gly_type_render(ver_x, ver_y, 4, "0.0.17", draw_queue_clojure(51));
    draw_queue_burn(1);
#endif
}
