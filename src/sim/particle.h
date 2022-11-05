#pragma once

#include <stdint.h>

#define IN_BOUNDS(x, y) (x >= 0 && y >= 0 && x <= SIM_W && y <= SIM_H)

namespace sim {
    typedef int16_t PartId;

    struct Particle {
        uint8_t type;
        int16_t meta;
        uint8_t x;
        uint8_t y;
        uint8_t dynamic : 1;
    };
}