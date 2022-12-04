#pragma once

#include "sim.h"

extern sim::Particle parts[MAX_PARTS];
extern sim::PartId pmap[SIM_W][SIM_H];
extern uint16_t parts_num;
extern sim::PartId pfree;

extern uint32_t frames;