/**
 * This library defines the state and behavior of a Rubik's cube.
 */

#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>

/* RUBIK'S CUBE CONSTANTS */

#define NUM_POS 9     // number of positions on a face
#define NUM_FACES 6         // number of faces and colors on a cube

/* TYPE DEFINITIONS */

typedef uint8_t Id;

/* 3-bit encoding for 6 colors. */
typedef Id ColorId;

/* Encoding for 9 positions of a face. 
 * Guaranteed to range on the interval [0, NUM_POS-1] for indexing. */
typedef Id PosId;

/* Encoding for 6 faces of a cube. 
 * Guaranteed to range on the interval [0, NUM_FACES-1] for indexing. */
typedef Id FaceId;

/* Face encoded as 9 3-bit ColorId values sorted by PosId value. */
typedef uint32_t Face;

/* Cube encoded as 6 faces sorted by FaceId values. */
typedef struct {
    Face fs[6];
} Cube;

/* Move encoded as a face id, inverse-rotation flag, double-rotation flag, and cube-rotation flag. */
typedef struct {
    FaceId fid;     // face id: id of face(s) rotated by the move
    int b_c : 1;    // cube flag: 0 if face-rotation, 1 if cube-rotation
    int b_d : 1;    // double flag: 0 if quarter-rotation, 1 if half-rotation
    int b_i : 1;    // inverse flag: 0 if clockwise, 1 if counter-clockwise
} Move;

/* ENUMERATIONS */

const static ColorId    WHITE=1, YELLOW=2, RED=3, ORANGE=4, BLUE=5, GREEN=6;

const static PosId      CC=0, UL=1, UU=2, UR=3, RR=4, DR=5, DD=6, DL=7, LL=8;

const static FaceId     U=0, D=1, R=2, L=3, F=4, B=5;
const static FaceId     X=7, Y=8, Z=9; // dummy ids for cube-rotation moves

const static Move       M_U = { U, 0, 0, 0 }, M_UI = { U, 0, 0, 1 }, M_U2 = { U, 0, 1, 0 },
                        M_D = { D, 0, 0, 0 }, M_DI = { D, 0, 0, 1 }, M_D2 = { D, 0, 1, 0 },
                        M_R = { R, 0, 0, 0 }, M_RI = { R, 0, 0, 1 }, M_R2 = { R, 0, 1, 0 },
                        M_L = { L, 0, 0, 0 }, M_LI = { L, 0, 0, 1 }, M_L2 = { L, 0, 1, 0 },
                        M_F = { F, 0, 0, 0 }, M_FI = { F, 0, 0, 1 }, M_F2 = { F, 0, 1, 0 },
                        M_B = { B, 0, 0, 0 }, M_BI = { B, 0, 0, 1 }, M_B2 = { B, 0, 1, 0 },
                        M_X = { X, 1, 0, 0 }, M_XI = { X, 1, 0, 1 },
                        M_Y = { Y, 1, 0, 0 }, M_YI = { Y, 1, 0, 1 },
                        M_Z = { Z, 1, 0, 0 }, M_ZI = { Z, 1, 0, 1 };

/* FUNCTIONS */

ColorId readColor(Cube c, FaceId fid, PosId pid);

void readColors(Cube c, FaceId *fids, PosId *pids, ColorId *cids, int n);

Cube writeColor(Cube c, FaceId fid, PosId pid, ColorId cid);

Cube writeColors(Cube c, FaceId *fids, PosId *pids, ColorId *cids, int n);

Cube applyMove(Cube c, Move m);

Cube applyMoves(Cube c, Move *ms, int n);

Cube solvedCubeFactory();

Cube scrambledCubeFactory(Move *ms, int n);

int isSolved(Cube c);

void printCube(Cube c);

void printMove(Move m);

void printMoves(Move *ms, int n);

#endif