#include <stdio.h>
#include "cube.h"

/*
TODO:
1. cubieEncoding() corners
2. convertCube()
*/

// represent colors and faces by FaceMask using corresponding MoveMask
typedef MoveMask FaceMask;

typedef struct {
    FaceMask C, U, D, R, L, UR, UL, DR, DL;
} Face;

typedef struct {
    Face U, D, F, B, R, L;
} ColorCube;

static Int8 cubieEncoding(ColorCube cc, CubieEnum ce, int isEdge) {
    FaceMask colors[3] = {};
    Int8 permutation, orientation;

    // read colors
    if (isEdge) {
        if (ce == UF) {
            colors[0] = cc.U.D; colors[1] = cc.F.U;
        }
        else if (ce == UB) {
            colors[0] = cc.U.U; colors[1] = cc.B.U;
        }
        else if (ce == UR) {
            colors[0] = cc.U.R; colors[1] = cc.R.U;
        }
        else if (ce == UL) {
            colors[0] = cc.U.L; colors[1] = cc.L.U;
        }
        else if (ce == DF) {
            colors[0] = cc.D.U; colors[1] = cc.F.D;
        }
        else if (ce == DB) {
            colors[0] = cc.D.D; colors[1] = cc.B.D;
        }
        else if (ce == DR) {
            colors[0] = cc.D.R; colors[1] = cc.R.D;
        }
        else if (ce == DL) {
            colors[0] = cc.D.L; colors[1] = cc.L.D;
        }
        else if (ce == FR) {
            colors[0] = cc.F.R; colors[1] = cc.R.L;
        }
        else if (ce == FL) {
            colors[0] = cc.F.L; colors[1] = cc.L.R;
        }
        else if (ce == BR) {
            colors[0] = cc.B.L; colors[1] = cc.R.R;
        }
        else if (ce == BL) {
            colors[0] = cc.B.R; colors[1] = cc.L.L;
        }
    }
    else {
        // TODO
    }

    // set permutation
    if (isEdge) {
        if ((colors[0]|colors[1]) == (U|F)) {
            permutation = UF;
        }
        else if ((colors[0]|colors[1]) == (U|B)) {
            permutation = UB;
        }
        else if ((colors[0]|colors[1]) == (U|R)) {
            permutation = UR;
        }
        else if ((colors[0]|colors[1]) == (U|L)) {
            permutation = UL;
        }
        else if ((colors[0]|colors[1]) == (D|F)) {
            permutation = DF;
        }
        else if ((colors[0]|colors[1]) == (D|B)) {
            permutation = DB;
        }
        else if ((colors[0]|colors[1]) == (D|R)) {
            permutation = DR;
        }
        else if ((colors[0]|colors[1]) == (D|L)) {
            permutation = DL;
        }
        else if ((colors[0]|colors[1]) == (F|R)) {
            permutation = FR;
        }
        else if ((colors[0]|colors[1]) == (F|L)) {
            permutation = FL;
        }
        else if ((colors[0]|colors[1]) == (B|R)) {
            permutation = BR;
        }
        else if ((colors[0]|colors[1]) == (B|L)) {
            permutation = BL;
        }
    }
    else {
        // TODO
    }

    // set orientation
    if (isEdge) {
        orientation = (colors[0] & (L|R) || ((colors[0] & (F|B)) && (colors[1] & (U|D))));
    }
    else {
        // TODO
    }

    // return cubie encoding
    return permutation | (orientation << (CUBIE_BITS - (isEdge ? 1 : 2)));
}

Cube convertColorCube(ColorCube cc) {
    Cube c = { 0, 0 };

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        c.edges |= ((Int64) cubieEncoding(cc, i, 1)) << (CUBIE_BITS * i);
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        c.corners |= ((Int64) cubieEncoding(cc, i, 0)) << (CUBIE_BITS * i);
    }

    return c;
}

ColorCube convertCube(Cube c) {
    ColorCube cc;
    cc.U.C = U; cc.D.C = D; cc.F.C = F; cc.B.C = B; cc.R.C = R; cc.L.C = L;

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {

    }
    for(i=0 ; i<NUM_CORNERS ; i++) {

    }

    return cc;

}

