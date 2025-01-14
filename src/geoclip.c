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

    if ((*posx + *width) > 256) {
        *width = 256 - *posx;
    }

    if ((*posy + *height) > 192) {
        *height = 192 - *posy;
    }

    if (*width < 0) {
        *width = 0;
    }

    if (*height < 0) {
        *height = 0;
    }

    return (*height > 0) && (*width > 0);
}

/** 
 * @todo use cohen, liang-barsky, clipping de retA, cos, ou another method do clip drop line.
 * @brief adjusts a line within the screen
 * @param [in,out] x1
 * @param [in,out] y1
 * @param [in,out] x2
 * @param [in,out] y2
 * @return fits on the screen
 */
uint8_t geoclip_line(int32_t *x1, int32_t *y1, int32_t *x2, int32_t *y2)
{
    return 1;
}
