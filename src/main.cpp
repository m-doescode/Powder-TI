#include <graphx.h>
#include <keypadc.h>
#include "common.h"
#include "debug.h"
#include <time.h>

#include "gfx/gfx.h"

#include "sim/sim.h"
#include "nav/cursor.h"

clock_t time_last = 0;

bool render = 1;
bool simulate = 0;

bool keydetec = 0;

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

        if (simulate) {
            sim::update_sim();
        }

        if (render) {
            sim::render_sim();
        }

        if (!keydetec) {
            if (kb_IsDown(kb_Key9))
                render = !render;
            if (kb_IsDown(kb_Key0))
                simulate = !simulate;
        }

        keydetec = (kb_IsDown(kb_Key0) || kb_IsDown(kb_Key9));
        

        nav::cursor_update();
        nav::cursor_render();

        clock_t now = clock();

        clock_t diff = now - time_last;
        nav::Point8 cpos = nav::cursor_getpos();
        dbg_printf("FPS: %f\t\tFRN: %lu\t\tDYN: %d\n", 1.0F / ((double)diff / CLOCKS_PER_SEC), sim::frame_count(), sim::DEBUG_is_dynamic(cpos.x, cpos.y));

        time_last = now;
        
        gfx_BlitBuffer();
    }
    while (!kb_IsDown(kb_KeyAnnul));

    gfx_End();

    return 0;
}