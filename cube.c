#include <stdlib.h>
#include <stdio.h>
#include "cube.h"

/**
 * This source file implements the state and behavior of a Rubik's cube.
 * 
 * TODO:
 * 1. fix rotateLines(). Direction and inverse are broken.
 * 2. write saveCube() and loadCube() functions.
 */

/* TYPE DEFINITIONS & ENUMERATIONS */

/* Encoding for clockwise and counterclockwise directions.
 * Useful for symmetric transformation functions. */
typedef uint8_t Dir;
static Dir CW = 0, CCW = 1;

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
    return (U <= m && m <= B) ? CW : CCW;
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

static Cube rotateFaces(Cube c, Move m) {
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

// Face rotateLine(Face *f, Pos **l, Dir d) {
//     // Color c0[3], c1[3], c2[3], c3[3];

//     // readColors(f[0], l[0], c0, 3);
//     // readColors(f[1], l[1], c1, 3);
//     // readColors(f[2], l[2], c2, 3);
//     // readColors(f[3], l[3], c3, 3);

//     // f[0] = writeColors(f[0], l[0], (d == CW ? c1 : c3), 3);
//     // f[1] = writeColors(f[1], l[1], c2, 3);
//     // f[2] = writeColors(f[2], l[2], (d == CW ? c3 : c1), 3);
//     // f[3] = writeColors(f[3], l[3], c0, 3);

//     // return f;   

//     static int LINES_PER_RING = 4;
//     static int COLORS_PER_LINE = 3;

//     Color c[3];
//     int i;
//     for(i=0; i<LINES_PER_RING; i++) {
//         int j = (((d == CW) || (i % 2) ? i : LINES_PER_RING-i) + 1) % LINES_PER_RING;
//         f[i] = writeColors(f[i], l[i], readColors(f[j], l[j], c, COLORS_PER_LINE), COLORS_PER_LINE);
//     }
//     return f;
// }

// struct Ring {
//     Face f[4];
//     Pos *l[4];
// };

// Face *rotateRing(Face *f, Pos **l, Dir d) {

//     return f;
// }

static Cube rotateLines(Cube c, Move m) {
    Dir d = getDir(m);

    static int LINES = 4;   // number of lines in rotation

    /* Represents the linear positions that are swapped between faces in a rotation. */
    static Pos  UE[3] = { UL, UU, UR },
                DE[3] = { DL, DD, DR },
                RE[3] = { UR, RR, DR },
                LE[3] = { UL, LL, DL },
                LR[3] = { LL, CC, RR },
                UD[3] = { UU, CC, DD };

    // parallel arrays for rotation line per face
    // TODO: make this a struct
    Move x[LINES];  // faces with rotating lines
    Pos *l[LINES];  // lines corresponding to faces

    // struct {
    //     Move x[LINES];
    //     Pos *l[LINES];
    // } rotation;

    if (m == X || m == XI) {
        c = rotateLines(c, (d == CW ? R : RI));
        c = rotateLines(c, (d == CW ? LI : L));
        x[0] = F; l[0] = UD;
        x[1] = U; l[1] = UD;
        x[2] = B; l[2] = UD;
        x[3] = D; l[3] = UD;
    }
    else if (m == Y || m == YI) {
        c = rotateLines(c, (d == CW ? U : UI));
        c = rotateLines(c, (d == CW ? DI : D));
        x[0] = R; l[0] = LR;
        x[1] = F; l[1] = LR;
        x[2] = L; l[2] = LR;
        x[3] = B; l[3] = LR;
    }
    else if (m == Z || m == ZI) {
        c = rotateLines(c, (d == CW ? F : FI));
        c = rotateLines(c, (d == CW ? BI : B));
        x[0] = U; l[0] = LR;
        x[1] = R; l[1] = UD;
        x[2] = D; l[2] = LR;
        x[3] = L; l[3] = UD;        
    }
    else if (m == U || m == UI) {
        x[0] = L; l[0] = UE;
        x[1] = F; l[1] = UE;
        x[2] = R; l[2] = UE;
        x[3] = B; l[3] = UE;
    }
    else if (m == D || m == DI) {
        x[0] = R; l[0] = DE;
        x[1] = F; l[1] = DE;
        x[2] = L; l[2] = DE;
        x[3] = B; l[3] = DE;
    }
    else if (m == R || m == RI) {
        x[0] = D; l[0] = RE;
        x[1] = B; l[1] = RE;
        x[2] = U; l[2] = RE;
        x[3] = F; l[3] = RE;
    }
    else if (m == L || m == LI) {
        x[0] = U; l[0] = LE;
        x[1] = B; l[1] = LE;
        x[2] = D; l[2] = LE;
        x[3] = F; l[3] = LE;
    }
    else if (m == F || m == FI) {
        x[0] = U; l[0] = DE;
        x[1] = L; l[1] = RE;
        x[2] = D; l[2] = UE;
        x[3] = R; l[3] = LE;
    }
    else if (m == B || m == BI) {
        x[0] = U; l[0] = UE;
        x[1] = L; l[1] = LE;
        x[2] = D; l[2] = DE;
        x[3] = R; l[3] = RE;
    }

    Color tmp[3], buffer[3]; // buffer for reading colors
    int i;
    for(i=0; i<LINES; i++) {
        // save first read for last swap
        if (!i) readColors(c.f[x[i]], l[i], tmp, 3); 

        // compute swap index
        int j = ((((d == CW) || (i % 2)) ? i : LINES-i) + 1) % LINES;

        // write new line to buffer
        readColors(c.f[x[j]], l[j], buffer, 3);

        // write replace old line with new line
        c.f[x[i]] = writeColors(c.f[x[i]], l[i], (i < LINES-1 ? buffer : tmp), 3);
    }

    return c;
}

Cube transform(Cube c, Move m) {
    c = rotateFaces(c, m);
    c = rotateLines(c, m);
    return c;
}

static int isFaceSolved(Face f) {
    Color c = readColor(f, CC);
    int i;
    for (i=1; i<9; i++) {
        if (c != readColor(f, i)) {
            return 0;
        }
    }
    return 1;
}

int isSolved(Cube c) {
    int i;
    for (i=0; i<6; i++) {
        if (!isFaceSolved(c.f[i])) {
            return 0;
        }
    }
    return 1;
}

static Face faceFactory(Color c) {
    Face f;
    int i;
    for(i=0; i<9; i++) {
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

int main() {
    printf("%d bytes required to store cube state.\n", sizeof(Cube));

    Move moves[] = { U, DI, X };
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
