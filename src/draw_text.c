#include "core_native_gba.h"
#include "font/gly_type_render.h"

static uint8_t text_page1_index = 0;
static uint8_t text_page2_index = 0;
static const char* texts_page1[20];
static const char* texts_page2[20];

void native_draw_line_func(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void text_queue_push(lua_State *L, uint8_t func) {
    const char* text = NULL;

    if (lua_gettop(L) == 3) {
        uint8_t x = (uint8_t) luaL_checknumber(L, 1);
        uint8_t y = (uint8_t) luaL_checknumber(L, 2);
        text = luaL_checkstring(L, 3);
        draw_queue[draw_index_push++] = func;
        draw_queue[draw_index_push++] = x;
        draw_queue[draw_index_push++] = y;
        if (draw_index_push < sizeof(draw_queue)/2) {
            draw_queue[draw_index_push++] = text_page1_index;
            texts_page1[text_page1_index++] = text;
        } else {
            draw_queue[draw_index_push++] = text_page2_index;
            texts_page2[text_page2_index++] = text;
        }
        lua_pop(L, 3);
    }
    else {
        text = luaL_checkstring(L, 1);
        lua_pop(L, 1);
    }

    if (text_page1_index >= 19 || text_page2_index >= 19) {
        text_page1_index = 0;
        text_page2_index = 0;
    }

    lua_pushinteger(L, 1);
    lua_pushinteger(L, 1);
}

void text_queue_clear()
{
    text_page2_index = 0;
    text_page2_index = 0;
}

void native_draw_text_flush()
{
    uint8_t x = draw_queue[draw_index_erase++];
    uint8_t y = draw_queue[draw_index_erase++];
    const char* text = NULL;
    if (draw_index_erase < sizeof(draw_queue)/2) {
        text = texts_page1[draw_queue[draw_index_erase++]];
    } else {
        text = texts_page2[draw_queue[draw_index_erase++]];
    }
    gly_type_render(x, y, 5, text, native_draw_line_func);
}
