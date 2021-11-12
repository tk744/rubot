/**
 * This file implements the state and behavior of a Rubik's cube.
 */

#include <stdio.h>
#include "solver.h"

/* MODEL REPRESENTATION CONSTANTS */

#define COLOR_BITS 3    // bits required to store a color value

/* FUNCTION IMPLEMENTATION */

ColorId readColor(Cube c, FaceId fid, PosId pid) {
    return (c.fs[fid] >> (COLOR_BITS * pid)) & 0b111;
}

Cube writeColor(Cube c, FaceId fid, PosId pid, ColorId cid) {
    Face clear_mask = ~(0b111 << (COLOR_BITS * pid));
    Face write_mask = cid << (COLOR_BITS * pid);
    c.fs[fid] = c.fs[fid] & clear_mask | write_mask;
    return c;
}

void readColors(Cube c, FaceId *fids, PosId *pids, ColorId *cids, int n) {
    int i;
    for (i=0 ; i<n ; i++) {
        cids[i] = readColor(c, fids[i], pids[i]);
    }
}

Cube writeColors(Cube c, FaceId *fids, PosId *pids, ColorId *cids, int n) {
    int i;
    for (i=0 ; i<n ; i++) {
        c = writeColor(c, fids[i], pids[i], cids[i]);
    }
    return c;
}

static Id *reverseArray(Id *arr, int n) {
    int start = 0, end = n-1;
    int tmp;
    while(start < end) {
        tmp = arr[start];
        arr[start] = arr[end];
        arr[end] = tmp;
        start++;
        end--;
    }
    return arr;
}

static Cube shiftColors(Cube c, FaceId *fids, PosId *pids, int n, int stride, int inv) {
    // TODO: rotate arrays for inverse

    ColorId buffer[stride], tmp[stride];
    ColorId cid;

    // store colors from first stride into tmp
    readColors(c, fids, pids, tmp, stride);

    int i, i_next;
    for (i=0 ; i<n ; i+=stride) {
        // compute index of next stride
        i_next = (i+stride) % n;

        // read colors from current stride into buffer
        readColors(c, fids+i_next, pids+i_next, buffer, stride);

        // write colors from buffer into next stride
        c = writeColors(c, fids+i, pids+i, buffer, stride);
    }

    // write color from tmp into last stride
    c = writeColors(c, fids+(n-stride), pids+(n-stride), tmp, stride);

    return c;
}

Cube applyMove(Cube c, Move m) {
    typedef struct {
        FaceId *fids;
        PosId *pids;
        int n;
    } Shift;

    Shift s;        // shift lines of colors around cube
    if (m.fid == U) {
        static FaceId ufids[12] = { L, L, L, F, F, F, R, R, R, B, B, B };
        static PosId upids[12] = { UL, UU, UR, UL, UU, UR, UL, UU, UR, UL, UU, UR };
        static Shift uss = { ufids, upids, 12 };
        s = uss;
    }
    else if (m.fid == D) {
        static FaceId dfids[12] = { R, R, R, F, F, F, L, L, L, B, B, B };
        static PosId dpids[12] = { DR, DD, DL, DR, DD, DL, DR, DD, DL, DR, DD, DL };
        static Shift dss = { dfids, dpids, 12 };
        s = dss;
    }
    else if (m.fid == R) {
        static FaceId rfids[12] = { D, D, D, B, B, B, U, U, U, F, F, F };
        static PosId rpids[12] = { UR, RR, DR, UL, LL, DL, UR, RR, DR, UR, RR, DR };
        static Shift rss = { rfids, rpids, 12 };
        s = rss;
    }
    else if (m.fid == L) {
        static FaceId lfids[12] = { U, U, U, B, B, B, D, D, D, F, F, F };
        static PosId lpids[12] = { DL, LL, UL, DR, RR, UR, DL, LL, UL, DL, LL, UL };
        static Shift lss = { lfids, lpids, 12 };
        s = lss;
    }
    else if (m.fid == F) {
        static FaceId ffids[12] = { R, R, R, U, U, U, L, L, L, D, D, D };
        static PosId fpids[12] = { DL, LL, UL, DR, DD, DL, UR, RR, DR, UL, UU, UR };
        static Shift fss = { ffids, fpids, 12 };
        s = fss;
    }
    else if (m.fid == B) {
        static FaceId bfids[12] = { L, L, L, U, U, U, R, R, R, D, D, D };
        static PosId bpids[12] = { DL, LL, UL, UL, UU, UR, UR, RR, DR, DR, DD, DL };
        static Shift bss = { bfids, bpids, 12 };
        s = bss;
    }
    else if (m.fid == X) {
        static FaceId xfids[12] = { D, D, D, B, B, B, U, U, U, F, F, F };
        static PosId xpids[12] = { UU, CC, DD, DD, CC, UU, UU, CC, DD, UU, CC, DD };
        static Shift xss = { xfids, xpids, 12 };
        s = xss;
    }
    else if (m.fid == Y) {
        static FaceId yfids[12] = { L, L, L, F, F, F, R, R, R, B, B, B };
        static PosId ypids[12] = { LL, CC, RR, LL, CC, RR, LL, CC, RR, LL, CC, RR };
        static Shift yss = { yfids, ypids, 12 };
        s = yss;
    }
    else if (m.fid == Z) {
        static FaceId zfids[12] = { R, R, R, U, U, U, L, L, L, D, D, D };
        static PosId zpids[12] = { DD, CC, UU, RR, CC, LL, UU, CC, DD, LL, CC, RR };
        static Shift zss = { zfids, zpids, 12 };
        s = zss;
    }
    c = shiftColors(c, s.fids, s.pids, s.n, 3, m.b_i);

    if (!m.b_c) {   // rotate faces
        PosId pids[NUM_POS-1] = { LL, DL, DD, DR, RR, UR, UU, UL };
        FaceId fids[NUM_POS-1];
        int i;
        for (i=0 ; i<NUM_POS-1 ; i++) {
            fids[i] = m.fid;
        }
        c = shiftColors(c, fids, pids, NUM_POS-1, 2, m.b_i);
    } 
    else {          // recursively apply moves for cube rotations
        Move m1, m2;

        if (m.fid == X) {
            Move xm1 = { R, 0, m.b_d, m.b_i };
            Move xm2 = { L, 0, m.b_d, !m.b_i };
            m1 = xm1;
            m2 = xm2;
        }
        else if (m.fid == Y) {
            Move ym1 = { U, 0, m.b_d, m.b_i };
            Move ym2 = { D, 0, m.b_d, !m.b_i };
            m1 = ym1;
            m2 = ym2;
        }
        else if (m.fid == Z) {
            Move zm1 = { F, 0, m.b_d, m.b_i };
            Move zm2 = { B, 0, m.b_d, !m.b_i };
            m1 = zm1;
            m2 = zm2;
        }

        c = applyMove(c, m1);
        c = applyMove(c, m2);
    }

    return c;
}

