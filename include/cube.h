/**
 * This library defines the state and behavior of a Rubik's cube.
 */

#ifndef CUBE_H
#define CUBE_H

#include "types.h"

/* CUBE CONSTANTS */

#define NUM_EDGES 12
#define NUM_CORNERS 8
#define NUM_MOVES 18

/* MODEL CONSTANTS */

#define CUBIE_BITS 5

/* TYPE DEFINITIONS */

typedef Int8 Move, MoveMask;
typedef Int8 CubieEnum;

typedef struct {
    Int64 edges;
    Int64 corners;
} Cube;

/* Equivalent representation of faces and colors using existing move representation. */
typedef MoveMask FaceMask;

/* Representation of a face consisting of 9 colors. */
typedef struct {
    FaceMask C, U, D, R, L, UR, UL, DR, DL;
} Face;

/* Representation of a cube consisting of 6 faces. */
typedef struct {
    Face U, D, F, B, R, L;
} ColorCube;

/* ENUMERATIONS */

// 1-hot encoding masks
static const MoveMask NOP=0, U=1, D=2, F=4, B=8, R=16, L=32, I=64, H=128;

// edge cubie encoding index
static const CubieEnum UF=0, UB=1, UR=2, UL=3, DF=4, DB=5, DR=6, DL=7, FR=8, FL=9, BR=10, BL=11;
// corner cubie encoding index
static const CubieEnum UFR=0, UFL=1, UBR=2, UBL=3, DFR=4, DFL=5, DBR=6, DBL=7;

/* FUNCTION PROTOTYPES */

Int8 getCubie(Int64 encoding, CubieEnum ce);

Int8 getPermutation(Int8 cubie, int isEdge);

Int8 getOrientation(Int8 cubie, int isEdge);

void setCubie(Int64 *encoding, CubieEnum ce, Int8 cubie);

void setPermutation(Int8 *cubie, int isEdge, Int8 permutation);

void setOrientation(Int8 *cubie, int isEdge, Int8 orientation);

Cube cubeFactory();

Cube applyMove(Cube c, Move m);

Cube applyMoves(Cube c, Move *ms, int n);

Cube scramble(Cube c, Move *ms, int n);

int areEqual(Cube c1, Cube c2);

void printCube(Cube c);

void printMove(Move m);

void printMoves(Move *ms, int n);

Move inverseMove(Move m);

ColorCube convertCube(Cube c);

#endif