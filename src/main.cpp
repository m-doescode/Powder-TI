#include <graphx.h>
#include <tice.h>
#include <debug.h>
#include <time.h>

#include "gfx/global_palette.h"
#include "stopwatch.h"

#define TI84

#include "part.h"
#include "linkedlist.h"
#include "parttable.h"

static constexpr uint16_t SCREEN_WIDTH = 160, SCREEN_HEIGHT = 120;
static constexpr uint16_t MAX_GRID = SCREEN_WIDTH * SCREEN_HEIGHT;
static constexpr uint16_t MAX_PARTS = 2000;
static constexpr uint16_t NO_PART = 0xFFFF;


partidx_t* grid = new partidx_t[MAX_GRID] { };
PartTable parts(MAX_PARTS);
LinkedList<partidx_t> activeParts;

//

Particle part_at(upos x, upos y) {
    partidx_t idx = grid[y * SCREEN_WIDTH + x];
    if (idx == NO_PART)
        return { 0, 0, partpos_t { x, y } };
    return parts.get(idx);
}

partidx_t idx_at(partpos_t pos) {
    return grid[pos.y * SCREEN_WIDTH + pos.x];
}

partidx_t add_part(upos x, upos y, parttype_t type) {
    if (grid[y * SCREEN_WIDTH + x] != NO_PART)
        throw("Particle already exists at (%d, %d), value: %d\n", x, y, grid[y * SCREEN_WIDTH + x]);
    Particle part { type, 0, partpos_t { x, y } };
    partidx_t idx = parts.alloc();
    parts.get(idx) = part;
    grid[y * SCREEN_WIDTH + x] = idx;
    activeParts.push_back(idx);

    return idx;
}

void del_part(upos x, upos y) {
    partidx_t idx = grid[y * SCREEN_WIDTH + x];
    grid[y * SCREEN_WIDTH + x] = NO_PART;
    parts.free(idx);
    if (ListNode<partidx_t>* node = activeParts.find(idx))
        activeParts.remove(node);
}

void del_part(partidx_t idx) {
    Particle part = parts.get(idx);
    grid[part.pos.y * SCREEN_WIDTH + part.pos.x] = NO_PART;
    parts.free(idx);
    if (ListNode<partidx_t>* node = activeParts.find(idx))
        activeParts.remove(node);
}

void move_part(size_t idx, upos x, upos y) {
    Particle& part = parts.get(idx);
    grid[part.pos.y * SCREEN_WIDTH + part.pos.x] = NO_PART;
    grid[y * SCREEN_WIDTH + x] = idx;
    part.pos = { x, y };
}

//

void init_sim() {
    for (int i = 0; i < MAX_GRID; i++) {
        grid[i] = NO_PART;
    }
}

void simulate_once() {
    for (ListIterator it = activeParts.iterator(); it.has_current(); it.next()) {
        Particle& part = parts.get(it.current());

        // Forgot to make sure we don't go out of bounds
        if (part.pos.y >= SCREEN_HEIGHT - 1) {
            part.part_static = 1;
            continue;
        }

        sw_start();
        if (idx_at(part.pos + partpos_t { 0, 1 }) == NO_PART)
            move_part(it.position(), part.pos.x, part.pos.y + 1);
        else if (idx_at(part.pos + partpos_t { 1, 1 }) == NO_PART)
            move_part(it.position(), part.pos.x + 1, part.pos.y + 1);
        else if (idx_at(part.pos + partpos_t { 0, 1 } - partpos_t { 1, 0 }) == NO_PART) // Dumb ass hack bc I have the foresight of a quantum particle
            move_part(it.position(), part.pos.x - 1, part.pos.y + 1);
        else {
            part.part_static = 1;
        }
    }
}

void draw_pixel(uint8_t x, uint8_t y, uint8_t mul) {
    gfx_FillRectangle(x * mul, y * mul, mul, mul);
}

void render_sim() {
    // Clear screen
    gfx_FillScreen(0);

    for (partidx_t i = 0; i < MAX_GRID; i++) {
        if (grid[i] != NO_PART) {
            gfx_SetColor(4);
            draw_pixel(i % SCREEN_WIDTH, i / SCREEN_WIDTH, 2);
        }
    }

    gfx_BlitBuffer();
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

    // // simulate_once();
    init_sim();
    render_sim();

    for (int i = 0; i < 500; i++) {
        add_part(i % 100 + 20, i / 100 + 20, 1);
    }

    // render_sim();

    // sw_start();
    // parts.get(10);
    // sw_stop();
    // parts.get(999);
    // sw_stop();

    // gfx_End();
    // abort();

    for (int i = 0; i < 1000; i++) {
        // add_part(50, 5, 1);
        // sw_start();
        simulate_once();
        // sw_stop("SIMULATE");
        render_sim();
        // sw_stop("RENDER");
    }

    // sleep(1);
    os_GetKey();

    // Graphics shutdown
    gfx_End();
}