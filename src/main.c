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
    draw_library_install(L);

    luaL_loadbuffer(L, engine_bytecode_lua, engine_bytecode_lua_len, "");
	lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "native_callback_init");
    lua_pushnumber(L, 240);
    lua_pushnumber(L, 160);

    luaL_loadbuffer(L, game_bytecode_lua, game_bytecode_lua_len, "");
	lua_pcall(L, 0, 1, 0);
    lua_pcall(L, 3, 0, 0);

    static uint8_t page = 0;
    
    while (1) {
        native_pad_update(L);
        native_loop_update(L, 16);
        draw_queue_page(page++);
        draw_callback_update(L);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        draw_cmd_mode(0, 1, 0, 1);
        draw_queue_burn(page++);
        draw_cmd_mode(0, 0, 0, 1);
        draw_queue_burn(page++);
    }
    return 0;
}
