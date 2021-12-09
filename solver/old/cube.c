/**
 * This file implements the state and behavior of a Rubik's cube.
 */

#include <stdio.h>
#include <stdlib.h>
#include "cube.h"

/* MODEL CONSTANTS */

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
    while (n-- > 0) {
        *cids++ = readColor(c, *fids++, *pids++);
    }
}

Cube writeColors(Cube c, FaceId *fids, PosId *pids, ColorId *cids, int n) {
    while (n-- > 0) {
        c = writeColor(c, *fids++, *pids++, *cids++);
    }
    return c;
}

static Cube shiftColors(Cube c, FaceId *fids, PosId *pids, int n, int stride, int b_i) {
    ColorId tmp[stride];
    ColorId cid;

    int i, i_c, i_n;
    for (i=0 ; i<n ; i++) {
        // get current and next color index
        i_c = (!b_i ? i : n-1-i);
        i_n = (!b_i ? i+stride : n-1-(i+stride));

        // store color from first stride into tmp
        if (i<stride) {
            tmp[i] = readColor(c, fids[i_c], pids[i_c]);
        }

        // read colors from next stride into buffer
        cid = (i_n>n-1 || i_n<0) ? 
            tmp[i%stride] : readColor(c, fids[i_n], pids[i_n]);

        // write color from buffer into current stride
        c = writeColor(c, fids[i_c], pids[i_c], cid);
    }

    return c;
}

Cube applyMove(Cube c, Move m) {
    // shift lines of colors around cube
    FaceId *fids;
    PosId *pids;
    if (m.fid == U) {
        static FaceId ufids[12] = { L, L, L, F, F, F, R, R, R, B, B, B };
        static PosId upids[12] = { UL, UU, UR, UL, UU, UR, UL, UU, UR, UL, UU, UR };
        fids = ufids;
        pids = upids;
    }
    else if (m.fid == D) {
        static FaceId dfids[12] = { R, R, R, F, F, F, L, L, L, B, B, B };
        static PosId dpids[12] = { DR, DD, DL, DR, DD, DL, DR, DD, DL, DR, DD, DL };
        fids = dfids;
        pids = dpids;
    }
    else if (m.fid == R) {
        static FaceId rfids[12] = { D, D, D, B, B, B, U, U, U, F, F, F };
        static PosId rpids[12] = { UR, RR, DR, DL, LL, UL, UR, RR, DR, UR, RR, DR };
        fids = rfids;
        pids = rpids;
    }
    else if (m.fid == L) {
        static FaceId lfids[12] = { U, U, U, B, B, B, D, D, D, F, F, F };
        static PosId lpids[12] = { UL, LL, DL, DR, RR, UR, UL, LL, DL, UL, LL, DL };
        fids = lfids;
        pids = lpids;
    }
    else if (m.fid == F) {
        static FaceId ffids[12] = { R, R, R, U, U, U, L, L, L, D, D, D };
        static PosId fpids[12] = { DL, LL, UL, DR, DD, DL, UR, RR, DR, UL, UU, UR };
        fids = ffids;
        pids = fpids;
    }
    else if (m.fid == B) {
        static FaceId bfids[12] = { L, L, L, U, U, U, R, R, R, D, D, D };
        static PosId bpids[12] = { DL, LL, UL, UL, UU, UR, UR, RR, DR, DR, DD, DL };
        fids = bfids;
        pids = bpids;
    }
    else if (m.fid == X) {
        static FaceId xfids[12] = { D, D, D, B, B, B, U, U, U, F, F, F };
        static PosId xpids[12] = { UU, CC, DD, DD, CC, UU, UU, CC, DD, UU, CC, DD };
        fids = xfids;
        pids = xpids;
    }
    else if (m.fid == Y) {
        static FaceId yfids[12] = { L, L, L, F, F, F, R, R, R, B, B, B };
        static PosId ypids[12] = { LL, CC, RR, LL, CC, RR, LL, CC, RR, LL, CC, RR };
        fids = yfids;
        pids = ypids;
    }
    else if (m.fid == Z) {
        static FaceId zfids[12] = { R, R, R, U, U, U, L, L, L, D, D, D };
        static PosId zpids[12] = { DD, CC, UU, RR, CC, LL, UU, CC, DD, LL, CC, RR };
        fids = zfids;
        pids = zpids;
    }
    c = shiftColors(c, fids, pids, 12, 3, m.b_i);

    if (!m.b_c) {   // if face rotation, rotate face
        FaceId fid = m.fid;
        FaceId fids[NUM_POS-1] = { fid, fid, fid, fid, fid, fid, fid, fid };
        static PosId fr_pids[NUM_POS-1] = { LL, DL, DD, DR, RR, UR, UU, UL };
        c = shiftColors(c, fids, fr_pids, NUM_POS-1, 2, m.b_i);
    } 
    else {          // if cube rotation, apply appropriate face rotations
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

    if (m.b_d) {    // apply double move
        Move m1 = { m.fid, m.b_c, 0, m.b_i };
        c = applyMove(c, m1);
    }

    return c;
}

Cube applyMoves(Cube c, Move *ms, int n) {
    while(n-- > 0) {
        c = applyMove(c, *ms++);
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
        ms[i].fid = rand() % (NUM_FACES);
        ms[i].b_c = 0;
        ms[i].b_d = rand() % 2;
        ms[i].b_i = rand() % 2;
    }
    c = applyMoves(c, ms, n);
    return c;
}

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

void printMove(Move m) {
    if (m.fid == U) {
        printf("U");
    }
    else if (m.fid == D) {
        printf("D");
    }
    else if (m.fid == R) {
        printf("R");
    }
    else if (m.fid == L) {
        printf("L");
    }
    else if (m.fid == F) {
        printf("F");
    }
    else if (m.fid == B) {
        printf("B");
    }
    else if (m.fid == X) {
        printf("X");
    }
    else if (m.fid == Y) {
        printf("Y");
    }
    else if (m.fid == Z) {
        printf("Z");
    }

    if (m.b_d) {
        printf("2");
    }

    if (m.b_i) {
        printf("'");
    }
}

void printMoves(Move *ms, int n) {
    while (n-- > 0) {
        printMove(*(ms++));
        printf(" ");
    }
    printf("\n");
}
