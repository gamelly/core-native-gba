#include "zeebo.h"

/**
 * @brief adjusts a rectangle within the screen
 * @param [in,out] posx
 * @param [in,out] posy
 * @param [in,out] width
 * @param [in,out] height
 * @return fits on the screen
 */
uint8_t geoclip_rect(int32_t *posx, int32_t *posy, int32_t *width, int32_t *height)
{
    if (*posx < 0) {
        *width += *posx;
        *posx = 0;
    }

    if (*posy < 0) {
        *height += *posy;
        *posy = 0;
    }

    if ((*posx + *width) > 240) {
        *width = 240 - *posx;
    }

    if ((*posy + *height) > 160) {
        *height = 160 - *posy;
    }

    if (*width < 0) {
        *width = 0;
    }

    if (*height < 0) {
        *height = 0;
    }

    return (*height > 0) && (*width > 0);
}

