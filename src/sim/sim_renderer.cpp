#include <graphx.h>
#include <debug.h>
#include <ezSTL\vector.h>
#include <ezstd.h>

#include "particle.h"
#include "sim.h"

#include "common_externs.h"
#include "sim_renderer.h"


#include <time.h>

using std::vector;

using sim::PartUpdate;

vector<PartUpdate> pupdates;

void sim::render::render_sim() {
    //gfx_SetColor(gfx_RGBTo1555(0, 0, 0));

    //dbg_printf("UPD: %d  PARTS: %d  EFF: %f\n", pupdates.size(), parts_num, (float)parts_num / pupdates.size());
    while (pupdates.size() > 0) {
        clock_t c = clock();
        PartUpdate upd = pupdates[0];
        uint8_t x = upd.x;
        uint8_t y = upd.y;

        PartId pId = pmap[y][x];
        uint8_t color = 0;
        if (pId != 0xFF)
            color = (parts[pId].type == 1) ? 4 : 0; // Update in M3/v0.3

        //dbg_printf("Updated (%d, %d) with %d\n", x, y, color);

        gfx_SetColor(color);

        gfx_FillRectangle(x * 2 + 4, y * 2 + 4, 2, 2);
    
        clock_t a = clock();
        pupdates.erase(pupdates.begin());
        clock_t b = clock();
        dbg_printf("%lu, %lu\n", b - a, b - c);
    }
}

void sim::render::update_part(uint8_t x, uint8_t y) {
    PartUpdate upd;
    upd.x = x;
    upd.y = y;
    pupdates.push_back(upd);
}