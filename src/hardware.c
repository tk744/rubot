#include <time.h>
#include "hardware.h"

void delay(int ms) {
    clock_t start = clock(), end;
    do {
        end = clock();
    } while (end-start < CLOCKS_PER_SEC/1000 * ms);
}