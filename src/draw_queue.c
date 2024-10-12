#include "core_native_gba.h"

extern void native_draw_rect_flush();
extern void native_draw_line_flush();
extern void native_draw_text_flush();

uint16_t draw_index_erase = 0;
uint16_t draw_page1_index = 0;
uint16_t draw_page2_index = 0;
uint16_t draw_index_push = 0;
uint8_t draw_queue[8192];

void draw_queue_push(lua_State *L, uint8_t func) {
    uint8_t i = 1;
    uint8_t j = lua_gettop(L) + 1;

    draw_queue[draw_index_push] = func;

    while (i < j) {
        draw_queue[draw_index_push + i] = luaL_checknumber(L, i);
        i++;
    }

    draw_index_push += j;
    lua_pop(L, j);
}

/**
 * @li @b 0: clear page 1
 * @li @b 1: color page 1
 * @li @b 2: clear page 2
 * @li @b 3: color page 2
 * @li @b 4: erase page 1
 * @li @b 5: erase page 2
 */
void native_draw_update_flush(uint8_t flushmode, uint8_t page)
{
    if (page & 1) {
        draw_index_push = draw_page2_index;
        draw_index_erase = sizeof(draw_queue)/2;
    } else {
        draw_index_push = draw_page1_index;
        draw_index_erase = 0;
    }

    color_current.pixel2 = flushmode? color_tint.pixel2: color_erase.pixel2;

    static const void (*geometry_draw[])() = {
        native_draw_rect_flush,
        native_draw_line_flush,
        native_draw_text_flush
    };

    while(draw_index_erase < draw_index_push) {
        geometry_draw[draw_queue[draw_index_erase++]]();
    }  
}

void native_draw_update_queue(lua_State *L, uint8_t page)
{
    draw_index_push = page & 1? sizeof(draw_queue)/2: 0;
    lua_getglobal(L, "native_callback_draw");
    lua_pcall(L, 0, 0, 0);
    if (page & 1) {
        draw_page2_index = draw_index_push;
    } else {
        draw_page1_index = draw_index_push;
    }
}
