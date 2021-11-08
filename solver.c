#include "cube.c"

/* TODO:
 * 1. Function to orient the cube white down red forward.
 * 2. Implement CFOP:
 *    i.   White cross
 *    ii.  F2L
 *    iii. OLL
 *    iv.  PLL
 * 3. Postprocess in another source file more specific to robot:
 *    i.   Postprocess to convert U/U' to X,B/B',X' and D/D' equivalent. 
 *    ii.  Postprocess to reorient cube to minimize U/D conversions. */

/* Returns a list of moves to solve a cube from an initial state `st` using the CFOP algorithm. */
enum Move *solve(struct Cube st) {
    return;
}


enum Move *f2l(struct Cube st) {

}

enum Move *oll(struct Cube st) {

}

enum Move *pll(struct Cube st) {

}