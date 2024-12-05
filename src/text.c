#include "zeebo.h"
#include "font/gly_type_render.h"

static uint8_t previous_font_size = 5;
static uint8_t font_size = 5;

static int native_text_mock(lua_State *L)
{
    lua_settop(L, 0);
    return 0;
}

static int native_text_print(lua_State *L)
{
    uint8_t x = luaL_checknumber(L, 1);
    uint8_t y = luaL_checknumber(L, 2);
    const char* text = luaL_checkstring(L, 3);
    gly_type_render(x, y, font_size, text, draw_queue_clojure(51));
    lua_settop(L, 0);
    return 2;
}

static int native_text_mensure(lua_State *L)
{
    lua_settop(L, 0);
    lua_pushnumber(L, 1);
    lua_pushnumber(L, 1);
    return 2;
}

static int native_text_font_size(lua_State *L)
{
    previous_font_size = font_size;
    font_size = luaL_checknumber(L, 1) - 1;
    if (font_size < 3) {
        font_size = 3;
    }
    lua_settop(L, 0);
    return 0;
}

static int native_text_font_previous(lua_State *L)
{
    font_size = previous_font_size;
    lua_settop(L, 0);
    return 0;
}

void text_library_install(lua_State* L)
{
    int i = 0;
    static const luaL_Reg lib[] = {
        {"native_text_print", native_text_print},
        {"native_text_mensure", native_text_mensure},
        {"native_text_font_size", native_text_font_size},
        {"native_text_font_name", native_text_mock},
        {"native_text_font_default", native_text_mock},
        {"native_text_font_previous", native_text_font_previous},
    };

    while(i < sizeof(lib)/sizeof(luaL_Reg)) {
        lua_pushcfunction(L, lib[i].func);
        lua_setglobal(L, lib[i].name);
        i = i + 1;
    }
}
