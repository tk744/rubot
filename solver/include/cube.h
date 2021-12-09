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
//static const MoveMask MV_MASK_NOP=0, MV_MASK_U=1, MV_MASK_D=2, MV_MASK_F=4, MV_MASK_B=8, MV_MASK_R=16, MV_MASK_L=32, MV_MASK_I=64, MV_MASK_H=128;

#define MV_MASK_NOP ((char)0)
#define MV_MASK_U ((char)1)
#define MV_MASK_D ((char)2)
#define MV_MASK_F ((char)4)
#define MV_MASK_B ((char)8)
#define MV_MASK_R ((char)16)
#define MV_MASK_L ((char)32)
#define MV_MASK_I ((char)64)
#define MV_MASK_H ((char)128)

// edge cubie encoding index
//static const CubieEnum ECUBIE_UF=0, ECUBIE_UB=1, ECUBIE_UR=2, ECUBIE_UL=3, ECUBIE_DF=4, ECUBIE_DB=5, ECUBIE_DR=6, ECUBIE_DL=7, ECUBIE_FR=8, ECUBIE_FL=9, ECUBIE_BR=10, ECUBIE_BL=11;

#define ECUBIE_UF ((char)0)
#define ECUBIE_UB ((char)1)
#define ECUBIE_UR ((char)2)
#define ECUBIE_UL ((char)3)
#define ECUBIE_DF ((char)4)
#define ECUBIE_DB ((char)5)
#define ECUBIE_DR ((char)6)
#define ECUBIE_DL ((char)7)
#define ECUBIE_FR ((char)8)
#define ECUBIE_FL ((char)9)
#define ECUBIE_BR ((char)10)
#define ECUBIE_BL ((char)11)

// corner cubie encoding index
//static const CubieEnum CCUBIE_UFR=0, CCUBIE_UFL=1, CCUBIE_UBR=2, CCUBIE_UBL=3, CCUBIE_DFR=4, CCUBIE_DFL=5, CCUBIE_DBR=6, CCUBIE_DBL=7;

#define CCUBIE_UFR ((char)0)
#define CCUBIE_UFL ((char)1)
#define CCUBIE_UBR ((char)2)
#define CCUBIE_UBL ((char)3)
#define CCUBIE_DFR ((char)4)
#define CCUBIE_DFL ((char)5)
#define CCUBIE_DBR ((char)6)
#define CCUBIE_DBL ((char)7)

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

Move inverseMove(Move m);

void printCube(Cube c);

void printMove(Move m);

void printMoves(Move *ms, int n);

Move inverseMove(Move m);

ColorCube convertCube(Cube c);

#endif