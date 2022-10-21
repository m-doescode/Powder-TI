#pragma once

#include "particle.h"

namespace sim {
    // Init
    void init_sim();

    // Update funcs
    void update_sim();
    void render_sim();

    // Commands
    bool can_add_particle();

    PartId create_part(PartId pIdx, uint8_t x, uint8_t y, uint8_t type);

    void delete_part(uint8_t x, uint8_t y);
    void delete_part(PartId index);

    bool can_move_part(uint8_t newX, uint8_t newY);
    bool move_part(PartId p, uint8_t newX, uint8_t newY, bool f = false);

    //Particle get_part(uint8_t x, uint8_t y);
    //Particle get_part(PartId index);
}