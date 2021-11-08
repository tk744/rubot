#include "cube.c"

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
enum Move *solve(struct Cube st) {
    return;
}

enum Move *f2l(struct Cube st) {
    /* Note: can use Y and YI moves to reorient bc they will be removed post-process. */
}

enum Move *oll(struct Cube st) {

}

enum Move *pll(struct Cube st) {

}

enum Move *trim_xyz(enum Move *mv) {
    // remove all cube rotations X/XI, Y/YI, Z/ZI by computing perspective change
}