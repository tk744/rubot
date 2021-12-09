#include "cube.h"
#include "tft_gfx.h"
#include "tft_master.h"

typedef int screen;

static screen TITLE_SCREEN = 1, CUBE_SCREEN = 2, SOLVING_SCREEN = 3;

updateTFT(Cube c, Move mv, int mv_i, int mv_n);

drawTFT(Screen sc);