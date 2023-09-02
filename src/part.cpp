#include "part.h"

partpos_t partpos_t::operator +(const partpos_t other) {
    return { (upos)(x + other.x), (upos)(y + other.y) };
}

partpos_t partpos_t::operator -(const partpos_t other) {
    return { (upos)(x - other.x), (upos)(y - other.y) };
}

partpos_t partpos_t::operator *(const partpos_t other) {
    return { (upos)(x * other.x), (upos)(y * other.y) };
}

partpos_t partpos_t::operator /(const partpos_t other) {
    return { (upos)(x / other.x), (upos)(y / other.y) };
}