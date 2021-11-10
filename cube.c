#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This source file implements the state and behavior of a Rubik's cube. */

/* Model representation constants. */

#define COLOR_BITS 3

/* Face (0-5) and Move (0-8) values. */

#define U 0
#define D 1
#define L 2
#define R 3
#define F 4
#define B 5
#define X 6
#define Y 7
#define Z 8

/* Type Definitions. */

/* Global constants for cube data. 
 * Using type uint8_t instead of enum because it reduces cube state memory by 4x.  */

typedef uint8_t Color, Pos;
typedef uint32_t Face;
typedef struct { Face f[6]; } Cube;

typedef uint8_t Move, Dir;


/* TODO:
 * 1. Fix transform() function - problem with rotateEdge pointers probably.
 * 2. Cube state save and load functions.
 * 3. Replace 0b111 instances using COLOR_BITS.
 * 4. Documentation about how constants relate to state. */


/* Represents the set of colors. */
const Color WHITE = 1, YELLOW = 2, RED = 3, \
            ORANGE = 4, BLUE = 5, GREEN = 6;

/* Represents the set of positions on a face. */
const Pos   CC = 0, UL = 1, UU = 2, UR = 3, RR = 4, \
            DR = 5, DD = 6, DL = 7, LL = 8;

/* Represents the set of positions in a line that can rotate together on a face. */
Pos         UE[3] = { UL, UU, UR },
            DE[3] = { DL, DD, DR },
            RE[3] = { UR, RR, DR },
            LE[3] = { UL, LL, DL },
            LR[3] = { LL, CC, RR },
            UD[3] = { UU, CC, DD };

/* Represents the set of valid transformations of the cube. */
// const Move  U=1, D=2, L=3, R=4, F=5, B=6,   // face rotations
//             X=7, Y=8, Z=9,                  // cube rotations
//             NOP=0;                          // no transformation

const Dir   CW = 0, CCW = 1;    // clockwise, counterclockwise




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

