#include "zeebo.h"
#include "font/gly_type_render.h"

static uint8_t font_size = 0;

static int native_draw_start(lua_State *L)
{
    return 0;
}

static int native_draw_flush(lua_State *L)
{
    return 0;
}

static int native_draw_clear(lua_State *L)
{
    color_t t = { .pixel2 = luaL_checkinteger(L, 1) };
    draw_queue_push(48, 3, 0, 1, 0);
    draw_queue_push(49, t.c32.color.r, t.c32.color.g, t.c32.color.b, 0);
    lua_settop(L, 0);
    return 0;
}

static int native_draw_color(lua_State *L)
{
    color_t t = { .pixel2 = luaL_checkinteger(L, 1) };
    draw_queue_push(48, 0, 0, 1, 0);
    draw_queue_push(49, t.c32.color.r, t.c32.color.g, t.c32.color.b, 0);
    lua_settop(L, 0);
    return 0;
}

static int native_draw_rect(lua_State *L)
{
    uint8_t mode = luaL_checkinteger(L, 1);
    uint8_t posx = luaL_checknumber(L, 2);
    uint8_t posy = luaL_checknumber(L, 3);
    uint8_t width = luaL_checknumber(L, 4);
    uint8_t height = luaL_checknumber(L, 5);
    draw_queue_push(48, mode, 0, 1, 0);
    draw_queue_push(50, posx, posy, width, height);
    lua_settop(L, 0);
    return 0;
}

static int native_draw_line(lua_State *L)
{
    uint8_t x1 = luaL_checknumber(L, 1);
    uint8_t y1 = luaL_checknumber(L, 2);
    uint8_t x2 = luaL_checknumber(L, 3);
    uint8_t y2 = luaL_checknumber(L, 4);
    draw_queue_push(51, x1, y1, x2, y2);
    lua_settop(L, 0);
    return 0;
}

static int native_draw_font(lua_State *L)
{
    if (lua_gettop(L) == 2) {
        font_size = luaL_checknumber(L, 2);
    } else {
        font_size = luaL_checknumber(L, 1);
    }
    lua_settop(L, 0);
    return 0;
}

static int native_draw_text(lua_State *L)
{
    uint8_t x = luaL_checknumber(L, 1);
    uint8_t y = luaL_checknumber(L, 2);
    const char* text = luaL_checkstring(L, 3);
    gly_type_render(x, y, font_size, text, draw_queue_clojure(51));
    lua_settop(L, 0);
    lua_pushnumber(L, 1);
    lua_pushnumber(L, 1);
    return 2;
}

static int native_draw_text_tui(lua_State *L) {
    uint8_t x = luaL_checknumber(L, 1);
    uint8_t y = luaL_checknumber(L, 2);
    uint8_t s = luaL_checknumber(L, 7);
    const char* text = luaL_checkstring(L, 8);
    gly_type_render(x * 3, y * 3, s * 5, text, draw_queue_clojure(51));
    lua_settop(L, 0);
    return 0;
}

void draw_callback_update(lua_State* L)
{
    lua_getglobal(L, "native_callback_draw");
    lua_pcall(L, 0, 0, 0);
}

void draw_library_install(lua_State* L)
{
    int i = 0;
    static const luaL_Reg lib[] = {
        {"native_draw_start", native_draw_start},
        {"native_draw_flush", native_draw_flush},
        {"native_draw_clear", native_draw_clear},
        {"native_draw_color", native_draw_color},
        {"native_draw_rect", native_draw_rect},
        {"native_draw_line", native_draw_line},
        {"native_draw_font", native_draw_font},
        {"native_draw_text", native_draw_text},
        {"native_draw_text_tui", native_draw_text_tui},
    };

    while(i < sizeof(lib)/sizeof(luaL_Reg)) {
        lua_pushcfunction(L, lib[i].func);
        lua_setglobal(L, lib[i].name);
        i = i + 1;
    }

    lua_newtable(L);
    lua_pushstring(L, "repeats");
    lua_newtable(L);
    lua_pushboolean(L, 1);
    lua_seti(L, -2, 1);
    lua_pushboolean(L, 1);
    lua_seti(L, -2, 2);
    lua_settable(L, -3);
    lua_pushstring(L, "line");
    lua_pushcfunction(L, native_draw_line);
    lua_settable(L, -3); 
    lua_setglobal(L, "native_dict_poly");
}
