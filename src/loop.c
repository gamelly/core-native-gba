#include "core_native_gba.h"

void native_loop_update(lua_State* L)
{
    lua_getglobal(L, "native_callback_loop");
    lua_pushnumber(L, 32);
    lua_pcall(L, 1, 0, 0);
}
