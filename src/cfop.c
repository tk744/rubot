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
    Move *ms;    // stack data
    int n;   // stack size
} MoveStack;

static MoveStack stackFactory(int n) {
    Move ms[n];
    MoveStack s = { ms, 0 }; 
    return s;
}

static MoveStack push(MoveStack s, Move m) {
    s.ms[s.n++] = m;
    return s;
}

static Move pop(MoveStack s) {
    return s.ms[--s.n];
}

static MoveStack wc(Cube c, MoveStack s) {
    // s = push(s, M_U);
    return s;
}

static MoveStack f2l(Cube c, MoveStack s) {
    /* Note: can use Y and YI moves to reorient bc they will be removed post-process. */
    return s;
}

static MoveStack oll(Cube c, MoveStack s) {
    return s;
}

static MoveStack pll(Cube c, MoveStack s) {
    return s;
}

// remove instances of X, XI, Y, YI, Z, ZI
// by computing perspective change
int trim_xyz(Cube c, Move *ms, int n) {
    FaceId map[NUM_FACES];
    int size = 0;

    int i;
    for(i=0 ; i<NUM_FACES ; i++) {
        map[i] = i;
    }

    for(i=0 ; i<n ; i++) {
        if (!ms[i].b_c) {   // apply map
            Move m_mapped = { map[ms[i].fid], ms[i].b_c, ms[i].b_d, ms[i].b_i };
            ms[size++] = m_mapped;
        }
        else {              // update map

        }
    }

    return size;
}

int solve(Cube c, Move *ms, int n) {
    /* TODO: use function pointer array
    int i;
    for(i=0; i<NUM_STEPS ; i++) {
        Cube (*wc_ptr)(Cube, Move *, int *) = wc
    }
    */
    
    MoveStack s = { ms, n };

    s = wc(applyMoves(c, ms, s.n), s);
    s = f2l(applyMoves(c, ms, s.n), s);
    s = oll(applyMoves(c, ms, s.n), s);
    s = pll(applyMoves(c, ms, s.n), s);

    return s.n;
}
