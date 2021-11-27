#include "cube.h"

/*
TODO:
1. applyMove() update corner orientation
*/

/* FUNCTION IMPLEMENTATION */

Cube cubeFactory() {
    Int64 edges = 0, corners = 0;

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        edges |= i << (CUBIE_BITS * i);
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        corners |= i << (CUBIE_BITS * i);
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

    // update permutations and orientations of affected cubies
    Cube old_c = c;
    int i, isEdge;
    for (i=0 ; i<4 ; i++) {
        // alternate between edge and corner cubies
        for (isEdge=0 ; isEdge<2 ; isEdge++) {
            // get encoding index of old and new cubie
            CubieEnum old_enum, new_enum;
            old_enum = isEdge ? edge_enums[i] : corner_enums[i];
            new_enum = isEdge ? edge_enums[(i+1)%4] : corner_enums[(i+1)%4];

            // set all bits in clear mask
            Int64 clear_mask = ((1 << CUBIE_BITS) - 1);
 
            // set add mask to old cubie encoding
            Int64 add_mask = isEdge ? old_c.edges : old_c.corners;
            add_mask >>= CUBIE_BITS * old_enum;
            add_mask &= (1 << CUBIE_BITS) - 1;

            // update add mask to update cubie orientation
            if (isEdge && (m & (U|D))) {
                add_mask ^= 1 << (CUBIE_BITS - 1);
            }
            else if (!isEdge && 0) {
                // TODO: figure out how and when to change corner orientation
            }

            // shift masks into encoding index of new cubie
            clear_mask <<= CUBIE_BITS * new_enum;
            add_mask <<= CUBIE_BITS * new_enum;

            // update new cubie encoding by applying masks
            if (isEdge) {
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