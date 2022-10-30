#pragma once

#include "navcommon.h"

namespace nav {
    void cursor_update();
    void cursor_render();

    void cursor_moveto(Point8 pos);
    Point8 cursor_getpos();
}