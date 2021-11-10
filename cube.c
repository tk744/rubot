#include <stdlib.h>
#include <stdio.h>
#include "cube.h"

/**
 * This source file implements the state and behavior of a Rubik's cube.
 * 
 * TODO:
 * 1. write saveCube() and loadCube() functions.
 */

/* MODEL REPRESENTATION CONSTANTS */

#define COLOR_BITS 3    // bits required to store a color value
#define LINE_SIZE 3     // number of positions in a line
#define RING_SIZE 4     // number of lines in a ring

/* TYPE DEFINITIONS & ENUMERATIONS */

/* Encoding for clockwise and counterclockwise directions.
 * Useful for symmetric transformation functions. */
typedef uint8_t Dir;
static Dir CW = 0, CCW = 1;

/* Ring encoded as parallel array of faces and lines. */
typedef struct {
    Move f[RING_SIZE];  // face indexes
    Pos *l[RING_SIZE];  // lines
} Ring;

typedef Pos Line[LINE_SIZE];
static Line UE = { UL, UU, UR },
            DE = { DL, DD, DR },
            RE = { UR, RR, DR },
            LE = { UL, LL, DL },
            LR = { LL, CC, RR },
            UD = { UU, CC, DD };

/* FUNCTION IMPLEMENTATION */

Color readColor(Face f, Pos p) {
    return (f >> (COLOR_BITS * p)) & 0b111;
}

Color *readColors(Face f, Pos *p, Color *c, int count) {
    int i;
    for(i=0 ; i<count ; i++) {
        c[i] = readColor(f, p[i]);
    }
    return c;
}

static Face writeColor(Face f, Pos p, Color c) {
    Face clear_mask = ~(0b111 << (COLOR_BITS * p));
    Face write_mask = c << (COLOR_BITS * p);
    return f & clear_mask | write_mask;
}

static Face writeColors(Face f, Pos *p, Color *c, int count) {
    int i;
    for(i=0 ; i<count ; i++ ) {
        f = writeColor(f, p[i], c[i]);
    }
    return f;
}

// static Move baseMove(Move m) {
//     if (m == U || m == UI) {
//         return U;
//     }
//     else if (m == D || m == DI) {
//         return D;
//     }
//     else if (m == R || m == RI) {
//         return R;
//     }
//     else if (m == L || m == LI) {
//         return L;
//     }
//     else if (m == F || m == FI) {
//         return F;
//     }
//     else if (m == B || m == BI) {
//         return B;
//     }
//     else if (m == X || m == XI) {
//         return X;
//     }
//     else if (m == Y || m == YI) {
//         return Y;
//     }
//     else if (m == Z || m == ZI) {
//         return Z;
//     }
// }

static Dir getDir(Move m) {
    return (U <= m && m <= Z) ? CW : CCW;
}

static Ring getBaseRing(Cube c, Move m) {
    if (m == U || m == UI) {
        Ring r = {{ F, L, B, R }, { UE, UE, UE, UE }};
        return r;
    }
    else if (m == D || m == DI) {
        Ring r = {{ F, R, B, L }, { DE, DE, DE, DE }};
        return r;
    }
    else if (m == R || m == RI) {
        Ring r = {{ F, U, B, D }, { RE, RE, RE, RE }};
        return r;
    }
    else if (m == L || m == LI) {
        Ring r = {{ F, D, B, U }, { LE, LE, LE, LE }};
        return r;     
    }
    else if (m == F || m == FI) {
        Ring r = {{ U, R, D, L }, { DE, LE, UE, RE }};
        return r;        
    }
    else if (m == B || m == BI) {
        Ring r = {{ U, L, D, R }, { UE, LE, DE, RE }};
        return r;        
    }
    else if (m == X || m == XI) {
        Ring r = {{ F, U, B, D }, { UD, UD, UD, UD }};
        return r;     
    }
    else if (m == Y || m == YI) {
        Ring r = {{ F, L, B, R }, { LR, LR, LR, LR }};
        return r;        
    }
    else if (m == Z || m == ZI) {
        Ring r = {{ U, R, D, L }, { LR, UD, LR, UD }};
        return r;        
    }
}

static Ring getRing(Cube c, Move m) {
    Ring r = getBaseRing(c, m);

    if (getDir(m) == CW) {
        Move tmp_f = r.f[1];
        r.f[1] = r.f[3];
        r.f[3] = tmp_f;

        Pos *tmp_l = r.l[1];
        r.l[1] = r.l[3];
        r.l[3] = tmp_l;
    }

    return r;
}

static Face rotateFace(Face f, Dir d) {
    Pos lsb[3] = { CC, UL, UU };
    Pos msb[3] = { CC, DL, LL };
    Color colors[3];
    readColors(f, (d == CW ? msb : lsb), colors, 3);

    if (d == CW) {
        f = f << (2*COLOR_BITS) | f >> (32 - (2*COLOR_BITS));
    }
    else {
        f = f >> (2*COLOR_BITS) | f << (32 - (2*COLOR_BITS));
    }
    f = writeColors(f, (d == CW ? lsb : msb), colors, 3);
    return f;
}

static Cube transformFaces(Cube c, Move m) {
    Dir d = getDir(m);
    
    if (U <= m && m <= B) {
        c.f[m] = rotateFace(c.f[m], d);
    }
    else if (UI <= m && m <= BI) {
        c.f[m-UI] = rotateFace(c.f[m-UI], d);
    }
    else if((X <= m && m <= Z) || (XI <= m && m <= ZI)) {
        Face f1, f2;

        if(m == X || m == XI) {
            f1 = R;
            f2 = L;
        }
        else if (m == Y || m == YI) {
            f1 = U;
            f2 = D;
        }
        else if (m == Z || m == ZI) {
            f1 = F;
            f2 = B;
        }

        c.f[f1] = rotateFace(c.f[f1], d);
        c.f[f2] = rotateFace(c.f[f2], 1-d);
    }
    return c;
}

