#include <stdint.h>

/* RUBIK'S CUBE CONSTANTS */

#define NUM_FACES 6;

/* MODEL REPRESENTATION CONSTANTS */

#define COLOR_BITS 3

/* TYPE DEFINITIONS */

/* Encoding of valid transformations on a cube. */
typedef uint8_t Move, Dir;

/* 3-bit encoding of 6 colors. */
typedef uint8_t Color;

/* Encoding of 9 positions on a face. */
typedef uint8_t Pos;

/* Face represented as 9 3-bit Color values sorted by Pos value. */
typedef uint32_t Face;

/* Cube represented as 6 faces sorted by corresponding Move value. */
typedef struct { Face f[6]; } Cube;

/* ENUMERATIONS */

/* Guaranteed to start at 0 for array indexing. */
const Move  U=0, D=1, L=2, R=3, F=4, B=5, X=6, Y=7, Z=8;
const Dir   CW=0, CCW=1;    // clockwise, counterclockwise

const Color WHITE=1, YELLOW=2, RED=3, ORANGE=4, BLUE=5, GREEN=6;

/* Guaranteed to start at 0 for array indexing. */
const Pos   CC=0, UL=1, UU=2, UR=3, RR=4, DR=5, DD=6, DL=7, LL=8;

/* FUNCTIONS */

/** 
 * Pretty-prints the face `f` to the console.
 * @param f a face
 */
void printFace(Face f);

/** 
 * Pretty-prints the cube `c` to the console.
 * @param c a cube
 */
void printCube(Cube c);

/**
 * Encodes the state of cube `c` into string `*str`. 
 * Returns `*str`.
 * @param c a cube
 * @param *str the encoded savestring that is written into
 */
char *saveCube(Cube c, char *str);

/**
 * Decodes the string `*str` into a cube and returns it.
 * @param *str an encoded savestring
 */
Cube loadCube(char *str);

/**
 * Returns 1 if the cube `c` is solved, 0 otherwise.
 * @param c a cube
 */
int isCubeSolved(Cube c);

/**
 * Returns the color on face `f` at position `p`. 
 * @param f a face
 * @param p a position 
 */
Color readColor(Face f, Pos p);

/**
 * Reads the colors at an array of positions `*p` on face `f` into array `*c`.
 * @param f a face
 * @param *p an array of positions that remains unchanged
 * @param *c an array that the colors are written into
 * @param count the length of `*p` and `*c`
 */
Color *readColors(Face f, Pos *p, Color *c, int count);

/**
 * Returns a new solved cube.
 */
Cube cubeFactory();

/**
 * Applies `n` random moves to the cube `c` to scramble it and writes those moves to array `*m`. 
 * Returns the new scrambled cube.
 * @param c a cube to be scrambled
 * @param *m an array that the moves are written into
 * @param n the number of random moves to apply
 */
Cube scramble(Cube c, Move *m, int n);

/**
 * Returns a new cube by applying the transformation given by move `m` and direction `d` to cube `c`.
 * @param c the original cube which remains unchanged
 * @param m the move to apply
 * @param d the direction to apply the move
 */
Cube transform(Cube c, Move m, Dir d);
