#include <keypadc.h>
#include <graphx.h>

#include "../sim/sim.h"

#include "cursor.h"

using namespace nav;

bool need_update = false;
Point8 cursor_pos { 50, 50 };

void nav::cursor_update() {
    need_update = true;

    uint8_t factor = 1;
    if (kb_IsDown(kb_Key2nd))
        factor = 3;

    if (kb_IsDown(kb_KeyRight)) {
        cursor_pos.x += factor;
        if (cursor_pos.x >= SIM_W || cursor_pos.x < 0)
            cursor_pos.x = SIM_W - 1;
    } else if (kb_IsDown(kb_KeyLeft)) {
        cursor_pos.x -= factor;
        if (cursor_pos.x >= SIM_W || cursor_pos.x < 0)
            cursor_pos.x = 0;
    }

    if (kb_IsDown(kb_KeyDown)) {
        cursor_pos.y += factor;
        if (cursor_pos.y >= SIM_H || cursor_pos.y < 0)
            cursor_pos.y = SIM_H - 1;
    } else if (kb_IsDown(kb_KeyUp)) {
        cursor_pos.y -= factor;
        if (cursor_pos.y >= SIM_H || cursor_pos.y < 0)
            cursor_pos.y = 0;
    }

    
    if (kb_IsDown(kb_KeyEnter) && !sim::part_exists(cursor_pos.x, cursor_pos.y)) {
        sim::create_part(-1, cursor_pos.x, cursor_pos.y, 1);
    }
    
    if (kb_IsDown(kb_Key8) && sim::part_exists(cursor_pos.x, cursor_pos.y)) {
        sim::delete_part(cursor_pos.x, cursor_pos.y);
    }

    // else
    //     need_update = false;

}

void nav::cursor_render() {
    gfx_SetDrawBuffer();
    gfx_SetColor(3);
    gfx_FillRectangle(cursor_pos.x * 2 + 4, cursor_pos.y * 2 + 4, 2, 2);
}