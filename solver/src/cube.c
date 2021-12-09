#include "cube.h"

/* FUNCTION IMPLEMENTATION */

Cube cubeFactory() {
    Cube c = { 0, 0 };

    // set `c` to { 407901468851537952, 247132686368 }
    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        c.edges |= i << (CUBIE_BITS * i);
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        c.corners |= i << (CUBIE_BITS * i);
    }

    return c;
}

Int8 getCubie(Int64 encoding, CubieEnum ce) {
    return (encoding >> (CUBIE_BITS * ce)) & ((1 << CUBIE_BITS) - 1);
}

Int8 getPermutation(Int8 cubie, int isEdge) {
    return cubie & ((1 << (CUBIE_BITS - (isEdge ? 1 : 2))) - 1);
}

Int8 getOrientation(Int8 cubie, int isEdge) {
    return cubie >> (CUBIE_BITS - (isEdge ? 1 : 2));
}

void setCubie(Int64 *encoding, CubieEnum ce, Int8 cubie) {
    *encoding &= ~((((Int64) 1 << CUBIE_BITS) - 1) << (CUBIE_BITS * ce));
    *encoding |= (Int64) cubie << (CUBIE_BITS * ce);
}

void setPermutation(Int8 *cubie, int isEdge, Int8 permutation) {
    *cubie &= ~((1 << (CUBIE_BITS - (isEdge ? 1 : 2))) - 1);
    *cubie |= permutation;
}

void setOrientation(Int8 *cubie, int isEdge, Int8 orientation) {
    Int8 permutation = getPermutation(*cubie, isEdge);
    *cubie = orientation << (CUBIE_BITS - (isEdge ? 1 : 2));
    setPermutation(cubie, isEdge, permutation);
}

Cube applyMove(Cube c, Move m) {
    // recursive calls for double and inverse rotations
    if (m & MV_MASK_H) {
        m &= ~(MV_MASK_H|MV_MASK_I);
        c = applyMove(c, m);
        c = applyMove(c, m);
        return c;
    }
    else if (m & MV_MASK_I) {
        m &= ~(MV_MASK_H|MV_MASK_I);
        c = applyMove(c, m|MV_MASK_H);
        c = applyMove(c, m);
        return c;
    }

    // define cubies affected by base move
    CubieEnum *edge_enums;
    CubieEnum *corner_enums;
    if (m & MV_MASK_U) {
        static CubieEnum ueps[4] = { ECUBIE_UF, ECUBIE_UL, ECUBIE_UB, ECUBIE_UR };
        static CubieEnum ucps[4] = { CCUBIE_UFL, CCUBIE_UBL, CCUBIE_UBR, CCUBIE_UFR };
        edge_enums = ueps;
        corner_enums = ucps;
    }
    else if (m & MV_MASK_D) {
        static CubieEnum deps[4] = { ECUBIE_DF, ECUBIE_DR, ECUBIE_DB, ECUBIE_DL };
        static CubieEnum dcps[4] = { CCUBIE_DBL, CCUBIE_DFL, CCUBIE_DFR, CCUBIE_DBR };
        edge_enums = deps;
        corner_enums = dcps;
    }
    else if (m & MV_MASK_F) {
        static CubieEnum feps[4] = { ECUBIE_UF, ECUBIE_FR, ECUBIE_DF, ECUBIE_FL };
        static CubieEnum fcps[4] = { CCUBIE_UFL, CCUBIE_UFR, CCUBIE_DFR, CCUBIE_DFL };
        edge_enums = feps;
        corner_enums = fcps;
    }
    else if (m & MV_MASK_B) {
        static CubieEnum beps[4] = { ECUBIE_UB, ECUBIE_BL, ECUBIE_DB, ECUBIE_BR };
        static CubieEnum bcps[4] = { CCUBIE_UBR, CCUBIE_UBL, CCUBIE_DBL, CCUBIE_DBR };
        edge_enums = beps;
        corner_enums = bcps;
    }
    else if (m & MV_MASK_R) {
        static CubieEnum reps[4] = { ECUBIE_UR, ECUBIE_BR, ECUBIE_DR, ECUBIE_FR };
        static CubieEnum rcps[4] = { CCUBIE_UFR, CCUBIE_UBR, CCUBIE_DBR, CCUBIE_DFR };
        edge_enums = reps;
        corner_enums = rcps;
    }
    else if (m & MV_MASK_L) {
        static CubieEnum leps[4] = { ECUBIE_UL, ECUBIE_FL, ECUBIE_DL, ECUBIE_BL };
        static CubieEnum lcps[4] = { CCUBIE_UBL, CCUBIE_UFL, CCUBIE_DFL, CCUBIE_DBL };
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
            CubieEnum old_enum = isEdge ? edge_enums[i] : corner_enums[i];
            CubieEnum new_enum = isEdge ? edge_enums[(i+1)%4] : corner_enums[(i+1)%4];

            // get old cubie
            Int8 cubie = getCubie(isEdge ? old_c.edges : old_c.corners, old_enum);

            // update orientation of cubie
            Int8 orientation = getOrientation(cubie, isEdge);
            if (isEdge && (m & (MV_MASK_U|MV_MASK_D))) {
                orientation = !orientation;
            }
            else if (!isEdge && (m & (MV_MASK_U|MV_MASK_D|MV_MASK_R|MV_MASK_L))) {
                orientation += (i % 2) ? 2 : 1;
                orientation %= 3;
            }
            setOrientation(&cubie, isEdge, orientation);

            // set new cubie
            setCubie(isEdge ? &c.edges : &c.corners, new_enum, cubie);
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

static Move randomMove(MoveMask exclude) {
    Move m;

    // generate base move
    do {
        m = 1 << (rand() % 6);
    } while (m & exclude);

    // generate I or H modifier
    int mod = rand() % 3;
    m |= mod == 0 ? 0 : mod == 1 ? MV_MASK_I : MV_MASK_H;

    return m;
}


Cube scramble(Cube c, Move *ms, int n) {
    int i;
    for(i=0 ; i<n ; i++) {
        *(ms+i) = randomMove(i == 0 ? 0 : *(ms+i-1));
    }
    return applyMoves(c, ms, n);
}

int areEqual(Cube c1, Cube c2) {
    return (c1.edges == c2.edges && c1.corners == c2.corners);
}

Move inverseMove(Move m) {
    return (m & MV_MASK_H) ? m : m^MV_MASK_I;
}