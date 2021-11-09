#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define COLOR_BITS 3

/* This source file implements the state and behavior of a Rubik's cube. */

/* TODO:
 * 1. Complete move() function.
 *    i.  Edge and corner translations.
 *    ii. X/Y/Z slice translations.
 * 2. Cube state save and load functions.
 * 3. Replace 0b111 instances using COLOR_BITS.
 * 4. Documentation about how constants relate to state. */ 

/* Global constants for cube data. 
 * Using type uint8_t instead of enum because it reduces cube state memory by 4x. */

typedef uint8_t Color, Square, Move;
typedef uint32_t Face;
typedef struct { Face U, D, L, R, F, B; } Cube;

/* Represents the set of colors. */
const Color     WHITE = 1, YELLOW = 2, RED = 3, \
                ORANGE = 4, BLUE = 5, GREEN = 6;

/* Represents the set of square positions on a face. */
const Square    CC = 0, UL = 1, UU = 2, UR = 3, RR = 4, \
                DR = 5, DD = 6, DL = 7, LL = 8;

/* Represents the set of valid transformations of the cube. */
const Move      U = 1,  UI = 2,     // face rotations
                D = 3,  DI = 4,
                L = 5,  LI = 6,
                R = 7,  RI = 8,
                F = 9,  FI = 10,
                B = 11, BI = 12,
                X = 13, XI = 14,    // cube rotations
                Y = 15, YI = 16,
                Z = 17, ZI = 18,
                NOP = 0;            // no rotation

Color readSquare(Face f, Square p) {
    return (f >> (COLOR_BITS * p)) & 0b111;
}

Face writeSquare(Face f, Square p, Color c) {
    Face clear_mask = ~(0b111 << (COLOR_BITS * p));
    Face write_mask = c << (COLOR_BITS * p);
    return f & clear_mask | write_mask;
}

Face writeSquares(Face f, Square *p, Color *c, int count) {
    int i;
    for (i=0 ; i < count ; i++ ) {
        f = writeSquare(f, p[i], c[i]);
    }
    return f;
}

Face rotateFaceClockwise(Face f) {
    Square old_squares[] = { CC, DL, LL };
    Square new_squares[] = { CC, UL, UU };
    
    Color colors[3];
    int i;
    for(i=0 ; i<3; i++) {
        colors[i] = readSquare(f, old_squares[i]);
    }
    
    f = f << (2*COLOR_BITS) | f >> (32 - (2*COLOR_BITS));
    f = writeSquares(f, new_squares, colors, 3);

    return f;
}

Face rotateFaceCClockwise(Face f) {
    Square old_squares[] = { CC, UL, UU };
    Square new_squares[] = { CC, DL, LL };
    
    Color colors[3];
    int i;
    for(i=0 ; i<3; i++) {
        colors[i] = readSquare(f, old_squares[i]);
    }
    
    f = f >> (2*COLOR_BITS) | f << (32 - (2*COLOR_BITS));
    f = writeSquares(f, new_squares, colors, 3);

    return f;
}

Cube transform(Cube c, Move mv) {
    // no transformation
    if (mv == NOP) {
        return c;
    }

    // face rotations, edge and corner translations
    if (mv == U || mv == Y) {
        c.U = rotateFaceClockwise(c.U);
    }
    else if (mv == UI || mv == YI) {
        c.U = rotateFaceCClockwise(c.U);
    }
    else if (mv == D || mv == YI) {
        c.D = rotateFaceClockwise(c.D);
    }
    else if (mv == DI || mv == Y) {
        c.D = rotateFaceCClockwise(c.D);
    }
    else if (mv == L || mv == XI) {
        c.L = rotateFaceClockwise(c.L);
    }
    else if (mv == LI || mv == X) {
        c.L = rotateFaceCClockwise(c.L);
    }
    else if (mv == R || mv == X) {
        c.R = rotateFaceClockwise(c.R);
    }
    else if (mv == RI || mv == XI) {
        c.R = rotateFaceCClockwise(c.R);
    }
    else if (mv == F || mv == Z) {
        c.F = rotateFaceClockwise(c.F);
    }
    else if (mv == FI || mv == ZI) {
        c.F = rotateFaceCClockwise(c.F);
    }
    else if (mv == B || mv == ZI) {
        c.B = rotateFaceClockwise(c.B);
    }
    else if (mv == BI || mv == Z) {
        c.B = rotateFaceCClockwise(c.B);
    }

    // middle slice translations
    if (mv == X) {

    }
    else if (mv == XI) {

    }
    else if (mv == Y) {

    }
    else if (mv == YI) {

    }
    else if (mv == Z) {

    }
    else if (mv == ZI) {

    }

    return c;
}