static void printColor(FaceMask color) {
    if (color & U) {
        printf("\033[0;37m"); // white
    }
    else if (color & D) {
        printf("\033[0;33m"); // yellow
    }
    else if (color & F) {
        printf("\033[0;32m"); // green
    }
    else if (color & B) {
        printf("\033[0;34m"); // blue
    }
    else if (color & R) {
        printf("\033[0;31m"); // red
    }
    else if (color & L) {
        printf("\033[0;35m"); // purple
    }
    printf("▣\033[0m");
}

void printColorCube(ColorCube cc) {
    Face faces[6] = { cc.U, cc.L, cc.F, cc.R, cc.B, cc.D };

    printf("┌──┤U├──┬──┤L├──┬──┤F├──┬──┤R├──┬──┤B├──┬──┤D├──┐\n");

    int i, j, k; // i = row, j = face, k = column
    for (i=0 ; i<3 ; i++) {
        for (j=0 ; j<6 ; j++) {
            printf("│ ");
            for (k=0 ; k<3 ; k++) {
                if (i==0 && k==0) {
                    printColor(faces[j].UL);
                }
                else if (i==0 && k==1) {
                    printColor(faces[j].U);
                }
                else if (i==0 && k==2) {
                    printColor(faces[j].UR);
                }
                else if (i==1 && k==0) {
                    printColor(faces[j].L);
                }
                else if (i==1 && k==1) {
                    printColor(faces[j].C);
                }
                else if (i==1 && k==2) {
                    printColor(faces[j].R);
                }
                else if (i==2 && k==0) {
                    printColor(faces[j].DL);
                }
                else if (i==2 && k==1) {
                    printColor(faces[j].D);
                }
                else if (i==2 && k==2) {
                    printColor(faces[j].DR);
                }
                printf(" ");
            }
        }
        printf("│\n");
    }

    printf("└───────┴───────┴───────┴───────┴───────┴───────┘\n");
}

ColorCube colorCubeFactory() {
    ColorCube cc;
    Face *fs[6] = { &cc.U, &cc.D, &cc.F, &cc.B, &cc.L, &cc.R };
    FaceMask cs[6] = { U, D, F, B, L, R };
    
    int i;
    for (i=0 ; i<6 ; i++) {
        Face *f = fs[i];
        FaceMask color = cs[i];
        f->C = f->U = f->D = f->R = f->L = f->UL = f->UR = f->DL = f->DR = color;
    }

    return cc;
}

void printCube(Cube c) {
    // ColorCube cc = convertCube(c);
    // printColorCube(cc);

    int i;
    printf("  EDGES: ");
    for(i=0 ; i<NUM_EDGES ; i++) {
        Int8 cubie = getCubie(c.edges, i);
        printf("%2u:%u ", getPermutation(cubie, 1), getOrientation(cubie, 1));
    }
    printf("\nCORNERS: ");
    for(i=0 ; i<NUM_CORNERS ; i++) {
        Int8 cubie = getCubie(c.corners, i);
        printf("%2u:%u ", getPermutation(cubie, 0), getOrientation(cubie, 0));
    }
    printf("\n");
}

// int main() {
//     ColorCube cc0 = colorCubeFactory();
//     printColorCube(cc0);

//     Cube c0 = convertColorCube(cc0);
//     printCube(c0);
    
//     return 0;
// }


// static FaceMask getColor(Cube c, FaceMask fm, CubieEnum ce, int isEdge) {
//     Int8 cubie = ((isEdge ? c.edges : c.corners) >> (CUBIE_BITS * ce)) & ((1 << CUBIE_BITS) - 1);

//     Int8 permutation = cubie & ((1 << (CUBIE_BITS - (isEdge ? 1 : 2))) - 1);
//     Int8 orientation = cubie >> CUBIE_BITS - (isEdge ? 1 : 2);

//     FaceMask colors[3] = {};

