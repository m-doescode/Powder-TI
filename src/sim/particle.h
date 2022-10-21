#pragma once

#include <stdint.h>

#define IN_BOUNDS(x, y) (x >= 0 && y >= 0 && x < 100 && y < 100)

namespace sim {
    typedef int16_t PartId;

    struct Particle {
        uint8_t type;
        int16_t meta;
        uint8_t x;
        uint8_t y;
    };
}