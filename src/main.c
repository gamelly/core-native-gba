#include "engine/bytecode.h"
#include "game/bytecode.h"

#include "zeebo.h"
#include <nds.h>

int main()
{
    powerOn(POWER_ALL);
    videoSetMode(MODE_FB0);
    vramSetBankA(VRAM_A_LCD);

    draw_logo();

    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);
    text_library_install(L);
    draw_library_install(L);

    luaL_loadbuffer(L, engine_bytecode_lua, engine_bytecode_lua_len, "E");
	lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "native_callback_init");
    lua_pushnumber(L, 256);
    lua_pushnumber(L, 192);

    luaL_loadbuffer(L, game_bytecode_lua, game_bytecode_lua_len, "G");
	lua_pcall(L, 0, 1, 0);
    lua_pcall(L, 3, 0, 0);

    runtime(L);
    return 0;
}
