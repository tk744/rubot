/**
 * This library defines the state and behavior of a Rubik's cube.
 */

#ifndef CUBE_H
#define CUBE_H

/* CUBE CONSTANTS */

#define NUM_EDGES 12
#define NUM_CORNERS 8
#define NUM_MOVES 18

/* MODEL CONSTANTS */

#define CUBIE_BITS 5

/* TYPE DEFINITIONS */

typedef unsigned char Int8;
typedef unsigned long long Int64;

typedef Int8 Move, MoveMask;
typedef Int8 CubieEnum;
typedef Int8 FaceEnum;

typedef struct {
    Int64 edges;
    Int64 corners;
} Cube;

/* ENUMERATIONS */

// 1-hot encoding masks
static const MoveMask NOP=0, U=1, D=2, F=4, B=8, R=16, L=32, I=64, H=128;

// edge cubie encoding index
static const CubieEnum UF=0, UB=1, UR=2, UL=3, DF=4, DB=5, DR=6, DL=7, FR=8, FL=9, BR=10, BL=11;
// corner cubie encoding index
static const CubieEnum UFR=0, UFL=1, UBR=2, UBL=3, DFR=4, DFL=5, DBR=6, DBL=7;

static FaceEnum U_FACE=1, D_FACE=2, F_FACE=3, B_FACE=4, R_FACE=5, L_FACE=6;

/* FUNCTION PROTOTYPES */

Cube cubeFactory();

Cube applyMove(Cube c, Move m);

Cube applyMoves(Cube c, Move *ms, int n);

int areEqual(Cube c1, Cube c2);

void printCube(Cube c);

void printMove(Move m);

void printMoves(Move *ms, int n);

#endif