int isFaceSolved(Face f) {
    Color c = readSquare(f, CC);
    int i;
    for (i=1; i<9; i++) {
        if (c != readSquare(f, i)) {
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

void printFace(Face f) {
    printf("%d %d %d\n", readSquare(f, UL), readSquare(f, UU), readSquare(f, UR));
    printf("%d %d %d\n", readSquare(f, LL), readSquare(f, CC), readSquare(f, RR));
    printf("%d %d %d\n", readSquare(f, DL), readSquare(f, DD), readSquare(f, DR));
}

void printCube(Cube c) {
    printf("  U     L     F     R     B     D  ");

    printf("%d %d %d | ", readSquare(c.U, UL), readSquare(c.U, UU), readSquare(c.U, UR));
    printf("%d %d %d | ", readSquare(c.L, UL), readSquare(c.L, UU), readSquare(c.L, UR));
    printf("%d %d %d | ", readSquare(c.F, UL), readSquare(c.F, UU), readSquare(c.F, UR));
    printf("%d %d %d | ", readSquare(c.R, UL), readSquare(c.R, UU), readSquare(c.R, UR));
    printf("%d %d %d | ", readSquare(c.B, UL), readSquare(c.B, UU), readSquare(c.B, UR));
    printf("%d %d %d \n", readSquare(c.D, UL), readSquare(c.D, UU), readSquare(c.D, UR));

    printf("%d %d %d | ", readSquare(c.U, LL), readSquare(c.U, CC), readSquare(c.U, RR));
    printf("%d %d %d | ", readSquare(c.L, LL), readSquare(c.L, CC), readSquare(c.L, RR));
    printf("%d %d %d | ", readSquare(c.F, LL), readSquare(c.F, CC), readSquare(c.F, RR));
    printf("%d %d %d | ", readSquare(c.R, LL), readSquare(c.R, CC), readSquare(c.R, RR));
    printf("%d %d %d | ", readSquare(c.B, LL), readSquare(c.B, CC), readSquare(c.B, RR));
    printf("%d %d %d \n", readSquare(c.D, LL), readSquare(c.D, CC), readSquare(c.D, RR));

    printf("%d %d %d | ", readSquare(c.U, DL), readSquare(c.U, DD), readSquare(c.U, DR));
    printf("%d %d %d | ", readSquare(c.L, DL), readSquare(c.L, DD), readSquare(c.L, DR));
    printf("%d %d %d | ", readSquare(c.F, DL), readSquare(c.F, DD), readSquare(c.F, DR));
    printf("%d %d %d | ", readSquare(c.R, DL), readSquare(c.R, DD), readSquare(c.R, DR));
    printf("%d %d %d | ", readSquare(c.B, DL), readSquare(c.B, DD), readSquare(c.B, DR));
    printf("%d %d %d \n", readSquare(c.D, DL), readSquare(c.D, DD), readSquare(c.D, DR));
}

int main() {
    Face u, d, l, r, f, b;
    writeSquare(u, CC, WHITE);

    Face f1 = 51396313;
    Face f2 = rotateFaceClockwise(f1);
    Face f3 = rotateFaceCClockwise(f1);
    printFace(f1);
    printf("\nClockwise:\n");
    printFace(f2);
    printf("\nCClockwise:\n");
    printFace(f3);
    printf("\nSolved: %d\n", isFaceSolved(f1));
    printf("%d bytes required to store cube state.", sizeof(Cube));
}
