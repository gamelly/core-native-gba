#include <gba.h>

#include "engine/bytecode.h"
#include "game/bytecode.h"

#include "core_native_gba.h"

#ifndef GLY_GBA_FPSMODE
/**
 * @short fps configure
 * @brief lower FPS in heavier games ensures frame stability.
 * @details use flag @c -DFPS_MODE=0 in cmake to configure frameskip.
 * @li @b 0 loop 60 fps / draw 60 fps
 * @li @b 1 loop 60 fps / draw 30 fps
 * @li @b 2 loop 30 fps / draw 30 fps
 * @li @b 3 loop 20 fps / draw 20 fps
 */
#define GLY_GBA_FPSMODE 0
#endif

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

    uint8_t frameskip = 0;
    uint8_t page = 0;
    
    while (1) {
#if GLY_GBA_FPSMODE == 0
        native_pad_update(L);
        native_loop_update(L, 16);
        native_draw_update_queue(L, page++);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        native_draw_update_flush(COLOR_ERASE, page++);
        native_draw_update_flush(COLOR_TINT, page++);
#elif GLY_GBA_FPSMODE == 1
        native_pad_update(L);
        native_loop_update(L, 16);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        if (++frameskip & 1) {
            native_draw_update_queue(L, page++);
        } else {
            native_draw_update_flush(COLOR_ERASE, page++);
            native_draw_update_flush(COLOR_TINT, page++);
        }
#elif GLY_GBA_FPSMODE == 3
        if (++frameskip & 1) {
            native_pad_update(L);
            native_loop_update(L, 32);
            native_draw_update_queue(L, page++);
        }
        else {
            native_draw_update_flush(COLOR_ERASE, page++);
            native_draw_update_flush(COLOR_TINT, page++);
        }
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
#elif GLY_GBA_FPSMODE == 4
        if (++frameskip == 1) {
            native_pad_update(L);
            native_loop_update(L, 48);
        }
        else if (frameskip == 2) {
            native_draw_update_queue(L, page++);
        }
        else if (frameskip == 3) {
            native_draw_update_flush(COLOR_ERASE, page++);
            native_draw_update_flush(COLOR_TINT, page++);
            frameskip = 0;
        }
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
#endif
    }

    return 0;
}
