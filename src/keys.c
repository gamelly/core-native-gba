#include "zeebo.h"

static uint16_t keys_current = 0;
static uint16_t keys_old = 0;

static const char *const key_bindings[] = {
    "a",
    "b",
    "menu",
    "menu",
    "right",
    "left",
    "up",
    "down",
    "c",
    "d"
};

static void key_update(lua_State* L, int callback, const char *const key, uint8_t value)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
    lua_pushstring(L, key);
    lua_pushinteger(L, value);
    lua_pcall(L, 2, 0, 0);
}

/**
 * @li 1 released
 * @li 0 pressing
 */
void keys_callback_update(lua_State* L, int callback)
{
    uint16_t mask = 1;
    uint8_t i = 0;
    keys_current = *(uint16_t*)(0x4000130);

    while (i < sizeof(key_bindings)) {
        if ((keys_current &~ keys_old) & mask) {
            key_update(L, callback, key_bindings[i], 0);
        }
        else if ((~keys_current &  keys_old) & mask) {
            key_update(L, callback, key_bindings[i], 1);
        }
        mask = mask << 1;
        i = i + 1;
    }

    keys_old = keys_current;
}
