#include "part.h"

class PartTable {
    Particle* parts;
    partidx_t free_idx;

    public:
    PartTable(partidx_t alloc_size);
    partidx_t alloc();
    void free(partidx_t idx);
    Particle& get(partidx_t idx);
};