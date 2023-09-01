#include <graphx.h>
#include <tice.h>
#include <debug.h>
#include <time.h>

#include "gfx/global_palette.h"

static constexpr uint16_t SCREEN_WIDTH = 160, SCREEN_HEIGHT = 120;

typedef uint16_t upos;
typedef uint8_t parttype_t;

struct Particle {
    uint8_t type : 7;
    bool movedParity : 1;
};

Particle* grid = new Particle[SCREEN_WIDTH * SCREEN_HEIGHT] {};

bool globalParity = 0;

void set_part(upos x, upos y, parttype_t type) {
    grid[y * SCREEN_WIDTH + x] = { type, globalParity };
}

void set_part_sim(upos x, upos y, parttype_t type) {
    grid[y * SCREEN_WIDTH + x] = { type, !globalParity };
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
    clock_t cock = clock(); // CLOck Count (K)onstant

    for (int x = 0, y = 0; y < SCREEN_HEIGHT; x++) {
        if (x >= SCREEN_WIDTH) {
            x = 0;
            y++;
        }

        Particle part = get_part(x, y);

        // Ignore empty grid spaces
        if (part.type == 0)
            continue;

        // Don't move the particle if already at the bottom of the sim
        if ((y + 1) >= SCREEN_HEIGHT)
            continue;

        // Do not move if already moved based on parity bit
        if (globalParity != part.movedParity)
            continue;
        
        if (get_part(x, y + 1).type == 0) // Down
            set_part_sim(x, y + 1, part.type);
        else if (get_part(x + 1, y + 1).type == 0) // Down right
            set_part_sim(x + 1, y + 1, part.type);
        else if (get_part(x - 1, y + 1).type == 0) // Down left
            set_part_sim(x - 1, y + 1, part.type);
        else // Do not move
            continue;

        set_part_sim(x, y, 0);
    }

    globalParity = !globalParity;
    
    dbg_printf("Time taken: %f\n", ((double)(clock() - cock)) / CLOCKS_PER_SEC);
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

    simulate_once();
    init_sim();

    for (int i = 0; i < 1000; i++) {
    grid[i] = { 1, globalParity };
    }

    for (int i = 0; i < 1000; i++) {
        simulate_once();
        render_sim();
    }

    // sleep(1);
    os_GetKey();

    // Graphics shutdown
    gfx_End();
}