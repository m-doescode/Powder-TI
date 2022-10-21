#include <graphx.h>
#include <keypadc.h>
#include "common.h"
#include "debug.h"

#include "gfx/gfx.h"

#include "sim/sim.h"

int main() {
    gfx_Begin();

    gfx_SetDrawBuffer();

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(2);

    sim::init_sim();

    sim::create_part(-1, 50, 10, 1);
    sim::create_part(-1, 50, 11, 1);
    sim::create_part(-1, 50, 12, 1);
    sim::create_part(-1, 50, 13, 1);

    do {
        kb_Scan();

        if (kb_IsDown(kb_Key0)) {
            sim::update_sim();
        }

        //if (kb_IsDown(kb_KeyChs)) {
            sim::render_sim();
        //}

        
        gfx_BlitBuffer();
    }
    while (!kb_IsDown(kb_KeyAnnul));

    gfx_End();

    return 0;
}