Cube applyMoves(Cube c, Move *ms, int n) {
    int i;
    for (i=0 ; i<n ; i++) {
        c = applyMove(c, ms[i]);
    }
    return c;
}

Cube solvedCubeFactory() {
    Cube c;
    ColorId cids[NUM_FACES] = { YELLOW, WHITE, RED, ORANGE, BLUE, GREEN };
    int fid, pid;
    for (fid=0 ; fid<NUM_FACES ; fid++) {
        for (pid=0 ; pid<NUM_POS ; pid++) {
            c = writeColor(c, fid, pid, cids[fid]);
        }
    }
    return c;
}

Cube scrambledCubeFactory(Move *ms, int n) {
    Cube c = solvedCubeFactory();
    int i;
    for (i=0 ; i<n ; i++) {
        Move m = { NUM_FACES-1, 0, 0, 0 }; // TODO: randomize
        c = applyMove(c, (ms[i] = m));
    }
    return c;
}

Cube savedCubeFactory(char *str);

char *saveCube(Cube c, char *str, int n);

int isSolved(Cube c) {
    int fid, pid, cid;
    for (fid=0 ; fid<NUM_FACES ; fid++) {
        cid = readColor(c, fid, 0);
        for (pid=1 ; pid<NUM_POS ; pid++) {
            if (cid != readColor(c, fid, pid)) {
                return 0;
            }
        }
    }
    return 1;
}

void printCube(Cube c) {
    static char *symbol = "▣";
    printf("┌──┤U├──┬──┤L├──┬──┤F├──┬──┤R├──┬──┤B├──┬──┤D├──┐\n");

    FaceId fids[NUM_FACES] = { U, L, F, R, B, D };
    PosId pids[3];
    ColorId cid;

    int i, fid, pid;
    for (i=0 ; i<3; i++) {
        printf("│ ");
        for (fid=0 ; fid<NUM_FACES; fid++) {
            if (i == 0) {
                pids[0] = UL; pids[1] = UU; pids[2] = UR;
            }
            else if (i == 1) {
                pids[0] = LL; pids[1] = CC; pids[2] = RR;
            }
            else if (i == 2) {
                pids[0] = DL; pids[1] = DD; pids[2] = DR;
            }

            for (pid=0 ; pid<3 ; pid++) {
                cid = readColor(c, fids[fid], pids[pid]);
                if (cid == WHITE) {
                    printf("\033[0;37m");
                }
                else if (cid == YELLOW) {
                    printf("\033[0;33m");
                }
                else if (cid == RED) {
                    printf("\033[0;31m");
                }
                else if (cid == ORANGE) {
                    printf("\e[0;35m");
                }
                else if (cid == BLUE) {
                    printf("\033[0;34m");
                }
                else if (cid == GREEN) {
                    printf("\033[0;32m");
                }
                printf(symbol);
                printf("\033[0m ");
            }
            
            if (fid<NUM_FACES-1) {
                printf("│ ");
            }
        }
        printf("│\n");
    }
    printf("└───────┴───────┴───────┴───────┴───────┴───────┘\n");
}

// int main() {
//     Cube c0 = solvedCubeFactory();

//     int n = 6, stride = 1;
//     FaceId fids[] = { U, L, R, B, D, F };
//     PosId pids[] = { UU, LL, RR, UU, DL, DR };

//     Cube c1 = applyMove(c0, M_Z);
//     Cube c2 = applyMove(c1, M_D);

//     // scramble
//     // Move moves[] = { M_U, M_U, M_DI, M_DI, M_R, M_R, M_LI, M_LI, M_F, M_F, M_BI, M_BI };
//     // Cube c1 = applyMoves(c0, moves, sizeof(moves) / sizeof(moves[0]));
//     // printf("DEBUG 1: %d %d %d\n", readColor(c1.fs[U], CC), readColor(c1.fs[U], UL), readColor(c1.fs[U], UU));

//     // print
//     printf("\nOriginal cube:\n");
//     printCube(c0);
//     printf("%d\n", isSolved(c0));
//     printf("New cube:\n");
//     printCube(c1);
//     printf("%d\n", isSolved(c1));
//     printf("New cube:\n");
//     printCube(c2);
//     printf("%d\n", isSolved(c2));
// }