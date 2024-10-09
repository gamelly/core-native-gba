#include "core_native_gba.h"

void draw_color(lua_State *L, union color_u *const colorptr)
{
    colorptr->pixel = luaL_checkinteger(L, 1);
    colorptr->pixel = (colorptr->c.r << 10 | colorptr->c.g << 5 | colorptr->c.b);
    colorptr->arr[1] = colorptr->pixel;
    lua_pop(L, 1);
}
