#include <graphx.h>
#include <tice.h>
#include <debug.h>
#include <time.h>

#include "gfx/global_palette.h"

static constexpr uint16_t SCREEN_WIDTH = 160, SCREEN_HEIGHT = 120;

typedef uint16_t upos;
typedef uint8_t parttype_t;

struct Particle {
    uint8_t type;
};

Particle* grid = new Particle[SCREEN_WIDTH * SCREEN_HEIGHT] {};

void set_part(upos x, upos y, parttype_t type) {
    grid[y * SCREEN_WIDTH + x] = { type };
}

void set_part_buf(upos x, upos y, parttype_t type, Particle* gridBuffer) {
    gridBuffer[y * SCREEN_WIDTH + x] = { type };
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
    
    Particle* gridBuffer = new Particle[SCREEN_WIDTH * SCREEN_HEIGHT] {};
    
    for (int x = 0, y = 0; y < SCREEN_HEIGHT; x++) {
        if (x >= SCREEN_WIDTH) {
            x = 0;
            y++;
        }

        Particle part = get_part(x, y);
        if (part.type == 0)
            continue;
        
        if (get_part(x, y + 1).type == 0) // Down
            set_part_buf(x, y + 1, part.type, gridBuffer);
        else if (get_part(x + 1, y + 1).type == 0) // Down right
            set_part_buf(x + 1, y + 1, part.type, gridBuffer);
        else if (get_part(x - 1, y + 1).type == 0) // Down left
            set_part_buf(x - 1, y + 1, part.type, gridBuffer);
        else // Do not move
            set_part_buf(x, y, part.type, gridBuffer);
    }

    delete[] grid;
    grid = gridBuffer;
    
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
    render_sim();
    simulate_once();
    render_sim();

    for (int x = 0; x < 100; x++) {
        for (int y = 0; y < 100; y++) {
            set_part(x, y, 1);
        }
    }

    render_sim();
    // simulate_once();

    // sleep(1);
    os_GetKey();

    // Graphics shutdown
    gfx_End();
}