static Cube transformRings(Cube c, Move m) {
    Ring r = getRing(c, m);

    Color tmp[LINE_SIZE]; // temp array for swapping first and last line colors
    Color buffer[LINE_SIZE]; // buffer array for reading line colors
    int i;
    for(i=0; i<RING_SIZE; i++) {
        // save first line for last swap
        if (!i) readColors(c.f[r.f[i]], r.l[i], tmp, LINE_SIZE); 
        // compute swap index
        int j = (i + 1) % RING_SIZE;
        // write new line to buffer
        readColors(c.f[r.f[j]], r.l[j], buffer, LINE_SIZE);
        // write replace old line with new line
        c.f[r.f[i]] = writeColors(c.f[r.f[i]], r.l[i], (i < RING_SIZE-1 ? buffer : tmp), LINE_SIZE);
    }

    if (m == X || m == XI) {
        c = transformRings(c, (getDir(m) == CW ? R : RI));
        c = transformRings(c, (getDir(m) == CW ? LI : L));
    }
    else if (m == Y || m == YI) {
        c = transformRings(c, (getDir(m) == CW ? U : UI));
        c = transformRings(c, (getDir(m) == CW ? DI : D));
    }
    else if (m == Z || m == ZI) {
        c = transformRings(c, (getDir(m) == CW ? F : FI));
        c = transformRings(c, (getDir(m) == CW ? BI : B));        
    }

    return c;
}

Cube transform(Cube c, Move m) {
    c = transformFaces(c, m);
    c = transformRings(c, m);
    return c;
}

static int isFaceSolved(Face f) {
    Color c = readColor(f, CC);
    int i;
    for (i=1; i<NUM_POSITIONS; i++) {
        if (c != readColor(f, i)) {
            return 0;
        }
    }
    return 1;
}

int isSolved(Cube c) {
    int i;
    for (i=0; i<NUM_FACES; i++) {
        if (!isFaceSolved(c.f[i])) {
            return 0;
        }
    }
    return 1;
}

static Face faceFactory(Color c) {
    Face f;
    int i;
    for(i=0; i<NUM_POSITIONS; i++) {
        f = writeColor(f, i, c);
    }
    return f;
}

Cube cubeFactory() {
    Cube c = {
        faceFactory(YELLOW),
        faceFactory(WHITE),
        faceFactory(RED),
        faceFactory(ORANGE),
        faceFactory(BLUE),
        faceFactory(GREEN)
    };
    return c;
}

Cube scramble(Cube c, Move *m, int n) {
    int i;
    for(i=0 ; i<n ; i++) {
        m[i] = rand() % NUM_MOVES;
        c = transform(c, m[i]);
    }
    return c;
}

static void printColor(Color c) {
    static char *symbol = "▣";
    if (c == WHITE) {
        printf("\033[0;37m");
    }
    else if (c == YELLOW) {
        printf("\033[0;33m");
    }
    else if (c == RED) {
        printf("\033[0;31m");
    }
    else if (c == ORANGE) {
        printf("\e[0;35m");
    }
    else if (c == BLUE) {
        printf("\033[0;34m");
    }
    else if (c == GREEN) {
        printf("\033[0;32m");
    }
    printf(symbol);
    printf("\033[0m");
}

void printCube(Cube c) {
    Face order[NUM_FACES] = { U, L, F, R, B, D };
    printf("┌──┤U├──┬──┤L├──┬──┤F├──┬──┤R├──┬──┤B├──┬──┤D├──┐\n");

    Color l, m, r;
    int i, j;
    for (i=0 ; i<3; i++) {
        printf("│");
        for (j=0 ; j<NUM_FACES; j++) {
            if (i == 0) {
                l = readColor(c.f[order[j]], UL);
                m = readColor(c.f[order[j]], UU);
                r = readColor(c.f[order[j]], UR);
            }
            else if (i == 1) {
                l = readColor(c.f[order[j]], LL);
                m = readColor(c.f[order[j]], CC);
                r = readColor(c.f[order[j]], RR);
            }
            else if (i == 2) {
                l = readColor(c.f[order[j]], DL);
                m = readColor(c.f[order[j]], DD);
                r = readColor(c.f[order[j]], DR);
            }

            printf(" ");
            printColor(l);
            printf(" ");
            printColor(m);
            printf(" ");
            printColor(r);
            printf(" ");
            
            if (j<NUM_FACES-1) {
                printf("│");
            }
        }
        printf("│\n");
    }
    printf("└───────┴───────┴───────┴───────┴───────┴───────┘\n");
}

// char *saveCube(Cube c, char *str) {

// }

// Cube loadCube(char *str) {

// }

int main() {
    printf("%d bytes required to store cube state.\n", sizeof(Cube));

    Move moves[] = { U, U, DI, DI, R, R, LI, LI, F, F, BI, BI };
    Cube c = cubeFactory();

    printCube(c);
    int i;
    for(i=0 ; i < sizeof(moves) / sizeof(moves[0]) ; i++) {
        c = transform(c, moves[i]);
        printCube(c);
    }

    // Move smoves[10];
    // Cube cnew = scramble(c, smoves, 10);
    // printCube(cnew);
}
