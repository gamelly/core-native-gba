#include <gba.h>

#ifndef RGB15
#define RGB15(r, g, b) (((r) << 10) | ((g) << 5) | (b))
#endif

void drawSquare(int x, int y, int size, u16 color) {
    u16 *videoBuffer = (u16 *)VRAM;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            videoBuffer[(y + i) * 240 + (x + j)] = color;
        }
    }
}

int main() {
    SetMode(MODE_3 | BG2_ENABLE);

    
    u16 redColor = RGB15(31, 0, 0);

    drawSquare(50, 50, 40, redColor);

    while (1) {
        VBlankIntrWait();
    }

    return 0;
}
