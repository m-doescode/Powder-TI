#include <graphx.h>
#include <debug.h>
#include <ezSTL\vector.h>
#include <ezstd.h>

#include "particle.h"
#include "sim.h"

#include "sim_renderer.h"

#define MAX_PARTS 1000
#define SIM_W 100
#define SIM_H 100

using std::vector;

using sim::Particle;
using sim::PartId;
using sim::PartUpdate;

extern Particle parts[MAX_PARTS];
extern PartId pmap[SIM_W][SIM_H];
extern uint16_t parts_num;
extern PartId pfree;

extern uint32_t frames;

vector<PartUpdate> pupdates;

void sim::render::render_sim() {
    gfx_SetColor(gfx_RGBTo1555(0, 0, 0));
    gfx_FillRectangle(4, 4, SIM_W * 2, SIM_H * 2);

    dbg_printf("Particle updates: %d\n", pupdates.size());

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

void sim::render::update_part(uint8_t x, uint8_t y) {
    pupdates.push_back({ x, y });
}