#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#define COLOR_ERASE 0
#define COLOR_TINT 1

// draw
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

extern union color_u color_tint;
extern union color_u color_erase;
extern union color_u color_current;

extern uint16_t draw_index_erase;
extern uint16_t draw_index_push;
extern uint8_t draw_queue[8192];

void native_draw_install(lua_State* L);
void native_draw_update_queue(lua_State* L, uint8_t page);
void native_draw_update_flush(uint8_t flushmode, uint8_t page);

// loop
void native_loop_update(lua_State* L, uint8_t dt);

// pad
void native_pad_update(lua_State *L);
