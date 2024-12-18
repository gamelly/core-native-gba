#include "engine/bytecode.h"
#include "game/bytecode.h"

#include "zeebo.h"

/**
 * @li @b REG_DISPCNT @c 0x4000000
 * @li @b REG_VCOUNT @c 0x4000006
 * @li @b MODE_3 @c 0x3
 * @li @b BG2_ENABLE @c 0x400
 */
int main()
{
    (*(volatile uint32_t*)0x4000000) = 0x3 | 0x400;

    draw_logo();

    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);
    text_library_install(L);
    draw_library_install(L);

    luaL_loadbuffer(L, engine_bytecode_lua, engine_bytecode_lua_len, "E");
	lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "native_callback_init");
    lua_pushnumber(L, 240);
    lua_pushnumber(L, 160);

    luaL_loadbuffer(L, game_bytecode_lua, game_bytecode_lua_len, "G");
	lua_pcall(L, 0, 1, 0);
    lua_pcall(L, 3, 0, 0);

    runtime(L);
    return 0;
}
