#include "core_native_gba.h"

extern void native_draw_rect_flush();
extern void native_draw_line_flush();
extern void native_draw_text_flush();

uint16_t draw_index_erase = 0;
uint16_t draw_index_push = 0;
uint16_t draw_queue[4096];

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

void native_draw_update_flush(uint8_t flushmode)
{
    uint8_t func = 0;
    draw_index_erase = 0;
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

void native_draw_update_queue(lua_State *L)
{
    draw_index_push = 0;
    lua_getglobal(L, "native_callback_draw");
    lua_pcall(L, 0, 0, 0);
}
