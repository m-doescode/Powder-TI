#include <string.h>
#include <ezSTL\vector.h>
#include <ezstd.h>
#include <graphx.h>
#include <debug.h>

#include "sim_renderer.h"

#include "sim.h"

using std::vector;

using namespace sim;

Particle parts[MAX_PARTS];
PartId pmap[SIM_W][SIM_H];
uint16_t parts_num;
PartId pfree;

uint32_t frames;

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
    frames = 0;
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

void update_surrounding_parts(uint8_t x, uint8_t y) {
    static const int8_t vecs[8][2] {
        { 0, 1 },
        { 1, 0},
        { 0, -1 },
        { -1, 0 },
        { 1, 1 },
        { 1, -1},
        { -1, -1 },
        { -1, 1 }
    };

    for (int i = 0; i < 8; i++) {
        PartId partid = pmap[y + vecs[i][1]][x + vecs[i][0]];
        if (partid != -1) {
            parts[partid].dynamic = 1;
        }
    }
}

void update_part(PartId partId, bool xparity) {

    Particle* part = &parts[partId]; // (parts + partId)

    // If the particle hits rock bottom, make it static
    if (part->y >= (SIM_H - 1)) {
        part->dynamic = 0;
        return;
    }
    
    // Attempt to move particle to new pos (0,1)
    if (move_part(partId, part->x, part->y + 1))
        // If successful, move on...
        return;
    // If the particle we're trying to replace is dynamic, stay put. (0,1)
    else if (parts[pmap[part->y + 1][part->x]].dynamic)
        return;
    // Attempt to move particle to new pos (1/-1,1)
    else if (move_part(partId, part->x + 1 * xparity, part->y + 1))
        return;
    // Attempt to move particle to new pos (-1/1,1)
    else if (move_part(partId, part->x - 1 * xparity, part->y + 1))
        return;
    // Otherwise, stay put and become static.
    else {
        part->dynamic = 0;
        return;
    }
}

void sim::update_sim() {
    frames++;
    
    // Make it alternate between left and right sidedness
    int8_t xparity = (frames % 2) * 2 - 1;

    //dbg_printf("in update_falling_particles\n");
    int rp = 0; // Real parts
    for (int p = 0; p < MAX_PARTS && rp < parts_num; p++) {
        Particle part = parts[p];
        if (parts[p].type == 0)
            continue;
        rp++;

        // part.type == 1 because only DUST (1) is supported.
        if (!(part.type == 1 && part.dynamic == 1))
            continue;

        update_part(p, xparity);
    }
}

void sim::render_sim() {
    sim::render::redraw_all();
    // gfx_SetColor(gfx_RGBTo1555(0, 0, 0));
    // gfx_FillRectangle(4, 4, SIM_W * 2, SIM_H * 2);

    // gfx_SetColor(/*gfx_RGBTo1555(255, 224, 160)*/ 4);
    // int rp = 0; // Real parts
    // for (int p = 0; p < MAX_PARTS && rp < parts_num; p++) {
    //     if (parts[p].type == 0)
    //         continue;
    //     rp++;

    //     Particle part = parts[p];
    //     gfx_FillRectangle(part.x * 2 + 4, part.y * 2 + 4, 2, 2);
    
    // }
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
        parts[i].dynamic = 1;

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
        parts[pIdx].dynamic = 1;

        pmap[y][x] = pIdx;

        return pIdx;
    }
}

void sim::delete_part(uint8_t x, uint8_t y, bool update) {
    delete_part(pmap[y][x], update);
}

void sim::delete_part(PartId index, bool update) {
    Particle part = parts[index];
    pmap[part.y][part.x] = -1;

    if (update)
        update_surrounding_parts(part.x, part.y);

    parts[index].type = 0;
    parts[index].meta = pfree;
    parts[index].y = 0;
    parts[index].x = 0;
    parts[index].dynamic = 0;

    pfree = index;
    
    parts_num--;
}

bool sim::can_move_part(uint8_t newX, uint8_t newY) {
    return newX < SIM_W && newY < SIM_H && (pmap[newY][newX] == -1);
}

bool sim::move_part(PartId p, uint8_t newX, uint8_t newY, bool f, bool update) {
    // If force move is false, then check if the new position is free to move the particle to.
    if (!f && !can_move_part(newX, newY))
        return false;
    
    Particle part = parts[p];
    if (update)
        update_surrounding_parts(part.x, part.y);
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

uint32_t sim::frame_count() {
    return frames;
}

bool sim::DEBUG_is_dynamic(uint8_t x, uint8_t y) {
    if (pmap[y][x] == -1)
        return 0;
    else
        return parts[pmap[y][x]].dynamic;    
}