//     if (isEdge) {
//         if (permutation == UF) {
//             colors[0] = U_FACE; colors[1] = F_FACE;
//         }
//         else if (permutation == UB) {
//             colors[0] = U_FACE; colors[1] = B_FACE;
//         }
//         else if (permutation == UR) {
//             colors[0] = U_FACE; colors[1] = R_FACE;
//         }
//         else if (permutation == UL) {
//             colors[0] = U_FACE; colors[1] = L_FACE;
//         }
//         else if (permutation == DF) {
//             colors[0] = D_FACE; colors[1] = F_FACE;
//         }
//         else if (permutation == DB) {
//             colors[0] = D_FACE; colors[1] = B_FACE;
//         }
//         else if (permutation == DR) {
//             colors[0] = D_FACE; colors[1] = R_FACE;
//         }
//         else if (permutation == DL) {
//             colors[0] = D_FACE; colors[1] = L_FACE;
//         }
//         else if (permutation == FR) {
//             colors[0] = F_FACE; colors[1] = R_FACE;
//         }
//         else if (permutation == FL) {
//             colors[0] = F_FACE; colors[1] = L_FACE;
//         }
//         else if (permutation == BR) {
//             colors[0] = B_FACE; colors[1] = R_FACE;
//         }
//         else if (permutation == BL) {
//             colors[0] = B_FACE; colors[1] = L_FACE;
//         }
//     }
//     else {
//         if (permutation == UFR) {
//             colors[0] = U_FACE; colors[1] = F_FACE; colors[2] = R_FACE;
//         }
//         else if (permutation == UFL) {
//             colors[0] = U_FACE; colors[1] = F_FACE; colors[2] = L_FACE;
//         }
//         else if (permutation == UBR) {
//             colors[0] = U_FACE; colors[1] = B_FACE; colors[2] = R_FACE;
//         }
//         else if (permutation == UBL) {
//             colors[0] = U_FACE; colors[1] = B_FACE; colors[2] = L_FACE;
//         }
//         else if (permutation == DFR) {
//             colors[0] = D_FACE; colors[1] = F_FACE; colors[2] = R_FACE;
//         }
//         else if (permutation == DFL) {
//             colors[0] = D_FACE; colors[1] = F_FACE; colors[2] = L_FACE;
//         }
//         else if (permutation == DBR) {
//             colors[0] = D_FACE; colors[1] = B_FACE; colors[2] = R_FACE;
//         }
//         else if (permutation == DBL) {
//             colors[0] = D_FACE; colors[1] = B_FACE; colors[2] = L_FACE;
//         }
//         // TODO: fix
//         colors[0] = colors[1] = colors[2] = 0;
//     }

//     return colors[orientation];
// }

// static void printColor(FaceEnum color) {
//     if (color == U_FACE) {
//         printf("\033[0;37m"); // white
//     }
//     else if (color == D_FACE) {
//         printf("\033[0;33m"); // yellow
//     }
//     else if (color == F_FACE) {
//         printf("\033[0;32m"); // green
//     }
//     else if (color == B_FACE) {
//         printf("\033[0;34m"); // blue
//     }
//     else if (color == R_FACE) {
//         printf("\033[0;31m"); // red
//     }
//     else if (color == L_FACE) {
//         printf("\033[0;35m"); // purple
//     }
//     printf("▣\033[0m");
// }

// void printCube(Cube c) {
//     FaceEnum faces[6] = { U_FACE, L_FACE, F_FACE, R_FACE, B_FACE, D_FACE };
//     CubieEnum cubies[3][6][3] = {
//         { { UBL, UB, UBR }, { UBL, UL, UFL }, { UFL, UF, UFR }, { UFR, UR, UBR }, { UBR, UB, UBL }, { DFL, DF, DFR } },
//         { { UL, -1, UR }, { BL, -1, FL }, { FL, -1, FR }, { FR, -1, BR }, { BR, -1, BL }, { DL, -1, DR } },
//         { { UFL, UF, UFR }, { DBL, DL, DFL }, { DFL, DF, DFR }, { DFR, DR, DBR }, { DBR, DB, DBL }, { DBL, DB, DBR } } 
//     };

//     printf("┌──┤U├──┬──┤L├──┬──┤F├──┬──┤R├──┬──┤B├──┬──┤D├──┐\n");

//     int i, j, k;
//     for (i=0 ; i<3 ; i++) {
//         for (j=0 ; j<6 ; j++) {
//             printf("│ ");
//             for (k=0 ; k<3 ; k++) {
//                 int isEdge = ((i+k) % 2 == 1);
//                 FaceEnum color = (i == 1 && k == 1) ? faces[j] : getColor(c, faces[j], cubies[i][j][k], isEdge);
//                 printColor(color);
//                 printf(" ", color);
//             }
//         }
//         printf("│\n");
//     }

//     printf("└───────┴───────┴───────┴───────┴───────┴───────┘\n");

//     // FOR DEBUGGING PURPOSES ONLY:
//     printf("%llu %llu\n", c.edges, c.corners);
// }

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