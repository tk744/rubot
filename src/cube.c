#include <stdio.h>
#include "cube.h"

/*
TODO:
1. applyMove() update corner orientation
2. getColor()
*/

/* MODEL CONSTANTS */

#define CUBIE_BITS 5

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

static Int8 getCubie(Int64 encoding, CubieEnum ce) {
    return (encoding >> (CUBIE_BITS * ce)) & ((1 << CUBIE_BITS) - 1);
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
            Int64 add_mask = getCubie((isEdge ? old_c.edges : old_c.corners), old_enum);

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

static FaceEnum getColor(Cube c, FaceEnum fe, CubieEnum ce, int isEdge) {
    Int8 cubie = getCubie(isEdge ? c.edges : c.corners, ce);

    Int8 permutation = cubie & ((1 << (CUBIE_BITS - (isEdge ? 1 : 2))) - 1);
    Int8 orientation = cubie >> CUBIE_BITS - (isEdge ? 1 : 2);

    FaceEnum colors[3] = {};

    if (isEdge) {
        if (permutation == UF) {
            colors[0] = U_FACE; colors[1] = F_FACE;
        }
        else if (permutation == UB) {
            colors[0] = U_FACE; colors[1] = B_FACE;
        }
        else if (permutation == UR) {
            colors[0] = U_FACE; colors[1] = R_FACE;
        }
        else if (permutation == UL) {
            colors[0] = U_FACE; colors[1] = L_FACE;
        }
        else if (permutation == DF) {
            colors[0] = D_FACE; colors[1] = F_FACE;
        }
        else if (permutation == DB) {
            colors[0] = D_FACE; colors[1] = B_FACE;
        }
        else if (permutation == DR) {
            colors[0] = D_FACE; colors[1] = R_FACE;
        }
        else if (permutation == DL) {
            colors[0] = D_FACE; colors[1] = L_FACE;
        }
        else if (permutation == FR) {
            colors[0] = F_FACE; colors[1] = R_FACE;
        }
        else if (permutation == FL) {
            colors[0] = F_FACE; colors[1] = L_FACE;
        }
        else if (permutation == BR) {
            colors[0] = B_FACE; colors[1] = R_FACE;
        }
        else if (permutation == BL) {
            colors[0] = B_FACE; colors[1] = L_FACE;
        }
    }
    else {
        if (permutation == UFR) {
            colors[0] = U_FACE; colors[1] = F_FACE; colors[2] = R_FACE;
        }
        else if (permutation == UFL) {
            colors[0] = U_FACE; colors[1] = F_FACE; colors[2] = L_FACE;
        }
        else if (permutation == UBR) {
            colors[0] = U_FACE; colors[1] = B_FACE; colors[2] = R_FACE;
        }
        else if (permutation == UBL) {
            colors[0] = U_FACE; colors[1] = B_FACE; colors[2] = L_FACE;
        }
        else if (permutation == DFR) {
            colors[0] = D_FACE; colors[1] = F_FACE; colors[2] = R_FACE;
        }
        else if (permutation == DFL) {
            colors[0] = D_FACE; colors[1] = F_FACE; colors[2] = L_FACE;
        }
        else if (permutation == DBR) {
            colors[0] = D_FACE; colors[1] = B_FACE; colors[2] = R_FACE;
        }
        else if (permutation == DBL) {
            colors[0] = D_FACE; colors[1] = B_FACE; colors[2] = L_FACE;
        }
    }

    return colors[orientation];
}

static void printColor(FaceEnum color) {
    if (color == U_FACE) {
        printf("\033[0;37m"); // white
    }
    else if (color == D_FACE) {
        printf("\033[0;33m"); // yellow
    }
    else if (color == F_FACE) {
        printf("\033[0;32m"); // green
    }
    else if (color == B_FACE) {
        printf("\033[0;34m"); // blue
    }
    else if (color == R_FACE) {
        printf("\033[0;31m"); // red
    }
    else if (color == L_FACE) {
        printf("\033[0;35m"); // purple
    }
    printf("▣\033[0m");
}

void printCube(Cube c) {
    FaceEnum faces[6] = { U_FACE, L_FACE, F_FACE, R_FACE, B_FACE, D_FACE };
    CubieEnum cubies[3][6][3] = {
        { { UBL, UB, UBR }, { UBL, UL, UFL }, { UFL, UF, UFR }, { UFR, UR, UBR }, { UBR, UB, UBL }, { DFL, DF, DFR } },
        { { UL, -1, UR }, { BL, -1, FL }, { FL, -1, FR }, { FR, -1, BR }, { BR, -1, BL }, { DL, -1, DR } },
        { { UFL, UF, UFR }, { DBL, DL, DFL }, { DFL, DF, DFR }, { DFR, DR, DBR }, { DBR, DB, DBL }, { DBL, DB, DBR } } 
    };

    printf("┌──┤U├──┬──┤L├──┬──┤F├──┬──┤R├──┬──┤B├──┬──┤D├──┐\n");

    int i, j, k;
    for (i=0 ; i<3 ; i++) {
        for (j=0 ; j<6 ; j++) {
            printf("│ ");
            for (k=0 ; k<3 ; k++) {
                int isEdge = ((i+k) % 2 == 1);
                FaceEnum color = (i == 1 && k == 1) ? faces[j] : getColor(c, faces[j], cubies[i][j][k], isEdge);
                printColor(color);
                printf(" ", color);
            }
        }
        printf("│\n");
    }

    printf("└───────┴───────┴───────┴───────┴───────┴───────┘\n");

    // FOR DEBUGGING PURPOSES ONLY:
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