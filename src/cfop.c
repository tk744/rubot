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
#define MAX_STEPS WC_MAX+F2L_MAX+OLL_MAX+PLL_MAX

#define NUM_STEPS 4

typedef struct {
    Move *x;    // stack data
    int size;   // stack size
} MoveStack;

static MoveStack stackFactory(int size) {
    Move s[size];
    MoveStack ms = { s, 0 }; 
    return ms;
}

static MoveStack push(MoveStack ms, Move m) {
    ms.x[ms.size++] = m;
    return ms;
}

static Move pop(MoveStack ms) {
    return ms.x[--ms.size];
}

static MoveStack wc(Cube c, MoveStack ms) {
    ms = push(ms, U);
    return ms;
}

static MoveStack f2l(Cube c, MoveStack ms) {
    /* Note: can use Y and YI moves to reorient bc they will be removed post-process. */
    return ms;
}

static MoveStack oll(Cube c, MoveStack ms) {
    return ms;
}

static MoveStack pll(Cube c, MoveStack ms) {
    return ms;
}

int trim_xyz(Move *dest, Move *src, int n) {
    // remove all cube rotations X/XI, Y/YI, Z/ZI by computing perspective change
}

int solve(Cube c, Move *m, int max_steps) {
    /* TODO: use function pointer array
    int i;
    for(i=0; i<NUM_STEPS ; i++) {
        Cube (*wc_ptr)(Cube, Move *, int *) = wc
    }
    */
    
    MoveStack ms = { m, max_steps };

    ms = wc(applyMoves(c, ms.x, ms.size), ms);
    ms = f2l(applyMoves(c, ms.x, ms.size), ms);
    ms = oll(applyMoves(c, ms.x, ms.size), ms);
    ms = pll(applyMoves(c, ms.x, ms.size), ms);

    m = ms.x;
    return ms.size;
}

int main() {
    Cube c0 = cubeFactory();

    // scramble
    Move moves[] = { U, U, DI, DI, R, R, LI, LI, F, F, BI, BI };
    Cube c1 = applyMoves(c0, moves, sizeof(moves) / sizeof(moves[0]));

    // solve
    Move solution[MAX_STEPS];
    int steps = solve(c0, solution, MAX_STEPS);
    Cube c2 = applyMoves(c0, solution, steps);

    printf("Original cube:\n");
    printCube(c0);
    printf("Scrambled cube:\n");
    printCube(c1);
    printf("Solved cube (WIP): %d steps\n", steps);
    printCube(c2);
}