static Cube rotateFaces(Cube c, Move m, Dir d) {
    if (m == U || m == D || m == R || m == L || m == F || m == B) {
        c.f[m] = rotateFace(c.f[m], d);
    }
    else if(m == X || m == Y || m == Z) {
        Face f1, f2;

        if(m == X) {
            f1 = R;
            f2 = L;
        }
        else if (m == Y) {
            f1 = U;
            f2 = D;
        }
        else if (m == Z) {
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

static Cube rotateLines(Cube c, Move m, Dir d) {
    uint8_t x[4];
    Pos *l[4];

    switch (m) {
        case X:
            // rotation.f = { F, U, B, D };
            // rotation.l = { UD, UD, UD, UD };
            c = rotateLines(c, R, d);
            c = rotateLines(c, L, 1-d);

            x[0] = F; l[0] = UD;
            x[1] = U; l[1] = UD;
            x[2] = B; l[2] = UD;
            x[3] = D; l[3] = UD;
            break;
        case Y:
            c = rotateLines(c, U, d);
            c = rotateLines(c, D, 1-d);
            x[0] = R; l[0] = LR;
            x[1] = F; l[1] = LR;
            x[2] = L; l[2] = LR;
            x[3] = B; l[3] = LR;
            break;
        case Z:
            c = rotateLines(c, F, d);
            c = rotateLines(c, B, 1-d);
            x[0] = U; l[0] = LR;
            x[1] = R; l[1] = UD;
            x[2] = D; l[2] = LR;
            x[3] = L; l[3] = UD;
            break;
        case U:
            x[0] = R; l[0] = UE;
            x[1] = F; l[1] = UE;
            x[2] = L; l[2] = UE;
            x[3] = B; l[3] = UE;
            break;
        case D:
            x[0] = L; l[0] = DE;
            x[1] = F; l[1] = DE;
            x[2] = R; l[2] = DE;
            x[3] = B; l[3] = DE;
            break;
        case R:
            x[0] = U; l[0] = RE;
            x[1] = B; l[1] = RE;
            x[2] = D; l[2] = RE;
            x[3] = F; l[3] = RE;
            break;
        case L:
            x[0] = U; l[0] = LE;
            x[1] = F; l[1] = LE;
            x[2] = D; l[2] = LE;
            x[3] = B; l[3] = LE;
            break;
        case F:
            x[0] = U; l[0] = DE;
            x[1] = R; l[1] = LE;
            x[2] = D; l[2] = UE;
            x[3] = L; l[3] = RE;
            break;
        case B:
            x[0] = U; l[0] = UE;
            x[1] = L; l[1] = LE;
            x[2] = D; l[2] = DE;
            x[3] = R; l[3] = RE;
            break;
    }

    static int LINES_PER_RING = 4;
    static int COLORS_PER_LINE = 3;

    Color tmp[3], buffer[3]; // buffer for reading colors
    int i;
    for(i=0; i<LINES_PER_RING; i++) {
        // save first read for last swap
        if (!i) readColors(c.f[x[i]], l[i], tmp, COLORS_PER_LINE); 

        // compute swap index
        int j = (((d == CW) || (i % 2) ? i : LINES_PER_RING-i) + 1) % LINES_PER_RING;

        // write new line to buffer
        readColors(c.f[x[j]], l[j], buffer, COLORS_PER_LINE);

        // write replace old line with new line
        c.f[x[i]] = writeColors(c.f[x[i]], l[i], (i < LINES_PER_RING-1 ? buffer : tmp), COLORS_PER_LINE);
    }

    return c;
}

Cube transform(Cube c, Move m, Dir d) {
    c = rotateFaces(c, m, d);
    c = rotateLines(c, m, d);
    return c;
}

int isFaceSolved(Face f) {
    Color c = readColor(f, CC);
    int i;
    for (i=1; i<9; i++) {
        if (c != readColor(f, i)) {
            return 0;
        }
    }
    return 1;
}

int isCubeSolved(Cube c) {
    int i;
    for (i=0; i<6; i++) {
        if (!isFaceSolved(c.f[i])) {
            return 0;
        }
    }
    return 1;
}

Face faceFactory(Color c) {
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

void printFace(Face f) {
    printf("%d %d %d\n", readColor(f, UL), readColor(f, UU), readColor(f, UR));
    printf("%d %d %d\n", readColor(f, LL), readColor(f, CC), readColor(f, RR));
    printf("%d %d %d\n", readColor(f, DL), readColor(f, DD), readColor(f, DR));
}

void printCube(Cube c) {
    printf("  U       L       F       R       B       D  \n");

    printf("%d %d %d | ", readColor(c.f[U], UL), readColor(c.f[U], UU), readColor(c.f[U], UR));
    printf("%d %d %d | ", readColor(c.f[L], UL), readColor(c.f[L], UU), readColor(c.f[L], UR));
    printf("%d %d %d | ", readColor(c.f[F], UL), readColor(c.f[F], UU), readColor(c.f[F], UR));
    printf("%d %d %d | ", readColor(c.f[R], UL), readColor(c.f[R], UU), readColor(c.f[R], UR));
    printf("%d %d %d | ", readColor(c.f[B], UL), readColor(c.f[B], UU), readColor(c.f[B], UR));
    printf("%d %d %d \n", readColor(c.f[D], UL), readColor(c.f[D], UU), readColor(c.f[D], UR));

    printf("%d %d %d | ", readColor(c.f[U], LL), readColor(c.f[U], CC), readColor(c.f[U], RR));
    printf("%d %d %d | ", readColor(c.f[L], LL), readColor(c.f[L], CC), readColor(c.f[L], RR));
    printf("%d %d %d | ", readColor(c.f[F], LL), readColor(c.f[F], CC), readColor(c.f[F], RR));
    printf("%d %d %d | ", readColor(c.f[R], LL), readColor(c.f[R], CC), readColor(c.f[R], RR));
    printf("%d %d %d | ", readColor(c.f[B], LL), readColor(c.f[B], CC), readColor(c.f[B], RR));
    printf("%d %d %d \n", readColor(c.f[D], LL), readColor(c.f[D], CC), readColor(c.f[D], RR));

    printf("%d %d %d | ", readColor(c.f[U], DL), readColor(c.f[U], DD), readColor(c.f[U], DR));
    printf("%d %d %d | ", readColor(c.f[L], DL), readColor(c.f[L], DD), readColor(c.f[L], DR));
    printf("%d %d %d | ", readColor(c.f[F], DL), readColor(c.f[F], DD), readColor(c.f[F], DR));
    printf("%d %d %d | ", readColor(c.f[R], DL), readColor(c.f[R], DD), readColor(c.f[R], DR));
    printf("%d %d %d | ", readColor(c.f[B], DL), readColor(c.f[B], DD), readColor(c.f[B], DR));
    printf("%d %d %d \n", readColor(c.f[D], DL), readColor(c.f[D], DD), readColor(c.f[D], DR));
}

int main() {
    Cube c = cubeFactory();
    printCube(c);

    Cube c1 = transform(c, D, CCW);
    printCube(c1);

    Cube c2 = transform(c1, U, CCW);
    printCube(c2);

    Cube c3 = transform(c2, F, CW);
    printCube(c3);

    Cube c4 = transform(c3, R, CW);
    printCube(c4);

    // Face u, d, l, r, f, b;
    // writeColor(u, CC, WHITE);

    // Face f1 = 51396313;
    // Face f2 = rotateFace(f1, CW);
    // Face f3 = rotateFace(f1, CCW);
    // printFace(f1);
    // printf("\nClockwise:\n");
    // printFace(f2);
    // printf("\nCClockwise:\n");
    // printFace(f3);
    // printf("\nSolved: %d\n", isFaceSolved(f1));
    // printf("%d bytes required to store cube state.", sizeof(Cube));
}
