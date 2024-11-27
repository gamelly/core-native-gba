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

    luaL_loadbuffer(L, engine_bytecode_lua, engine_bytecode_lua_len, "");
	lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "native_callback_init");
    lua_pushnumber(L, 240);
    lua_pushnumber(L, 160);

    luaL_loadbuffer(L, game_bytecode_lua, game_bytecode_lua_len, "");
	lua_pcall(L, 0, 1, 0);
    lua_pcall(L, 3, 0, 0);


    lua_getglobal(L, "native_callback_loop");
    int native_callback_loop = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_getglobal(L, "native_callback_draw");
    int native_callback_draw = luaL_ref(L, LUA_REGISTRYINDEX);
    
    lua_getglobal(L, "native_callback_keyboard");
    int native_callback_keyboard = luaL_ref(L, LUA_REGISTRYINDEX);

    static uint8_t page = 0;
    static uint8_t frameskip = 0;
    
    while (1) {
        keys_callback_update(L, native_callback_keyboard);
        loop_callback_update(L, native_callback_loop, 16);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        if (++frameskip & 1) {
            draw_queue_page(page++);
            draw_callback_update(L, native_callback_draw);
        } else {
            draw_cmd_mode(0, 1, 0, 1);
            draw_queue_burn(page++);
            draw_cmd_mode(0, 0, 0, 1);
            draw_queue_burn(page++);
        }
    }
    return 0;
}
