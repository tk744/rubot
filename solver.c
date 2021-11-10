#include <stdio.h>
#include "cube.h"

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

/* Returns a list of moves to solve a cube from an initial state `st` using the CFOP algorithm. */
int solve(Cube c, Move *m) {
    return;
}

int f2l(Cube c, Move *m) {
    /* Note: can use Y and YI moves to reorient bc they will be removed post-process. */
}

int oll(Cube c, Move *m) {

}

int pll(Cube c, Move *m) {

}

int trim_xyz(Move *dest, Move *src) {
    // remove all cube rotations X/XI, Y/YI, Z/ZI by computing perspective change
}

int main() {
    
}