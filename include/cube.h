/**
 * This library defines the state and behavior of a Rubik's cube.
 */

#ifndef CUBE_H
#define CUBE_H

#define NUM_EDGES 12
#define NUM_CORNERS 8
#define NUM_MOVES 18

#include "types.h"

typedef struct {
    Int64 edges;
    Int64 corners;
} Cube128;

typedef Int8 Move, MoveMask;
typedef Int8 CubieEnum;

static const MoveMask NOP=0, U=1, D=2, F=4, B=8, R=16, L=32, I=64, H=128; // one-hot

static const CubieEnum UF=0, UB=1, UR=2, UL=3, DF=4, DB=5, DR=6, DL=7, FR=8, FL=9, BR=10, BL=11;

static const CubieEnum UFR=0, UFL=1, UBR=2, UBL=3, DFR=4, DFL=5, DBR=6, DBL=7;

Cube128 solvedCube();

Cube128 applyMove(Cube128 c, Move m);

Cube128 applyMoves(Cube128 c, Move *ms, int n);

Cube128 scramble(Cube128 c, Move *ms, int n);

Int8 getCubie(Int64 encoding, CubieEnum ce);

Int8 getPermutation(Int8 cubie, int isEdge);

Int8 getOrientation(Int8 cubie, int isEdge);

void setCubie(Int64 *encoding, CubieEnum ce, Int8 cubie);

void setPermutation(Int8 *cubie, int isEdge, Int8 permutation);

void setOrientation(Int8 *cubie, int isEdge, Int8 orientation);

int areEqual(Cube128 c1, Cube128 c2);

void printCube(Cube128 c);

void printMove(Move m);

void printMoves(Move *ms, int n);

#endif