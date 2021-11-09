#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define COLOR_BITS 3

/* This source file implements the state and behavior of a Rubik's cube. */

/* TODO:
 * 1. Fix transform() function - problem with rotateEdge pointers probably.
 * 2. Cube state save and load functions.
 * 3. Replace 0b111 instances using COLOR_BITS.
 * 4. Documentation about how constants relate to state. */ 

/* Global constants for cube data. 
 * Using type uint8_t instead of enum because it reduces cube state memory by 4x. */

typedef uint8_t Color, Pos, Move, Dir;
typedef uint32_t Face;
typedef struct { Face U, D, L, R, F, B; } Cube;

/* Represents the set of colors. */
const Color WHITE = 1, YELLOW = 2, RED = 3, \
            ORANGE = 4, BLUE = 5, GREEN = 6;

/* Represents the set of positions on a face. */
const Pos   CC = 0, UL = 1, UU = 2, UR = 3, RR = 4, \
            DR = 5, DD = 6, DL = 7, LL = 8;

/* Represents the set of positions that can rotate together on a face. */
Pos         UE[3] = { UL, UU, UR },
            DE[3] = { DL, DD, DR },
            RE[3] = { UR, RR, DR },
            LE[3] = { UL, LL, DL },
            LR[3] = { LL, CC, RR },
            UD[3] = { UU, CC, DD };

/* Represents the set of valid transformations of the cube. */
const Move  U=1, D=2, L=3, R=4, F=5, B=6,   // face rotations
            X=7, Y=8, Z=9,                  // cube rotations
            NOP=0;                          // no transformation

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

Face writeColor(Face f, Pos p, Color c) {
    Face clear_mask = ~(0b111 << (COLOR_BITS * p));
    Face write_mask = c << (COLOR_BITS * p);
    return f & clear_mask | write_mask;
}

Face writeColors(Face f, Pos *p, Color *c, int count) {
    int i;
    for(i=0 ; i<count ; i++ ) {
        f = writeColor(f, p[i], c[i]);
    }
    return f;
}

