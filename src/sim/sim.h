#pragma once

#include "particle.h"

#define MAX_PARTS 1000
#define SIM_W 156
#define SIM_H 104

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
    bool move_part(PartId p, uint8_t newX, uint8_t newY, bool f = false, bool update = true);

    bool part_exists(uint8_t x, uint8_t y);
    bool part_exists(PartId index);

    uint32_t frame_count();

    //Particle get_part(uint8_t x, uint8_t y);
    //Particle get_part(PartId index);
}