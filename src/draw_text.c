#include "core_native_gba.h"
#include "font/gly_type_render.h"

static uint8_t text_index = 0;
static const char* texts[20] = {};

void native_draw_line_func(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void text_queue_push(lua_State *L, uint8_t func) {
    const char* text = NULL;

    if (lua_gettop(L) == 3) {
        uint8_t x = (uint8_t) luaL_checknumber(L, 1);
        uint8_t y = (uint8_t) luaL_checknumber(L, 2);
        text = luaL_checkstring(L, 3);
        texts[text_index] = text;
        draw_queue[draw_index_push++] = func;
        draw_queue[draw_index_push++] = x;
        draw_queue[draw_index_push++] = y;
        draw_queue[draw_index_push++] = text_index++;
        lua_pop(L, 3);
    }
    else {
        text = luaL_checkstring(L, 1);
        lua_pop(L, 1);
    }

    if (text_index >= sizeof(texts[0])/sizeof(texts)) {
        text_index = 0;
    }

    lua_pushinteger(L, 1);
    lua_pushinteger(L, 1);
}

void text_queue_clear()
{
    text_index = 0;
}

void native_draw_text_flush()
{
    uint8_t x = draw_queue[draw_index_erase++];
    uint8_t y = draw_queue[draw_index_erase++];
    const char* text = texts[draw_queue[draw_index_erase++]];
    gly_type_render(x, y, 5, "hello world", native_draw_line_func);
}
