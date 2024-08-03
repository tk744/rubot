#include "rubot.h"

#define CUBIE_BITS 5

Cube cubeSolved() {
    Cube c = { 0, 0 };

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        c.edges |= i << (CUBIE_BITS * i);
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        c.corners |= i << (CUBIE_BITS * i);
    }

    // `c` = { 407901468851537952, 247132686368 }
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
        static CubieEnum ucps[4] = { UFL, UBL, UBR, UFR };
        edge_enums = ueps;
        corner_enums = ucps;
    }
    else if (m & D) {
        static CubieEnum deps[4] = { DF, DR, DB, DL };
        static CubieEnum dcps[4] = { DBL, DFL, DFR, DBR };
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
            CubieEnum old_enum = isEdge ? edge_enums[i] : corner_enums[i];
            CubieEnum new_enum = isEdge ? edge_enums[(i+1)%4] : corner_enums[(i+1)%4];

            // get old cubie
            Int8 cubie = getCubie(isEdge ? old_c.edges : old_c.corners, old_enum);

            // update orientation of cubie
            Int8 orientation = getOrientation(cubie, isEdge);
            if (isEdge && (m & (U|D))) {
                orientation = !orientation;
            }
            else if (!isEdge && (m & (U|D|R|L))) {
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

static Move randomMove(Move exclude) {
    Move m;
    // generate base move
    do {
        m = 1 << (rand() % 6);
    } while (m & exclude);

    // generate I or H modifier
    int mod = rand() % 3;
    m |= mod == 0 ? 0 : mod == 1 ? I : H;

    return m;
}

void setRandomMoves(Move *ms, int n) {
    int i;
    for(i=0 ; i<n ; i++) {
        *(ms+i) = randomMove(i == 0 ? NOP : *(ms+i-1));
    }
}

Cube scramble(Cube c, Move *ms, int n) {
    int i;
    for(i=0 ; i<n ; i++) {
        *(ms+i) = randomMove(i == 0 ? NOP : *(ms+i-1));
    }
    return applyMoves(c, ms, n);
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

int equalCubes(Cube c1, Cube c2) {
    return (c1.edges == c2.edges && c1.corners == c2.corners);
}