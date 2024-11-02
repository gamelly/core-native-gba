#include "core_native_gba.h"

static uint16_t index_cmd = 0;
static uint16_t page1_cmd = 0;
static uint16_t page2_cmd = 0;
static uint16_t index_param = 0;
static uint16_t page_current = 0;

static uint8_t queue_command[2048];
static uint8_t queue_param[8192];

void draw_queue_push(uint8_t cmd, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    if (index_cmd < sizeof(queue_command) && index_param + 4 <= sizeof(queue_param)) {
        queue_command[index_cmd++] = cmd;
        queue_param[index_param++] = a;
        queue_param[index_param++] = b;
        queue_param[index_param++] = c;
        queue_param[index_param++] = d;
    }
}

void draw_queue_page(uint8_t page)
{
    page_current = page & 1;
    if (page_current) {
        page1_cmd = index_cmd;
        index_cmd = sizeof(queue_command) / 2;
        index_param = sizeof(queue_param) / 2;
    } else {
        page2_cmd = index_cmd;
        index_cmd = 0;
        index_param = 0;
    }
}

void draw_queue_burn(uint8_t page)
{
    uint16_t index = page & 1? sizeof(queue_command)/2: 0;
    uint16_t param = index * 4;
    uint16_t final = index_cmd;

    if ((page & 1) != page_current) {
        final = index == 0? page1_cmd: page2_cmd;
    }

    static const void (*commands[])(uint8_t, uint8_t, uint8_t, uint8_t) = {
        draw_cmd_mode,
        draw_cmd_color,
        draw_cmd_rect,
        draw_cmd_line
    };

    while (index < final) {
        uint8_t cmd = queue_command[index] - 48;
        if (cmd < sizeof(commands) / sizeof(commands[0]) && commands[cmd] != NULL) {
            uint8_t param_1 = queue_param[param++];
            uint8_t param_2 = queue_param[param++];
            uint8_t param_3 = queue_param[param++];
            uint8_t param_4 = queue_param[param++];
            commands[cmd](param_1, param_2, param_3, param_4);
        }
        index++;
    }
}
