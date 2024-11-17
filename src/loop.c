#include "zeebo.h"

void loop_callback_update(lua_State* L, int callback, uint8_t dt)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
    lua_pushnumber(L, dt);
    lua_pcall(L, 1, 0, 0);
}
