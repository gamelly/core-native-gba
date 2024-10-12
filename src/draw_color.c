#include "core_native_gba.h"

void draw_color(lua_State *L, union color_u *const colorptr)
{
    colorptr->pixel2 = luaL_checkinteger(L, 1);
    colorptr->c.a = 0;
    colorptr->pixel = (colorptr->c.r | colorptr->c.g << 5 | colorptr->c.b << 10);
    colorptr->arr[1] = colorptr->pixel;
    lua_pop(L, 1);
}

void erase_screen()
{
    static const uint32_t *end = (uint32_t *) (0x06000000 + (240 * 160 * 2));
    uint32_t *i = (uint32_t *) 0x06000000;

    while (i < end) {
        *i++ = color_erase.pixel2;
    }
}
