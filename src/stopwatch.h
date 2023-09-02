#include <time.h>
#include <debug.h>

clock_t cock = clock();

void sw_start() {
    cock = clock();
}

void sw_stop(const char* label = nullptr) {
    if (label != nullptr)
        dbg_printf("[%s] ", label);

    dbg_printf("Taken %f\n", (float)(clock() - cock) / CLOCKS_PER_SEC);
    sw_start();
}