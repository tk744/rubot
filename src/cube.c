#include <stdint.h>
#include <stdio.h>

#define NUM_EDGES 12
#define NUM_CORNERS 8

#define EDGE_BITS 5
#define CORNER_BITS 5

typedef unsigned char CubieId, MoveId;

typedef unsigned char Move;

static const CubieId UF=0, UB=1, UR=2, UL=3, FR=4, FL=5, DF=6, DB=7, DR=8, DL=9, BR=10, BL=11;
static const CubieId UFR=0, UFL=1, UBR=2, UBL=3, DFR=4, DFL=5, DBR=6, DBL=7;

MoveId U=1, D=2, F=4, B=8, R=16, L=32, I=64, H=128;

typedef struct {
    unsigned long edges1, edges2;
    unsigned long corners1, corners2;
} Cube;

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

Cube apply(Cube c, Move m) {
    Cube old_c = c;

    CubieId *edge_permutations;
    CubieId *corner_permutations;
    // int *edge_orientations, *corner_orientations;

    if (m & U) {
        static CubieId ueps[4] = { UF, UL, UB, UR };
        // static int ueos[4]      = {};
        static CubieId ucps[4] = { UFR, UFL, UBL, UBR };
        // static int ucos[4]      = {};
        edge_permutations = ueps;
        // edge_orientations = ueos;
        corner_permutations = ucps;
        // corner_orientations = ucos;
    }
    else if (m & D) {
        static CubieId deps[4] = { DF, DR, DB, DL };
        // static int deos[4]      = {};
        static CubieId dcps[4] = { DFL, DFR, DBR, DBL };
        // static int dcos[4]      = {};
        edge_permutations = deps;
        // edge_orientations = deos;
        corner_permutations = dcps;
        // corner_orientations = dcos;
    }
    else if (m & F) {
        static CubieId feps[4] = { UF, FR, DF, FL };
        // static int feos[4]      = {};
        static CubieId fcps[4] = { UFL, UFR, DFR, DFL };
        // static int fcos[4]      = {};
        edge_permutations = feps;
        // edge_orientations = feos;
        corner_permutations = fcps;
        // corner_orientations = fcos;
    }
    else if (m & B) {
        static CubieId beps[4] = { UB, BL, DB, BR };
        // static int beos[4]      = {};
        static CubieId bcps[4] = { UBR, UBL, DBL, DBR };
        // static int bcos[4]      = {};
        edge_permutations = beps;
        // edge_orientations = beos;
        corner_permutations = bcps;
        // corner_orientations = bcos;
    }
    else if (m & R) {
        static CubieId reps[4] = { UR, BR, DR, FR };
        // static int reos[4]      = {};
        static CubieId rcps[4] = { UFR, UBR, DBR, DFR };
        // static int rcos[4]      = {};
        edge_permutations = reps;
        // edge_orientations = reos;
        corner_permutations = rcps;
        // corner_orientations = rcos;
    }
    else if (m & L) {
        static CubieId leps[4] = { UL, FL, DL, BL };
        // static int leos[4]      = {};
        static CubieId lcps[4] = { UBL, UFL, DFL, DBL };
        // static int lcos[4]      = {};
        edge_permutations = leps;
        // edge_orientations = leos;
        corner_permutations = lcps;
        // corner_orientations = lcos;
    }
    else {
        return c;
    }

    if (m & I) {
        CubieId tmp_eid = edge_permutations[1];
        edge_permutations[1] = edge_permutations[3];
        edge_permutations[3] = tmp_eid;

        CubieId tmp_cid = corner_permutations[1];
        corner_permutations[1] = corner_permutations[3];
        corner_permutations[3] = tmp_cid;
    }

    if (m * H) {
        // TODO
    }

    int i, b;
    for (i=0 ; i<4 ; i++) {
        for (b=0 ; b<2 ; b++) {
            CubieId old_id, new_id;
            old_id = b ? edge_permutations[i] : corner_permutations[i];
            new_id = b ? edge_permutations[(i+1)%4] : corner_permutations[(i+1)%4];

            unsigned long add_mask, clear_mask;

            int bits = b ? EDGE_BITS : CORNER_BITS;
            int num = (b ? NUM_EDGES : NUM_CORNERS)/2;

            add_mask = b ?  ((old_id < num) ? c.edges1 : c.edges2) :
                            ((old_id < num) ? c.corners1 : c.corners2);
            add_mask >>= bits*(old_id % num);
            add_mask &= ((1 << bits) -1);
            add_mask <<= bits*(new_id % num);

            clear_mask = ((1 << bits) -1);
            clear_mask <<= bits*(new_id % num);

            if (b) {
                if (new_id < num) {
                    c.edges1 = (c.edges1 & ~clear_mask) | add_mask;
                }
                else {
                    c.edges2 = (c.edges2 & ~clear_mask) | add_mask;
                }
            }
            else {
                if (new_id < num) {
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

int main() {
    Cube c1 = cubeFactory();
    Cube c2 = c1;

    int i;
    for (i=0 ; i<2000000 ; i++) {
        c2 = apply(c2, R);
    }

    printf("c1: %d %d %d %d\n", c1.edges1, c1.edges2, c1.corners1, c1.corners2);
    printf("c2: %d %d %d %d\n", c2.edges1, c2.edges2, c2.corners1, c2.corners2);
}
