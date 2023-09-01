#include <graphx.h>
#include <tice.h>
#include <debug.h>

#include "gfx/global_palette.h"

static constexpr uint16_t SCREEN_WIDTH = 160, SCREEN_HEIGHT = 120;

typedef uint16_t upos;
typedef uint8_t parttype_t;

struct Particle {
    uint8_t type;
};

Particle grid[SCREEN_WIDTH * SCREEN_HEIGHT];
Particle gridBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void set_part(upos x, upos y, parttype_t type) {
    grid[y * SCREEN_WIDTH + x] = { type };
}

Particle get_part(upos x, upos y) {
    return grid[y * SCREEN_WIDTH + x];
}

void init_sim() {
    for (int i = 0; i < 10; i++) {
        set_part(i * 2 + 20, 20, 1);
    }
}

void draw_pixel(uint8_t x, uint8_t y, uint8_t mul) {
    gfx_FillRectangle(x * mul, y * mul, mul, mul);
}

void render_sim() {
    gfx_FillScreen(0);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        Particle part = grid[i];
        if (part.type > 0) {
            gfx_SetColor(4);
            draw_pixel(i % SCREEN_WIDTH, i / SCREEN_WIDTH, 2);
        }
    }
    gfx_BlitBuffer();
}

void simulate_once() {
    
}

int main() {

    // Graphics initialization
    gfx_Begin();

    gfx_SetDrawBuffer();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(2);

    // Main
    dbg_printf("INIT\n");
    gfx_FillScreen(0);
    gfx_BlitBuffer();

    init_sim();
    render_sim();

    // sleep(1);
    os_GetKey();

    // Graphics shutdown
    gfx_End();
}