#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define COLOR_BITS 3

typedef uint8_t Color, Pos, Move;
typedef uint32_t Face;
typedef struct { Face U, D, L, R, F, B; } Cube;

const Color WHITE = 1, YELLOW = 2, RED = 3, \
            ORANGE = 4, BLUE = 5, GREEN = 6;

const Pos   CC = 0, UL = 1, UU = 2, UR = 3, RR = 4, \
            DR = 5, DD = 6, DL = 7, LL = 8;

const Move  U=1,  UI=2,  // face rotations
            D=3,  DI=4,
            L=5,  LI=6,
            R=7,  RI=8,
            F=9,  FI=10,
            B=11, BI=12,
            X=13, XI=14, // cube rotations
            Y=15, YI=16,
            Z=17, ZI=18,
            NOP=0;       // no rotation

Color readFace(Face f, Pos p) {
    return (f >> (COLOR_BITS * p)) & 0b111;
}

Face writeFace(Face f, Pos p, Color c) {
    Face clear_mask = ~(0b111 << (COLOR_BITS * p));
    Face write_mask = c << (COLOR_BITS * p);
    return f & clear_mask | write_mask;
}

Face rotateFaceClockwise(Face f) {
    Color cc = readFace(f, CC);
    Color dl = readFace(f, DL), ll = readFace(f, LL);
    
    f = f << (2*COLOR_BITS) | f >> (32 - (2*COLOR_BITS));

    f = writeFace(f, CC, cc);
    f = writeFace(f, UL, dl);
    f = writeFace(f, UU, ll);

    return f;
}

Face rotateFaceCClockwise(Face f) {
    Color cc = readFace(f, CC);
    Color ul = readFace(f, UL), uu = readFace(f, UU);
    
    f = f >> (2*COLOR_BITS) | f << (32 - (2*COLOR_BITS));

    f = writeFace(f, CC, cc);
    f = writeFace(f, DL, ul);
    f = writeFace(f, LL, uu);

    return f;
}

Cube transform(Cube c, Move mv) {
    // no rotation
    if (mv == NOP) {
        return c;
    }

    // face rotations
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

    // edge and corner rotations
    
}

int isSolved(Cube c) {
    return 0;
}

void printFace(Face f) {
    printf("%d %d %d\n", readFace(f, UL), readFace(f, UU), readFace(f, UR));
    printf("%d %d %d\n", readFace(f, LL), readFace(f, CC), readFace(f, RR));
    printf("%d %d %d\n", readFace(f, DL), readFace(f, DD), readFace(f, DR));
}

void printCube(Cube c) {
    printf("  U     L     F     R     B     D  ");

    printf("%d %d %d | ", readFace(c.U, UL), readFace(c.U, UU), readFace(c.U, UR));
    printf("%d %d %d | ", readFace(c.L, UL), readFace(c.L, UU), readFace(c.L, UR));
    printf("%d %d %d | ", readFace(c.F, UL), readFace(c.F, UU), readFace(c.F, UR));
    printf("%d %d %d | ", readFace(c.R, UL), readFace(c.R, UU), readFace(c.R, UR));
    printf("%d %d %d | ", readFace(c.B, UL), readFace(c.B, UU), readFace(c.B, UR));
    printf("%d %d %d \n", readFace(c.D, UL), readFace(c.D, UU), readFace(c.D, UR));

    printf("%d %d %d | ", readFace(c.U, LL), readFace(c.U, CC), readFace(c.U, RR));
    printf("%d %d %d | ", readFace(c.L, LL), readFace(c.L, CC), readFace(c.L, RR));
    printf("%d %d %d | ", readFace(c.F, LL), readFace(c.F, CC), readFace(c.F, RR));
    printf("%d %d %d | ", readFace(c.R, LL), readFace(c.R, CC), readFace(c.R, RR));
    printf("%d %d %d | ", readFace(c.B, LL), readFace(c.B, CC), readFace(c.B, RR));
    printf("%d %d %d \n", readFace(c.D, LL), readFace(c.D, CC), readFace(c.D, RR));

    printf("%d %d %d | ", readFace(c.U, DL), readFace(c.U, DD), readFace(c.U, DR));
    printf("%d %d %d | ", readFace(c.L, DL), readFace(c.L, DD), readFace(c.L, DR));
    printf("%d %d %d | ", readFace(c.F, DL), readFace(c.F, DD), readFace(c.F, DR));
    printf("%d %d %d | ", readFace(c.R, DL), readFace(c.R, DD), readFace(c.R, DR));
    printf("%d %d %d | ", readFace(c.B, DL), readFace(c.B, DD), readFace(c.B, DR));
    printf("%d %d %d \n", readFace(c.D, DL), readFace(c.D, DD), readFace(c.D, DR));
}

int main() {
    Face u, d, l, r, f, b;
    writeFace(u, CC, WHITE);

    Face f1 = 51396313;
    Face f2 = rotateFaceCClockwise(f1);
    printFace(f1);
    printf("\n");
    printFace(f2);
    printf("%d bytes required to core cube cate.", sizeof(Cube));
}
