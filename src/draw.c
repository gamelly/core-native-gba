#include "core_native_gba.h"

union color_u color_tint = {0xFF};
union color_u color_erase = {0x0FF0};
union color_u color_current = {0x00FF};

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
    draw_queue_push(48, 3, 0, 1, 0);
    draw_queue_push(49, 0, 0, 0, 0);
    lua_settop(L, 0);
    return 0;
}

static int native_draw_color(lua_State *L)
{
    draw_queue_push(48, 0, 0, 1, 0);
    draw_queue_push(49, 0xFF, 0xFF, 0xFF, 0xFF);
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
    lua_settop(L, 0);
    return 0;
}

static int native_draw_text(lua_State *L)
{
    lua_settop(L, 2);
    return 2;
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
        {"native_draw_text", native_draw_text}
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
