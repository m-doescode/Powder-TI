#include <string.h>
#include <ezSTL\vector.h>
#include <ezstd.h>
#include <graphx.h>
#include <debug.h>

#include "sim.h"

using std::vector;

using namespace sim;

Particle parts[MAX_PARTS];
PartId pmap[SIM_W][SIM_H];
uint16_t parts_num;
PartId pfree;

bool sim::can_add_particle() {
    return parts_num < MAX_PARTS;
}

PartId pop_id() {
    PartId i = pfree;
    if (i == -1)
        return -1;
    pfree = parts[i].meta;
    return i;
}

// For future reference:
// This prgm uses the same method as TPT for allocating particles:
// pfree points to a part that is available for taking.
// The life (meta) of that part points to the next available part
// and so on, each time a particle becomes free, it is erased
// and its life (meta) value is set to the current value of pfree
// and pfree is set to its index.
// Conversly, when a new particle is allocated, the index for it
// is taken from pfree, and pfree is set to its life (meta) value.

//

void sim::init_sim() {
    parts_num = 0;
    pfree = 0;
	memset(pmap, 0, sizeof(pmap));
    // Copied directly from TPT.
	memset(parts, 0, sizeof(Particle)*MAX_PARTS);
	for (int i = 0; i < MAX_PARTS-1; i++)
		parts[i].meta = i+1;
    for (int y = 0; y < SIM_H; y++) {
        for (int x = 0; x < SIM_W; x++) {
            pmap[y][x] = -1;
        }
    }

    gfx_SetColor(gfx_RGBTo1555(0, 0, 0));
    gfx_FillRectangle(0, 0, GFX_LCD_WIDTH, GFX_LCD_HEIGHT);

    gfx_SetColor(3);
    gfx_Rectangle(3, 3, GFX_LCD_WIDTH - 6, GFX_LCD_HEIGHT - 6);
}

void update_falling_particles() {
    dbg_printf("in update_falling_particles\n");
    int rp = 0; // Real parts
    for (int p = 0; p < MAX_PARTS && rp < parts_num; p++) {
        if (parts[p].type == 0)
            continue;
        rp++;

        Particle part = parts[p];
        if (part.type == 1) {
            if (part.y <= 0)
                continue;
            dbg_printf("%d: %d at (%d, %d)\n", p, pmap[part.y][part.x], part.x, part.y);
            // Attempt to move sand down
            if (sim::can_move_part(part.x, part.y - 1)) {
                sim::move_part(p, part.x, part.y - 1, true); // Save cycles by setting force to true
            // Attempt to move sand down right
            } else if (sim::can_move_part(part.x - 1, part.y - 1)) {
                sim::move_part(p, part.x - 1, part.y - 1, true);
            // Attempt to move sand down left
            } else if (sim::can_move_part(part.x + 1, part.y - 1)) {
                sim::move_part(p, part.x + 1, part.y - 1, true);
            }
        }
    }
}

void sim::update_sim() {
    update_falling_particles();
}

void sim::render_sim() {
    gfx_SetColor(gfx_RGBTo1555(0, 0, 0));
    gfx_FillRectangle(4, 4, SIM_W * 2, SIM_H * 2);

    gfx_SetColor(/*gfx_RGBTo1555(255, 224, 160)*/ 4);
    int rp = 0; // Real parts
    for (int p = 0; p < MAX_PARTS && rp < parts_num; p++) {
        if (parts[p].type == 0)
            continue;
        rp++;

        Particle part = parts[p];
        gfx_FillRectangle(part.x * 2 + 4, part.y * 2 + 4, 2, 2);

    }
}


//

PartId sim::create_part(PartId pIdx, uint8_t x, uint8_t y, uint8_t type) {
    if (pIdx == -1) {

        PartId i = pfree;
        if (i == -1)
            return -1;
        pfree = parts[i].meta;

        parts[i].type = type;
        parts[i].meta = 0;
        parts[i].x = x;
        parts[i].y = y;

        pmap[y][x] = i;

        parts_num++;

        return i;
    } else {
        // Check if the existing particle exists
        if (parts[pIdx].type == 0)
            return -1;
        
        // Check if the position of the new particle does not overlap another. Allowed if it is the position of the old particle.
        // pmap[y][x] != -1 | Check if there is a particle. pmap[y][x] != pIdx | Check that it is not the particle we're overwriting.
        if (pmap[y][x] != -1 && pmap[y][x] != pIdx)
            return -1;

        parts[pIdx].type = type;
        parts[pIdx].meta = 0;
        parts[pIdx].x = x;
        parts[pIdx].y = y;

        pmap[y][x] = pIdx;

        return pIdx;
    }
}

void sim::delete_part(uint8_t x, uint8_t y) {
    delete_part(pmap[y][x]);
}

void sim::delete_part(PartId index) {
    Particle part = parts[index];
    pmap[part.y][part.x] = -1;

    parts[index].type = 0;
    parts[index].meta = pfree;
    parts[index].y = 0;
    parts[index].x = 0;

    pfree = index;
    
    parts_num--;
}

bool sim::can_move_part(uint8_t newX, uint8_t newY) {
    return newX < SIM_W && newY < SIM_H && (pmap[newY][newX] == -1);
}

bool sim::move_part(PartId p, uint8_t newX, uint8_t newY, bool f) {
    // If force move is false, then check if the new position is free to move the particle to.
    if (!f && !can_move_part(newX, newY))
        return false;
    
    Particle part = parts[p];
    pmap[newY][newX] = p;
    pmap[part.y][part.x] = -1;
    part.x = newX;
    part.y = newY;
    parts[p] = part;
    
    return true;
}

bool sim::part_exists(uint8_t x, uint8_t y) {
    return pmap[y][x] != -1; 
}

bool sim::part_exists(PartId index) {
    return parts[index].type != 0;
}

// Particle sim::get_part(uint8_t x, uint8_t y) {
//     if (!IN_BOUNDS(x, y))
//         throw "Out of bounds";
    
//     return get_part(pmap[y][x]);
// }

// Particle sim::get_part(PartId index) {
//     if (index < 0 | index >= 1000)
//         throw "Out of bounds!";

//     return parts[index];
// }