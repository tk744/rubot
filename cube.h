#include <stdint.h>

/* RUBIK'S CUBE CONSTANTS */

#define NUM_FACES 6         // number of faces/colors on a cube
#define NUM_POSITIONS 9     // number of positions on a face
#define NUM_MOVES 18        // number of supported moves

/* TYPE DEFINITIONS */

/* 3-bit encoding for 6 colors. */
typedef uint8_t Color;

/* Encoding for all valid transformations of a cube. */
typedef uint8_t Move;

/* Encoding for 9 positions on a face. */
typedef uint8_t Pos;

/* Face encoded as 9 3-bit Color values sorted by Pos value. */
typedef uint32_t Face;

/* Cube encoded as 6 faces.
 * `f` can be indexed by the first 6 Move values. */
typedef struct { Face f[6]; } Cube;

/* ENUMERATIONS */

const static Color  WHITE=1, YELLOW=2, RED=3, ORANGE=4, BLUE=5, GREEN=6;

/* Guaranteed that base moves start at 0 for array indexing. 
 * Guaranteed that values on the intervals [U,Z] and [UI,ZI] are continuous.
 * Guaranteed that values on the interval [0, NUM_MOVES] are continuous. */
const static Move   U=0, UI=9,   // face rotations
                    D=1, DI=10,
                    R=2, RI=11,
                    L=3, LI=12,
                    F=4, FI=13,
                    B=5, BI=14,
                    X=6, XI=15,  // cube rotations
                    Y=7, YI=16,
                    Z=8, ZI=17;

/* Guaranteed to start at 0 for array indexing. */
const static Pos    CC=0, UL=1, UU=2, UR=3, RR=4, DR=5, DD=6, DL=7, LL=8;

/* FUNCTIONS */

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
int isSolved(Cube c);

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
 */
Cube transform(Cube c, Move m);