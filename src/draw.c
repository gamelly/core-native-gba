#include "zeebo.h"

static int native_draw_start(lua_State *L)
{
    return 0;
}

static int native_draw_flush(lua_State *L)
{
    return 0;
}

/**
 * @param[in] color @c int
 */
static int native_draw_clear(lua_State *L)
{
    color_t t = { .pixel2 = luaL_checkinteger(L, 1) };
    draw_queue_push(48, 3, 0, 1, 0);
    draw_queue_push(49, t.c32.color.r, t.c32.color.g, t.c32.color.b, 0);
    lua_settop(L, 0);
    return 0;
}

/**
 * @param[in] color @c int
 */
static int native_draw_color(lua_State *L)
{
    color_t t = { .pixel2 = luaL_checkinteger(L, 1) };
    draw_queue_push(48, 0, 0, 1, 0);
    draw_queue_push(49, t.c32.color.r, t.c32.color.g, t.c32.color.b, 0);
    lua_settop(L, 0);
    return 0;
}

/**
 * @param[in] mode @c int
 * @param[in] x @c double
 * @param[in] y @c double
 * @param[in] width @c double
 * @param[in] height @c double
 */
static int native_draw_rect(lua_State *L)
{
    uint8_t mode = luaL_checkinteger(L, 1);
    int32_t posx = luaL_checknumber(L, 2);
    int32_t posy = luaL_checknumber(L, 3);
    int32_t width = luaL_checknumber(L, 4);
    int32_t height = luaL_checknumber(L, 5);

    if (geoclip_rect(&posx, &posy, &width, &height)) {
        draw_queue_push(48, mode, 0, 1, 0);
        draw_queue_push(50, (uint8_t) posx, (uint8_t) posy, (uint8_t) width, (uint8_t) height);
    }

    lua_settop(L, 0);
    return 0;
}

/**
 * @param[in] x1 @c double
 * @param[in] y1 @c double
 * @param[in] x2 @c double
 * @param[in] y2 @c double
 */
static int native_draw_line(lua_State *L)
{
    int32_t x1 = luaL_checknumber(L, 1);
    int32_t y1 = luaL_checknumber(L, 2);
    int32_t x2 = luaL_checknumber(L, 3);
    int32_t y2 = luaL_checknumber(L, 4);

    if (geoclip_line(&x1, &y1, &x2, &y2)) {
        draw_queue_push(51, (uint8_t) x1, (uint8_t) y1, (uint8_t) x2, (uint8_t) y2);
    }

    lua_settop(L, 0);
    return 0;
}


/**
 * @param[in] source @c string
 * @param[in] x @c double
 * @param[in] y @c double
 */
static int native_draw_image(lua_State *L) {
    uint8_t x = luaL_checknumber(L, 2);
    uint8_t y = luaL_checknumber(L, 3);
    draw_queue_push(48, 0, 0, 1, 0);
    draw_queue_push(49, 0XFF, 0xFF, 0xFF, 0);
    draw_queue_push(50, x, y, 1, 1);
    lua_settop(L, 0);
    return 0;
}

void draw_callback_update(lua_State* L, int callback)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
    lua_pcall(L, 0, 0, 0);
}

void draw_library_install(lua_State* L)
{
    int i = 0;
    static const luaL_Reg lib[] = {
        {"native_draw_start", native_draw_start},
        {"native_draw_flush", native_draw_flush},
        {"native_draw_clear", native_draw_clear},
        {"native_draw_color", native_draw_color},
        {"native_draw_rect", native_draw_rect},
        {"native_draw_line", native_draw_line},
        {"native_draw_image", native_draw_image}
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
