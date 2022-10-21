#pragma once

#include <graphx.h>

void print(const char* str) {
    gfx_SetColor(gfx_RGBTo1555(0, 0, 0));
    gfx_PrintStringXY(str, 0, 0);
    gfx_BlitBuffer();
}