#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

#include "engine/bytecode.h"
#include "game/bytecode.h"

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#ifndef RGB15
#define RGB15(r, g, b) (((r) << 10) | ((g) << 5) | (b))
#endif

unsigned short rgba32_to_rgb15(unsigned int rgba) {
    unsigned char r = (rgba >> 24) & 0xFF;
    unsigned char g = (rgba >> 16) & 0xFF;
    unsigned char b = (rgba >> 8) & 0xFF;

    unsigned short rgb15_r = (r >> 3) & 0x1F;
    unsigned short rgb15_g = (g >> 3) & 0x1F;
    unsigned short rgb15_b = (b >> 3) & 0x1F;

    return RGB15(rgb15_r, rgb15_g, rgb15_b);
}

static int native_draw_clear(lua_State *L)
{
    return 0;
}

/**
 * @param[in] mode @c int 0 fill, 1 frame
 * @param[in] x @c double pos X
 * @param[in] y @c double pos Y
 * @param[in] w @c double width
 * @param[in] h @c double height
 */
static int native_draw_rect(lua_State *L)
{
    u16 *videoBuffer = (u16 *)VRAM;
    short x = luaL_checknumber(L, 2);
    short y = luaL_checknumber(L, 3);
    short w = luaL_checknumber(L, 4);
    short h = luaL_checknumber(L, 5);
    short color = RGB15(31, 0, 0);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            videoBuffer[(y + i) * 240 + (x + j)] = color;
        }
    }

    return 0;
}

int main()
{
    SetMode(MODE_3 | BG2_ENABLE);
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, native_draw_rect);
    lua_setglobal(L, "native_draw_rect");
    lua_pushcfunction(L, native_draw_clear);
    lua_setglobal(L, "native_draw_clear");
    lua_pushcfunction(L, native_draw_clear);
    lua_setglobal(L, "native_draw_color");
    lua_pushcfunction(L, native_draw_clear);
    lua_setglobal(L, "native_draw_start");
    lua_pushcfunction(L, native_draw_clear);
    lua_setglobal(L, "native_draw_flush");
    lua_pushcfunction(L, native_draw_clear);
    lua_setglobal(L, "native_draw_text");
    lua_pushcfunction(L, native_draw_clear);
    lua_setglobal(L, "native_draw_font");

    luaL_loadbuffer(L, engine_bytecode_lua, engine_bytecode_lua_len, "");
	lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "native_callback_init");
    lua_pushnumber(L, 240);
    lua_pushnumber(L, 160);

    luaL_loadbuffer(L, game_bytecode_lua, game_bytecode_lua_len, "");
	lua_pcall(L, 0, 1, 0);
    lua_pcall(L, 3, 0, 0);

    while (1) {
        lua_getglobal(L, "native_callback_loop");
        lua_pushnumber(L, 16);
        lua_pcall(L, 1, 0, 0);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        lua_getglobal(L, "native_callback_draw");
        lua_pcall(L, 0, 0, 0);
    }

    return 0;
}
