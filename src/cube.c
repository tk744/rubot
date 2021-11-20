#include <stdio.h>
#include "cube.h"

/*
TODO:
1. orientation handling in applyMove()
2. printCube()
3. moveToString()
*/

/* MODEL CONSTANTS */

#define EDGE_BITS 5
#define CORNER_BITS 5

/* FUNCTION IMPLEMENTATION */

Cube cubeFactory() {
    unsigned long edges1 = 0, edges2 = 0, corners1 = 0, corners2 = 0;

    int i;
    for(i=0 ; i<NUM_EDGES/2 ; i++) {
        edges1 |= i << (EDGE_BITS * i);
    }
    for(i=NUM_EDGES/2 ; i<NUM_EDGES ; i++) {
        edges2 |= i << (EDGE_BITS * (i-NUM_EDGES/2));
    }
    for(i=0 ; i<NUM_CORNERS/2 ; i++) {
        corners1 |= i << (CORNER_BITS * i);
    }
    for(i=NUM_CORNERS/2 ; i<NUM_CORNERS ; i++) {
        corners2 |= i << (CORNER_BITS * (i-NUM_CORNERS/2));
    }

    // 172066848 379887846 100384 235684
    Cube c = { edges1, edges2, corners1, corners2 };
    return c;
}

Cube applyMove(Cube c, Move m) {
    Cube old_c = c;

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

    if (m & I) {
        CubieEnum tmp_enum;

        tmp_enum = edge_enums[1];
        edge_enums[1] = edge_enums[3];
        edge_enums[3] = tmp_enum;

        tmp_enum = corner_enums[1];
        corner_enums[1] = corner_enums[3];
        corner_enums[3] = tmp_enum;
    }

    if (m * H) {
        // TODO
    }

    int i, b;
    for (i=0 ; i<4 ; i++) {
        for (b=0 ; b<2 ; b++) {
            CubieEnum old_enum, new_enum;
            old_enum = b ? edge_enums[i] : corner_enums[i];
            new_enum = b ? edge_enums[(i+1)%4] : corner_enums[(i+1)%4];

            int bits = b ? EDGE_BITS : CORNER_BITS;
            int num = (b ? NUM_EDGES : NUM_CORNERS)/2;

            unsigned long add_mask, clear_mask;

            add_mask = b ? ((old_enum < num) ? c.edges1 : c.edges2) :
                        ((old_enum < num) ? c.corners1 : c.corners2);
            add_mask >>= bits*(old_enum % num);
            add_mask &= ((1 << bits) -1);
            add_mask <<= bits*(new_enum % num);

            clear_mask = ((1 << bits) -1);
            clear_mask <<= bits*(new_enum % num);

            if (b) {
                if (new_enum < num) {
                    c.edges1 = (c.edges1 & ~clear_mask) | add_mask;
                }
                else {
                    c.edges2 = (c.edges2 & ~clear_mask) | add_mask;
                }
            }
            else {
                if (new_enum < num) {
                    c.corners1 = (c.corners1 & ~clear_mask) | add_mask;
                }
                else {
                    c.corners2 = (c.corners2 & ~clear_mask) | add_mask;
                }
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

void printCube(Cube c) {

}

char *moveToString(Move m) {
    
}

int main() {
    Cube c1 = cubeFactory();
    Cube c2 = applyMove(c1, R);

    printCube(c1);
    printCube(c2);

    printf("c1: %d %d %d %d\n", c1.edges1, c1.edges2, c1.corners1, c1.corners2);
    printf("c2: %d %d %d %d\n", c2.edges1, c2.edges2, c2.corners1, c2.corners2);
}
