#include "zeebo.h"

#ifndef FPS_MODE
/**
 * @short fps configure
 * @brief lower FPS in heavier games ensures frame stability.
 * @details use flag @c -DFPS_MODE=2 in cmake to configure frameskip.
 * @li @b 0 loop 60 fps / draw 60 fps
 * @li @b 1 loop 60 fps / draw 30 fps
 * @li @b 2 loop 30 fps / draw 30 fps
 * @li @b 3 loop 20 fps / draw 20 fps
 * @li @b 4 loop 20 fps / draw 10 fps
 */
#define FPS_MODE 2
#endif

#ifndef FPS_DELTA
#if FPS_MODE <= 1
#define FPS_DELTA 16
#elif FPS_MODE <= 3
#define FPS_DELTA 33
#elif FPS_MODE <= 4
#define FPS_DELTA 50
#else
#error invalid FPS_MODE
#endif
#endif

void runtime(lua_State* L)
{
    lua_getglobal(L, "native_callback_loop");
    int native_callback_loop = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_getglobal(L, "native_callback_draw");
    int native_callback_draw = luaL_ref(L, LUA_REGISTRYINDEX);
    
    lua_getglobal(L, "native_callback_keyboard");
    int native_callback_keyboard = luaL_ref(L, LUA_REGISTRYINDEX);

    static uint8_t page = 0;
    static uint8_t frameskip = 0;
    
#if FPS_MODE == 0
    while (1) {
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        keys_callback_update(L, native_callback_keyboard);
        loop_callback_update(L, native_callback_loop, FPS_DELTA);
        draw_queue_page(page++);
        draw_callback_update(L, native_callback_draw);  
        while(*(volatile uint16_t*) 0x04000006 < 160);
        draw_cmd_mode(0, 1, 0, 1);
        draw_queue_burn(page++);
        draw_cmd_mode(0, 0, 0, 1);
        draw_queue_burn(page++);
    }
#elif FPS_MODE == 1
    while (1) {
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        keys_callback_update(L, native_callback_keyboard);
        loop_callback_update(L, native_callback_loop, FPS_DELTA);
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
#elif FPS_MODE == 2
    while (1) {
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        keys_callback_update(L, native_callback_keyboard);
        loop_callback_update(L, native_callback_loop, FPS_DELTA);
        draw_queue_page(page++);
        draw_callback_update(L, native_callback_draw);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        draw_cmd_mode(0, 1, 0, 1);
        draw_queue_burn(page++);
        draw_cmd_mode(0, 0, 0, 1);
        draw_queue_burn(page++);
    }
#elif FPS_MODE == 3
    while (1) {
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        keys_callback_update(L, native_callback_keyboard);
        loop_callback_update(L, native_callback_loop, FPS_DELTA);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        draw_queue_page(page++);
        draw_callback_update(L, native_callback_draw);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        draw_cmd_mode(0, 1, 0, 1);
        draw_queue_burn(page++);
        draw_cmd_mode(0, 0, 0, 1);
        draw_queue_burn(page++);  
    }
#elif FPS_MODE == 4
    while (1) {
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        keys_callback_update(L, native_callback_keyboard);
        loop_callback_update(L, native_callback_loop, FPS_DELTA);
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
        if (++frameskip & 1) {
            draw_queue_page(page++);
            draw_callback_update(L, native_callback_draw);
        }
        else {
            draw_cmd_mode(0, 1, 0, 1);
            draw_queue_burn(page++);
            draw_cmd_mode(0, 0, 0, 1);
            draw_queue_burn(page++);
        }
        while(*(volatile uint16_t*) 0x04000006 >= 160);
        while(*(volatile uint16_t*) 0x04000006 < 160);
    }
#else
#error invalid FPS_MODE
#endif
}
