#include <stdio.h>
#include "cube.h"

/*
TODO:
1. applyMove() orientations
2. printCube()
*/

/* MODEL CONSTANTS */

#define EDGE_BITS 5
#define CORNER_BITS 5

/* FUNCTION IMPLEMENTATION */

Cube cubeFactory() {
    Int64 edges = 0, corners = 0;

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        edges |= i << (EDGE_BITS * i);
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        corners |= i << (CORNER_BITS * i);
    }

    // 407901468851537952 247132686368
    Cube c = { edges, corners };
    return c;
}

Cube applyMove(Cube c, Move m) {
    // recursive calls for double and inverse rotations
    if (m & H) {
        m &= ~(H|I);
        c = applyMove(c, m);
        c = applyMove(c, m);
        return c;
    }
    else if (m & I) {
        m &= ~(H|I);
        c = applyMove(c, m|H);
        c = applyMove(c, m);
        return c;
    }

    // define cubies affected by base move
    CubieEnum *edge_enums;
    CubieEnum *corner_enums;
    if (m & U) {
        static CubieEnum ueps[4] = { UF, UL, UB, UR };
        static CubieEnum ucps[4] = { UFR, UFL, UBL, UBR };
        edge_enums = ueps;
        corner_enums = ucps;
    }
    else if (m & D) {
        static CubieEnum deps[4] = { DF, DR, DB, DL };
        static CubieEnum dcps[4] = { DFL, DFR, DBR, DBL };
        edge_enums = deps;
        corner_enums = dcps;
    }
    else if (m & F) {
        static CubieEnum feps[4] = { UF, FR, DF, FL };
        static CubieEnum fcps[4] = { UFL, UFR, DFR, DFL };
        edge_enums = feps;
        corner_enums = fcps;
    }
    else if (m & B) {
        static CubieEnum beps[4] = { UB, BL, DB, BR };
        static CubieEnum bcps[4] = { UBR, UBL, DBL, DBR };
        edge_enums = beps;
        corner_enums = bcps;
    }
    else if (m & R) {
        static CubieEnum reps[4] = { UR, BR, DR, FR };
        static CubieEnum rcps[4] = { UFR, UBR, DBR, DFR };
        edge_enums = reps;
        corner_enums = rcps;
    }
    else if (m & L) {
        static CubieEnum leps[4] = { UL, FL, DL, BL };
        static CubieEnum lcps[4] = { UBL, UFL, DFL, DBL };
        edge_enums = leps;
        corner_enums = lcps;
    }
    else {
        return c;
    }

    // rotate permutations and compute orientations of affected cubies
    Cube old_c = c;
    int i, b;
    for (i=0 ; i<4 ; i++) {
        for (b=0 ; b<2 ; b++) {
            CubieEnum old_enum, new_enum;
            old_enum = b ? edge_enums[i] : corner_enums[i];
            new_enum = b ? edge_enums[(i+1)%4] : corner_enums[(i+1)%4];

            int bits = b ? EDGE_BITS : CORNER_BITS;

            Int64 add_mask, clear_mask;

            add_mask = b ? old_c.edges : old_c.corners;
            add_mask >>= bits * old_enum;
            add_mask &= ((1 << bits) - 1);
            add_mask <<= bits * new_enum;

            clear_mask = ((1 << bits) - 1);
            clear_mask <<= bits * new_enum;

            if (b) {
                c.edges = (c.edges & ~clear_mask) | add_mask;
            }
            else {
                c.corners = (c.corners & ~clear_mask) | add_mask;
            }
        }
    }
    return c;
}

Cube applyMoves(Cube c, Move *ms, int n) {
    while(n-- > 0) {
        c = applyMove(c, *ms++);
    }
    return c;
}

int areEqual(Cube c1, Cube c2) {
    return (c1.edges == c2.edges && c1.corners == c2.corners);
}

void printCube(Cube c) {
    printf("%llu %llu\n", c.edges, c.corners);
}

void printMove(Move m) {
    if (m & U) {
        printf("U");
    }
    else if (m & D) {
        printf("D");
    }
    else if (m & F) {
        printf("F");
    }
    else if (m & B) {
        printf("B");
    }
    else if (m & R) {
        printf("R");
    }
    else if (m & L) {
        printf("L");
    }
    else {
        printf("NOP");
        return;
    }

    if (m & H) {
        printf("2");
    }
    else if (m & I) {
        printf("'");
    }
}

void printMoves(Move *ms, int n) {
    while(n-- > 0) {
        printMove(*ms++);
        printf(n == 0 ? "\n" : ", ");
    }
}