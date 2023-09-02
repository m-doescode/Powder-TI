#include <stdint.h>

typedef uint16_t upos;
typedef uint16_t partidx_t;
typedef uint8_t parttype_t;

struct partpos_t {
    upos x;
    upos y;

    partpos_t operator +(const partpos_t other) {
        return { (upos)(x + other.x), (upos)(y + other.y) };
    }

    partpos_t operator -(const partpos_t other) {
        return { (upos)(x - other.x), (upos)(y - other.y) };
    }

    partpos_t operator *(const partpos_t other) {
        return { (upos)(x * other.x), (upos)(y * other.y) };
    }

    partpos_t operator /(const partpos_t other) {
        return { (upos)(x / other.x), (upos)(y / other.y) };
    }
};

struct Particle {
    uint8_t type : 7;
    bool part_static : 1;
    union {
        partpos_t pos;
        partidx_t next_free;
    };
};