#include <debug.h>

#include "parttable.h"

PartTable::PartTable(partidx_t alloc_size) {
    this->parts = new Particle[alloc_size];
    free_idx = 0;
    for (partidx_t i = 0; i < alloc_size; i++) {
        this->parts[i] = Particle { 0, 0, (partidx_t)(i + 1U)};
    }
}


partidx_t PartTable::alloc() {
    partidx_t idx = this->free_idx;
    this->free_idx = this->parts[idx].next_free;
    return idx;
}

void PartTable::free(partidx_t idx) {
    parts[idx] = Particle { 0, 0, partidx_t { this->free_idx } };
    this->free_idx = idx;
}

