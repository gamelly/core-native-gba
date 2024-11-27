#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#define COLOR_ERASE 0
#define COLOR_TINT 1

typedef void (*cmd_t)(uint8_t, uint8_t, uint8_t, uint8_t);

typedef union {
    uint16_t pixel;
    uint32_t pixel2;
    uint16_t arr[2];
    struct {
        struct {
            uint16_t b: 5; 
            uint16_t g: 5; 
            uint16_t r: 5;
            uint16_t a: 1;
        } color;
    } c16;
    struct {
        struct {
            uint8_t a;
            uint8_t b;
            uint8_t g;
            uint8_t r;
        } color;
    } c32;
} color_t;

extern uint8_t draw_mode;
extern color_t draw_color;

void draw_logo();

void draw_cmd_mode(uint8_t, uint8_t, uint8_t, uint8_t);
void draw_cmd_color(uint8_t, uint8_t, uint8_t, uint8_t);
void draw_cmd_rect(uint8_t, uint8_t, uint8_t, uint8_t);
void draw_cmd_line(uint8_t, uint8_t, uint8_t, uint8_t);
void draw_queue_burn(uint8_t page);
void draw_queue_page(uint8_t page);
void draw_queue_push(uint8_t cmd, uint8_t a, uint8_t b, uint8_t c, uint8_t d);
cmd_t draw_queue_clojure(uint8_t cmd);

void text_library_install(lua_State* L);
void draw_library_install(lua_State* L);
void draw_callback_update(lua_State* L, int);
void keys_callback_update(lua_State *L, int);
void loop_callback_update(lua_State* L, int, uint8_t dt);
