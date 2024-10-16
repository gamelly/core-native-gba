#include "core_native_gba.h"

extern void erase_screen();
extern void draw_color(lua_State *L, union color_u *);
extern void text_font_size(lua_State *L);
extern void draw_queue_push(lua_State *L, uint8_t);
extern void text_font_size(lua_State *L);
extern void text_queue_push(lua_State *L, uint8_t);
extern void text_queue_clear();

union color_u color_tint = {0xFF};
union color_u color_erase = {0x0FF0};
union color_u color_current = {0x00FF};

static int native_draw_start(lua_State *L)
{
    text_queue_clear();
    return 0;
}

static int native_draw_flush(lua_State *L)
{
    return 0;
}

/**
 * @short @c std.draw.clear
 * @param[in] color @c int
 * @warning Changing the color that is clearing the screen is expensive,
 * it will take 2 to 3 frames, so avoid changing the color constantly.
 */
static int native_draw_clear(lua_State *L)
{
    static uint32_t old_color = 0xFACADA;
    draw_color(L, &color_erase);
    if (old_color != color_erase.pixel2) {
        old_color = color_erase.pixel2;
        erase_screen();
    }
    return 0;
}

/**
 * @short @c std.draw.color
 * @param[in] color @c int
 */
static int native_draw_color(lua_State *L)
{
    draw_color(L, &color_tint);
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
    draw_queue_push(L, 0);
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
    draw_queue_push(L, 1);
    return 0;
}

/**
 * @short @c std.draw.font
 * @param[in] font @c string
 * @param[in] size @c double
 * @warning long texts are currently expensive and impact vertical synchronization,
 * if you happen to experience flickering, change @b FPS_MODE to @b 3
 * or focus on smaller texts and at the bottom of the screen.
 */
static int native_draw_font(lua_State *L)
{
    text_font_size(L);
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
    text_queue_push(L, 2);
    return 2;
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
