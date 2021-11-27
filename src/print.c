#include <stdio.h>
#include "cube.h"

/*
TODO:
1. getColor()
*/

static FaceEnum getColor(Cube c, FaceEnum fe, CubieEnum ce, int isEdge) {
    Int8 cubie = ((isEdge ? c.edges : c.corners) >> (CUBIE_BITS * ce)) & ((1 << CUBIE_BITS) - 1);

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