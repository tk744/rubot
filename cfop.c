#include <stdio.h>
#include "solver.h"

/* TODO:
 * 1. Function to orient the cube white down red forward.
 * 2. Implement CFOP:
 *    i.   White cross
 *    ii.  F2L
 *    iii. OLL
 *    iv.  PLL
 * 3. Postprocess in another source file more specific to robot:
 *    i.   Convert R,LI and RI,L and FB and UD consecutive pairs to Z/Z', X/X' and Y/Y'
 *    ii.  Remove all Y/Y' (and X/X', Z/Z') by changing perspective.
 *    iii. Convert U/U' to X,B/B',X' or Z,B/B',Z' and D/D' equivalent. 
 *    iv.  Reorient cube initially to minimize U/D conversions. */

#define WC_MAX 0
#define F2L_MAX 0
#define OLL_MAX 0
#define PLL_MAX 0

#define NUM_STEPS 4

typedef struct {
    Move *m;
    int c;
} MoveCount;

static Cube wc(Cube c, Move *m, int *n) {
    *n = 10;
    return transform(c, U);
}

static int f2l(Cube c, Move *m) {
    /* Note: can use Y and YI moves to reorient bc they will be removed post-process. */
}

static int oll(Cube c, Move *m) {

}

static int pll(Cube c, Move *m) {

}

int trim_xyz(Move *dest, Move *src) {
    // remove all cube rotations X/XI, Y/YI, Z/ZI by computing perspective change
}

// Cube (*wc_ptr)(Cube, Move *, int *) = wc;

int solve(Cube c, Move *m) {



    /* TODO: use function pointer array
    int i;
    for(i=0; i<NUM_STEPS ; i++) {
        Cube (*wc_ptr)(Cube, Move *, int *) = wc
    }
    */

    return 0;
}

int main() {
    Cube c = cubeFactory();
    printCube(c);

    // scramble
    Move moves[] = { U, U, DI, DI, R, R, LI, LI, F, F, BI, BI };
    c = applyMoves(c, moves, sizeof(moves) / sizeof(moves[0]));
    printCube(c);

    // white cross
    Move m[WC_MAX];
    int count;
    c = wc(c, m, &count);
    printCube(c);
    printf("%d", count);
}