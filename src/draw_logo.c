#include "zeebo.h"

#ifndef ENGINE_LOGO
#define ENGINE_LOGO 1
#endif

extern void gly_type_render(uint8_t, uint8_t, uint8_t, char*, void*);

void draw_logo()
{
#if ENGINE_LOGO != 0
    draw_queue_page(1);
    draw_queue_push(49, 0xFF, 0xFF, 0xFF, 0);
    draw_queue_push(48, 3, 0, 1, 0);
    draw_queue_push(49, 0, 0, 0, 0);
#if ENGINE_LOGO == 1
    draw_queue_push(50, 90, 45, 50, 45);
    gly_type_render(95, 60, 12, "gly", draw_queue_clojure(51));
#else
    gly_type_render(95, 67, 5, "native", draw_queue_clojure(51));
#endif
    gly_type_render(95, 74, 5, "engine", draw_queue_clojure(51));
    gly_type_render(97, 80, 4, "0.0.12", draw_queue_clojure(51));
    draw_queue_burn(1);
#endif
}
