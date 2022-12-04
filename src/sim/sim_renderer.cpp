#include <graphx.h>

#include "sim.h"
#include "sim_privates.h"

#include "sim_renderer.h"

using namespace sim;

void sim::render::redraw_all() {
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