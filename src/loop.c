#include "zeebo.h"

void loop_callback_update(lua_State* L, uint8_t dt)
{
    lua_getglobal(L, "native_callback_loop");
    lua_pushnumber(L, dt);
    lua_pcall(L, 1, 0, 0);
}
