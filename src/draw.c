#include "core_native_gba.h"

union color_u {
    uint16_t pixel;
    uint32_t pixel2;
    uint16_t arr[2];
    struct {
        uint8_t a;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } c;
};

static uint16_t draw_index_erase = 0;
static uint16_t draw_index_push = 0;
static uint16_t draw_queue[4096];

static union color_u color_tint = {0};
static union color_u color_erase = {0};

static int native_draw_start(lua_State *L)
{
    return 0;
}

static int native_draw_flush(lua_State *L)
{
    return 0;
}

/**
 * @short @c std.draw.clear
 * @param[in] color @c int
 */
static int native_draw_clear(lua_State *L)
{
    color_erase.pixel = luaL_checkinteger(L, 1);
    color_erase.pixel = (color_erase.c.r << 10 | color_erase.c.g << 5 | color_erase.c.b);
    color_erase.arr[1] = color_erase.pixel;
    lua_pop(L, 1);
    return 0;
}

/**
 * @short @c std.draw.color
 * @param[in] color @c int
 */
static int native_draw_color(lua_State *L) {
    color_tint.pixel = luaL_checkinteger(L, 1);
    color_tint.pixel = (color_tint.c.r << 10 | color_tint.c.g << 5 | color_tint.c.b);
    color_tint.arr[1] = color_tint.pixel;
    lua_pop(L, 1);
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
    uint8_t i = 0;
    while (i < 5) {
        draw_queue[draw_index_push + i] = luaL_checknumber(L, i + 1);
        i++;
    }
    draw_index_push += 5;

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

void native_draw_update_flush(uint8_t flushmode)
{
    draw_index_erase = 0;
    uint16_t color = flushmode? color_tint.pixel: color_erase.pixel;

    while(draw_index_erase < draw_index_push) {
        draw_index_erase++;
        uint16_t x = draw_queue[draw_index_erase++];
        uint16_t y = draw_queue[draw_index_erase++];
        uint16_t w = draw_queue[draw_index_erase++];
        uint16_t h = draw_queue[draw_index_erase++];

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                *(uint16_t *)(0x06000000 + ((y + i) * 240 + (x + j)) * 2) = color;
            }
        }
    }  
}

void native_draw_update_queue(lua_State *L)
{
    draw_index_push = 0;
    lua_getglobal(L, "native_callback_draw");
    lua_pcall(L, 0, 0, 0);
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
