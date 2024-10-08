#include <gba.h>

#include "engine/bytecode.h"
#include "game/bytecode.h"

#include "core_native_gba.h"

int main()
{
    SetMode(MODE_3 | BG2_ENABLE);
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);
    native_draw_install(L);

    luaL_loadbuffer(L, engine_bytecode_lua, engine_bytecode_lua_len, "");
	lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "native_callback_init");
    lua_pushnumber(L, 240);
    lua_pushnumber(L, 160);

    luaL_loadbuffer(L, game_bytecode_lua, game_bytecode_lua_len, "");
	lua_pcall(L, 0, 1, 0);
    lua_pcall(L, 3, 0, 0);

    while (1) {
        native_pad_update(L);
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