Face rotateFace(Face f, Dir d) {
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

void rotateEdge(Face *f, Pos **p, Dir d) {
    Color c0[3], c1[3], c2[3], c3[3];

    readColors(f[0], p[0], c0, 3);
    readColors(f[1], p[1], c1, 3);
    readColors(f[2], p[2], c2, 3);
    readColors(f[3], p[3], c3, 3);

    f[0] = writeColors(f[0], p[0], (d == CW ? c1 : c3), 3);
    f[1] = writeColors(f[1], p[1], c2, 3);
    f[2] = writeColors(f[2], p[2], (d == CW ? c3 : c1), 3);
    f[3] = writeColors(f[3], p[3], c0, 3);
}

Cube transform(Cube c, Move m, Dir d) {
    if (m == NOP) {
        return c;
    }
    else if (m == X) {
        Face faces[4] = { c.F, c.U, c.B, c.D };
        Pos *edges[4] = { UD, UD, UD, UD };
        rotateEdge(faces, edges, d);
        c = transform(c, R, d);
        c = transform(c, L, 1-d);
    }
    else if (m == Y) {
        Face faces[4] = { c.R, c.F, c.L, c.B };
        Pos *edges[4] = { LR, LR, LR, LR };
        rotateEdge(faces, edges, d);
        c = transform(c, U, d);
        c = transform(c, D, 1-d);   
    }
    else if (m == Z) {
        Face faces[4] = { c.U, c.R, c.D, c.L };
        Pos *edges[4] = { LR, UD, LR, UD };
        rotateEdge(faces, edges, d);
        c = transform(c, F, d);
        c = transform(c, B, 1-d);
    }
    else if (m == U) {
        Face faces[4] = { c.R, c.F, c.L, c.B };
        Pos *edges[4] = { UE, UE, UE, UE };
        rotateEdge(faces, edges, d);
        c.U = rotateFace(c.U, d);
    }
    else if (m == D) {
        Face faces[4] = { c.L, c.F, c.R, c.B };
        Pos *edges[4] = { DE, DE, DE, DE };
        rotateEdge(faces, edges, d);
        c.D = rotateFace(c.D, d);
    }
    else if (m == R) {
        Face faces[4] = { c.U, c.B, c.D, c.F };
        Pos *edges[4] = { RE, RE, RE, RE };
        rotateEdge(faces, edges, d);
        c.R = rotateFace(c.R, d);
    }
    else if (m == L) {
        Face faces[4] = { c.U, c.F, c.D, c.B };
        Pos *edges[4] = { LE, LE, LE, LE };
        rotateEdge(faces, edges, d);
        c.L = rotateFace(c.L, d);
    }
    else if (m == F) {
        Face faces[4] = { c.U, c.R, c.D, c.L };
        Pos *edges[4] = { DE, LE, UE, RE };
        rotateEdge(faces, edges, d);
        c.F = rotateFace(c.F, d);
    }
    else if (m == B) {
        Face faces[4] = { c.U, c.L, c.D, c.R };
        Pos *edges[4] = { UE, LE, DE, RE };
        rotateEdge(faces, edges, d);
        c.B = rotateFace(c.B, d);
    }

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
    Face faces[] = { c.U, c.D, c.L, c.R, c.F, c.B };
    int i;
    for (i=0; i<6; i++) {
        if (!isFaceSolved(faces[i])) {
            return 0;
        }
    }
    return 1;
}

Face faceFactory(Color c) {
    Face f;
    Pos p[9] = { CC, UL, UU, UR, RR, DR, DD, DL, LL };
    Color colors[9] = { c, c, c, c, c, c, c, c, c };
    return writeColors(f, p, colors, 9);
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

    printf("%d %d %d | ", readColor(c.U, UL), readColor(c.U, UU), readColor(c.U, UR));
    printf("%d %d %d | ", readColor(c.L, UL), readColor(c.L, UU), readColor(c.L, UR));
    printf("%d %d %d | ", readColor(c.F, UL), readColor(c.F, UU), readColor(c.F, UR));
    printf("%d %d %d | ", readColor(c.R, UL), readColor(c.R, UU), readColor(c.R, UR));
    printf("%d %d %d | ", readColor(c.B, UL), readColor(c.B, UU), readColor(c.B, UR));
    printf("%d %d %d \n", readColor(c.D, UL), readColor(c.D, UU), readColor(c.D, UR));

    printf("%d %d %d | ", readColor(c.U, LL), readColor(c.U, CC), readColor(c.U, RR));
    printf("%d %d %d | ", readColor(c.L, LL), readColor(c.L, CC), readColor(c.L, RR));
    printf("%d %d %d | ", readColor(c.F, LL), readColor(c.F, CC), readColor(c.F, RR));
    printf("%d %d %d | ", readColor(c.R, LL), readColor(c.R, CC), readColor(c.R, RR));
    printf("%d %d %d | ", readColor(c.B, LL), readColor(c.B, CC), readColor(c.B, RR));
    printf("%d %d %d \n", readColor(c.D, LL), readColor(c.D, CC), readColor(c.D, RR));

    printf("%d %d %d | ", readColor(c.U, DL), readColor(c.U, DD), readColor(c.U, DR));
    printf("%d %d %d | ", readColor(c.L, DL), readColor(c.L, DD), readColor(c.L, DR));
    printf("%d %d %d | ", readColor(c.F, DL), readColor(c.F, DD), readColor(c.F, DR));
    printf("%d %d %d | ", readColor(c.R, DL), readColor(c.R, DD), readColor(c.R, DR));
    printf("%d %d %d | ", readColor(c.B, DL), readColor(c.B, DD), readColor(c.B, DR));
    printf("%d %d %d \n", readColor(c.D, DL), readColor(c.D, DD), readColor(c.D, DR));
}

int main() {
    Cube c = cubeFactory();
    printCube(c);

    Cube c1 = transform(c, D, CCW);
    printCube(c1);

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
