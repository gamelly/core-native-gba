#include "core_native_gba.h"

static uint16_t keys_current = 0;
static uint16_t keys_old = 0;

static const char *const key_bindings[] = {
    "red",
    "green",
    "menu",
    "menu",
    "left",
    "right",
    "down",
    "up",
    "yellow",
    "blue"
};

static void key_update(lua_State* L, const char *const key, int value)
{
    lua_getglobal(L, "native_callback_keyboard");
    lua_pushstring(L, key);
    lua_pushinteger(L, value);
    lua_pcall(L, 2, 0, 0);
}

void native_pad_update(lua_State* L)
{
    uint16_t mask = 1;
    uint8_t i = 0;
    keys_current = *(uint16_t*)(0x4000130);

    while (i < sizeof(key_bindings)) {
        if ((keys_current &~ keys_old) & mask) {
            key_update(L, key_bindings[i], 1);
        }
        else if ((~keys_current &  keys_old) & mask) {
            key_update(L, key_bindings[i], 0);
        }
        mask = mask << 1;
        i = i + 1;
    }

    keys_old = keys_current;
}
