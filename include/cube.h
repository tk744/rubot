/**
 * This library defines the state and behavior of a Rubik's cube.
 */

#ifndef CUBE_H
#define CUBE_H

/* CUBE CONSTANTS */

#define NUM_EDGES 12
#define NUM_CORNERS 8
#define NUM_MOVES 18

/* TYPE DEFINITIONS */

typedef unsigned char Int8;
typedef unsigned long long Int64;

typedef Int8 Move, MoveMask;
typedef Int8 CubieEnum;

typedef struct {
    Int64 edges;
    Int64 corners;
} Cube;

/* ENUMERATIONS */

// 1-hot encoding masks
static const MoveMask NOP=0, U=1, D=2, F=4, B=8, R=16, L=32, I=64, H=128;

// edge cubies
static const CubieEnum UF=0, UB=1, UR=2, UL=3, FR=4, FL=5, DF=6, DB=7, DR=8, DL=9, BR=10, BL=11;
// corner cubies
static const CubieEnum UFR=0, UFL=1, UBR=2, UBL=3, DFR=4, DFL=5, DBR=6, DBL=7;

/* FUNCTION PROTOTYPES */

Cube cubeFactory();

Cube applyMove(Cube c, Move m);

Cube applyMoves(Cube c, Move *ms, int n);

int areEqual(Cube c1, Cube c2);

void printCube(Cube c);

void printMove(Move m);

void printMoves(Move *ms, int n);

#endif