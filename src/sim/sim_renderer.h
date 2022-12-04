#pragma once

#include "sim.h"

namespace sim::render {
    // If the user unpauses rendering or if it REALLY is necessary
    void redraw_all();
    
    /***
     * Redraw ONE part. If partId == -1, then part is set to whatever is at that position. If the position is empty, it renders black.
     * Otherwise, it will render the requested part at the specified position
    */
    void render_part(PartId partId, uint8_t x, uint8_t y);

    // NYI, for toolbar
    void redraw_toolbar();
}