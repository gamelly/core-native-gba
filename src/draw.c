#include "core_native_gba.h"

union {
    uint16_t pixel;
    uint32_t pixel2;
    uint16_t arr[2];
} color_current = {0};

union {
    uint32_t pixel;
    struct {
        uint8_t a;
        uint8_t b;
        uint8_t g;
        uint8_t r;
    } c;
} color_conversor = {0};

static int native_draw_start(lua_State *L)
{
    return 0;
}

static int native_draw_flush(lua_State *L)
{
    return 0;
}
/**
 * @short @c std.draw.color
 * @param[in] color @c int
 */
static int native_draw_color(lua_State *L) {
    color_conversor.pixel = luaL_checkinteger(L, 1);
    color_current.pixel = (color_conversor.c.r << 10 | color_conversor.c.g << 5 | color_conversor.c.b);
    color_current.arr[1] = color_current.pixel;
    lua_pop(L, 1);
    return 0;
}

/**
 * @short @c std.draw.clear
 * @param[in] color @c int
 */
static int native_draw_clear(lua_State *L)
{
    native_draw_color(L);

    static const uint32_t *end = (uint32_t *) (0x06000000 + (240 * 160 * 2));
    uint32_t *i = (uint32_t *) 0x06000000;

    while (i < end) {
        *i++ = color_current.pixel2;
    }

    return 0;
}

/**
 * @short @c std.draw.rect
 * @param[in] mode @c int 0 fill, 1 frame
 * @param[in] x @c double pos X
 * @param[in] y @c double pos Y
 * @param[in] w @c double width
 * @param[in] h @c double height
 */
static int native_draw_rect(lua_State *L)
{
    short x = luaL_checknumber(L, 2);
    short y = luaL_checknumber(L, 3);
    short w = luaL_checknumber(L, 4);
    short h = luaL_checknumber(L, 5);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            *(uint16_t *)(0x06000000 + ((y + i) * 240 + (x + j)) * 2) = color_current.pixel;
        }
    }

    lua_pop(L, 5);

    return 0;
}

/**
 * @short @c std.draw.line
 * @param[in] x1 @c double
 * @param[in] y1 @c double
 * @param[in] x2 @c double
 * @param[in] y2 @c double
 */
static int native_draw_line(lua_State *L)
{
    return 0;
}

/**
 * @short @c std.draw.font
 * @param[in] font @c string
 * @param[in] size @c double
 */
static int native_draw_font(lua_State *L)
{
   return 0;
}

/**
 * @short @c std.draw.text
 * @param[in] x @c double
 * @param[in] y @c double
 * @param[in] text @c string
 */
static int native_draw_text(lua_State *L)
{
   return 0;
}

void native_draw_install(lua_State* L)
{
    int i = 0;
    static const luaL_Reg lib[] = {
        {"native_draw_start", native_draw_start},
        {"native_draw_flush", native_draw_flush},
        {"native_draw_clear", native_draw_clear},
        {"native_draw_color", native_draw_color},
        {"native_draw_rect", native_draw_rect},
        {"native_draw_line", native_draw_line},
        {"native_draw_font", native_draw_font},
        {"native_draw_text", native_draw_text}
    };

    while(i < sizeof(lib)/sizeof(luaL_Reg)) {
        lua_pushcfunction(L, lib[i].func);
        lua_setglobal(L, lib[i].name);
        i = i + 1;
    }

    lua_newtable(L);
    lua_pushstring(L, "repeats");
    lua_newtable(L);
    lua_pushboolean(L, 1);
    lua_seti(L, -2, 1);
    lua_pushboolean(L, 1);
    lua_seti(L, -2, 2);
    lua_settable(L, -3);
    lua_pushstring(L, "line");
    lua_pushcfunction(L, native_draw_line);
    lua_settable(L, -3); 
    lua_setglobal(L, "native_dict_poly");
}
