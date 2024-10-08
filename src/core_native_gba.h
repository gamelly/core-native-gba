#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

void native_draw_install(lua_State* L);
void native_draw_update_queue(lua_State* L);
void native_draw_update_flush(uint8_t flushmode);

void native_loop_update(lua_State* L);

void native_pad_update(lua_State *L);
