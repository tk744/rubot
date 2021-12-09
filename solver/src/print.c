#include <stdio.h>
#include "cube.h"

/* Returns a new solved ColorCube. */
ColorCube colorCubeFactory() {
    ColorCube cc;
    Face *faces[6] = { &cc.U, &cc.D, &cc.F, &cc.B, &cc.L, &cc.R };
    FaceMask colors[6] = { MV_MASK_U, MV_MASK_D, MV_MASK_F, MV_MASK_B, MV_MASK_L, MV_MASK_R };
    
    int i;
    for (i=0 ; i<6 ; i++) {
        Face *f = faces[i];
        FaceMask color = colors[i];
        f->C = f->U = f->D = f->R = f->L = f->UL = f->UR = f->DL = f->DR = color;
    }

    return cc;
}

/* Returns representation of a cubie from ColorCube `cc` specified by cubie enum. */
static Int8 getColorCubie(ColorCube cc, CubieEnum ce, int isEdge) {
    Int8 permutation, orientation;
    
    FaceMask colors[3] = {};

    // read colors
    if (isEdge) {
        if (ce == ECUBIE_UF) {
            colors[0] = cc.U.D; colors[1] = cc.F.U;
        }
        else if (ce == ECUBIE_UB) {
            colors[0] = cc.U.U; colors[1] = cc.B.U;
        }
        else if (ce == ECUBIE_UR) {
            colors[0] = cc.U.R; colors[1] = cc.R.U;
        }
        else if (ce == ECUBIE_UL) {
            colors[0] = cc.U.L; colors[1] = cc.L.U;
        }
        else if (ce == ECUBIE_DF) {
            colors[0] = cc.D.U; colors[1] = cc.F.D;
        }
        else if (ce == ECUBIE_DB) {
            colors[0] = cc.D.D; colors[1] = cc.B.D;
        }
        else if (ce == ECUBIE_DR) {
            colors[0] = cc.D.R; colors[1] = cc.R.D;
        }
        else if (ce == ECUBIE_DL) {
            colors[0] = cc.D.L; colors[1] = cc.L.D;
        }
        else if (ce == ECUBIE_FR) {
            colors[0] = cc.F.R; colors[1] = cc.R.L;
        }
        else if (ce == ECUBIE_FL) {
            colors[0] = cc.F.L; colors[1] = cc.L.R;
        }
        else if (ce == ECUBIE_BR) {
            colors[0] = cc.B.L; colors[1] = cc.R.R;
        }
        else if (ce == ECUBIE_BL) {
            colors[0] = cc.B.R; colors[1] = cc.L.L;
        }
    }
    else {
        // TODO
    }

    // set permutation
    if (isEdge) {
        if ((colors[0]|colors[1]) == (MV_MASK_U|MV_MASK_F)) {
            permutation = ECUBIE_UF;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_U|MV_MASK_B)) {
            permutation = ECUBIE_UB;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_U|MV_MASK_R)) {
            permutation = ECUBIE_UR;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_U|MV_MASK_L)) {
            permutation = ECUBIE_UL;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_D|MV_MASK_F)) {
            permutation = ECUBIE_DF;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_D|MV_MASK_B)) {
            permutation = ECUBIE_DB;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_D|MV_MASK_R)) {
            permutation = ECUBIE_DR;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_D|MV_MASK_L)) {
            permutation = ECUBIE_DL;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_F|MV_MASK_R)) {
            permutation = ECUBIE_FR;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_F|MV_MASK_L)) {
            permutation = ECUBIE_FL;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_B|MV_MASK_R)) {
            permutation = ECUBIE_BR;
        }
        else if ((colors[0]|colors[1]) == (MV_MASK_B|MV_MASK_L)) {
            permutation = ECUBIE_BL;
        }
    }
    else {
        // TODO
    }

    // set orientation
    if (isEdge) {
        orientation = (colors[0] & (MV_MASK_L|MV_MASK_R) || ((colors[0] & (MV_MASK_F|MV_MASK_B)) && (colors[1] & (MV_MASK_U|MV_MASK_D))));
    }
    else {
        // TODO
    }

    // return cubie encoding
    Int8 cubie;
    setPermutation(&cubie, isEdge, permutation);
    setOrientation(&cubie, isEdge, orientation);
    return cubie;
}

/* Returns color of a square from Cube `c` specified by cubie enum and face enum. */
static FaceMask getColor(Cube c, CubieEnum ce, int isEdge, FaceMask fm) {
    Int8 cubie = getCubie(isEdge ? c.edges : c.corners, ce);
    Int8 permutation = getPermutation(cubie, isEdge);
    Int8 orientation = getOrientation(cubie, isEdge);

    FaceMask colors[3] = {};
    int idx = 0;

    if (isEdge) {
        // set color array
        if (permutation == ECUBIE_UF) {
            colors[0] = MV_MASK_U; colors[1] = MV_MASK_F;
        }
        else if (permutation == ECUBIE_UB) {
            colors[0] = MV_MASK_U; colors[1] = MV_MASK_B;
        }
        else if (permutation == ECUBIE_UR) {
            colors[0] = MV_MASK_U; colors[1] = MV_MASK_R;
        }
        else if (permutation == ECUBIE_UL) {
            colors[0] = MV_MASK_U; colors[1] = MV_MASK_L;
        }
        else if (permutation == ECUBIE_DF) {
            colors[0] = MV_MASK_D; colors[1] = MV_MASK_F;
        }
        else if (permutation == ECUBIE_DB) {
            colors[0] = MV_MASK_D; colors[1] = MV_MASK_B;
        }
        else if (permutation == ECUBIE_DR) {
            colors[0] = MV_MASK_D; colors[1] = MV_MASK_R;
        }
        else if (permutation == ECUBIE_DL) {
            colors[0] = MV_MASK_D; colors[1] = MV_MASK_L;
        }
        else if (permutation == ECUBIE_FR) {
            colors[0] = MV_MASK_F; colors[1] = MV_MASK_R;
        }
        else if (permutation == ECUBIE_FL) {
            colors[0] = MV_MASK_F; colors[1] = MV_MASK_L;
        }
        else if (permutation == ECUBIE_BR) {
            colors[0] = MV_MASK_B; colors[1] = MV_MASK_R;
        }
        else if (permutation == ECUBIE_BL) {
            colors[0] = MV_MASK_B; colors[1] = MV_MASK_L;
        }

        // set color index
        idx = (colors[1] & (MV_MASK_L|MV_MASK_R)) ? !orientation : orientation;
        if ((fm & (MV_MASK_F|MV_MASK_B)) || ((fm & (MV_MASK_U|MV_MASK_D)) && (ce == ECUBIE_UR || ce == ECUBIE_UL || ce == ECUBIE_DR || ce == ECUBIE_DL))) {
            idx = !idx;
        }
    }
    else {
        // set color array
        if (permutation == CCUBIE_UFR) {
            colors[0] = MV_MASK_F; colors[1] = MV_MASK_R; colors[2] = MV_MASK_U;
        }
        else if (permutation == CCUBIE_UFL) {
            colors[0] = MV_MASK_F; colors[1] = MV_MASK_L; colors[2] = MV_MASK_U;
        }
        else if (permutation == CCUBIE_UBR) {
            colors[0] = MV_MASK_B; colors[1] = MV_MASK_R; colors[2] = MV_MASK_U;
        }
        else if (permutation == CCUBIE_UBL) {
            colors[0] = MV_MASK_B; colors[1] = MV_MASK_L; colors[2] = MV_MASK_U;
        }
        else if (permutation == CCUBIE_DFR) {
            colors[0] = MV_MASK_F; colors[1] = MV_MASK_R; colors[2] = MV_MASK_D;
        }
        else if (permutation == CCUBIE_DFL) {
            colors[0] = MV_MASK_F; colors[1] = MV_MASK_L; colors[2] = MV_MASK_D;
        }
        else if (permutation == CCUBIE_DBR) {
            colors[0] = MV_MASK_B; colors[1] = MV_MASK_R; colors[2] = MV_MASK_D;
        }
        else if (permutation == CCUBIE_DBL) {
            colors[0] = MV_MASK_B; colors[1] = MV_MASK_L; colors[2] = MV_MASK_D;
        }

        // set color index
        int permutation_tetrad, ce_tetrad;

        if (permutation == CCUBIE_UFR || permutation == CCUBIE_DFL || permutation == CCUBIE_UBL || permutation == CCUBIE_DBR) {
            permutation_tetrad = 1;
        }
        else {
            permutation_tetrad = 0;
        }

        if (ce == CCUBIE_UFR || ce == CCUBIE_DFL || ce == CCUBIE_UBL || ce == CCUBIE_DBR) {
            ce_tetrad = 1;
        }
        else {
            ce_tetrad = 0;
        }

        if (fm & (MV_MASK_F|MV_MASK_B)) {
            if (ce_tetrad == 1) {
                if (orientation == 0) {
                    idx = 0;
                }
                else if (orientation == 1) {
                    idx = (permutation_tetrad == ce_tetrad) ? 2 : 1;
                }
                else if (orientation == 2) {
                    idx = (permutation_tetrad == ce_tetrad) ? 1 : 2;
                }
            }
            else {
                if (orientation == 0) {
                    idx = 0;
                }
                else if (orientation == 1) {
                    idx = (permutation_tetrad == ce_tetrad) ? 1 : 2;
                }
                else if (orientation == 2) {
                    idx = (permutation_tetrad == ce_tetrad) ? 2 : 1;
                }
            }
        }
        else if (fm & (MV_MASK_R|MV_MASK_L)) {
            if (orientation == 0) {
                if (permutation_tetrad == ce_tetrad) {
                    idx = 1;
                }
                else {
                    idx = 2;
                }
            }
            else if (orientation == 1) {
                if (ce_tetrad == 0) {
                    idx = (permutation_tetrad == ce_tetrad) ? 2 : 1;
                }
                else {
                    idx = 0;
                }
            }
            else if (orientation == 2) {
                if (ce_tetrad == 1) {
                    idx = (permutation_tetrad == ce_tetrad) ? 2 : 1;
                }
                else {
                    idx = 0;
                }
            }
        }
        else if (fm & (MV_MASK_U|MV_MASK_D)) {
            if (orientation == 0) {
                if (permutation_tetrad == ce_tetrad) {
                    idx = 2;
                }
                else {
                    idx = 1;
                }
            }
            else if (orientation == 1) {
                if (ce_tetrad == 0) {
                    idx = 0;
                }
                else {
                    idx = (permutation_tetrad == ce_tetrad) ? 1 : 2;
                }
            }
            else if (orientation == 2) {
                if (ce_tetrad == 1) {
                    idx = 0;
                }
                else {
                    idx = (permutation_tetrad == ce_tetrad) ? 1 : 2;
                }
            }
        }
    }

    return colors[idx];
}

/* Returns ColorCube representation of Cube `c`. */
ColorCube convertCube(Cube c) {
    ColorCube cc;
    
    // set center cubies
    cc.U.C = MV_MASK_U; 
    cc.D.C = MV_MASK_D; 
    cc.F.C = MV_MASK_F; 
    cc.B.C = MV_MASK_B; 
    cc.R.C = MV_MASK_R; 
    cc.L.C = MV_MASK_L;
    
    // set edge cubies
    cc.U.D = getColor(c, ECUBIE_UF, 1, MV_MASK_U);
    cc.F.U = getColor(c, ECUBIE_UF, 1, MV_MASK_F);
    cc.U.U = getColor(c, ECUBIE_UB, 1, MV_MASK_U);
    cc.B.U = getColor(c, ECUBIE_UB, 1, MV_MASK_B);
    cc.U.R = getColor(c, ECUBIE_UR, 1, MV_MASK_U);
    cc.R.U = getColor(c, ECUBIE_UR, 1, MV_MASK_R);
    cc.U.L = getColor(c, ECUBIE_UL, 1, MV_MASK_U);
    cc.L.U = getColor(c, ECUBIE_UL, 1, MV_MASK_L);
    cc.D.U = getColor(c, ECUBIE_DF, 1, MV_MASK_D);
    cc.F.D = getColor(c, ECUBIE_DF, 1, MV_MASK_F);
    cc.D.D = getColor(c, ECUBIE_DB, 1, MV_MASK_D);
    cc.B.D = getColor(c, ECUBIE_DB, 1, MV_MASK_B);
    cc.D.R = getColor(c, ECUBIE_DR, 1, MV_MASK_D);
    cc.R.D = getColor(c, ECUBIE_DR, 1, MV_MASK_R);
    cc.D.L = getColor(c, ECUBIE_DL, 1, MV_MASK_D);
    cc.L.D = getColor(c, ECUBIE_DL, 1, MV_MASK_L);
    cc.F.R = getColor(c, ECUBIE_FR, 1, MV_MASK_F);
    cc.R.L = getColor(c, ECUBIE_FR, 1, MV_MASK_R);
    cc.F.L = getColor(c, ECUBIE_FL, 1, MV_MASK_F);
    cc.L.R = getColor(c, ECUBIE_FL, 1, MV_MASK_L);
    cc.B.L = getColor(c, ECUBIE_BR, 1, MV_MASK_B);
    cc.R.R = getColor(c, ECUBIE_BR, 1, MV_MASK_R);
    cc.B.R = getColor(c, ECUBIE_BL, 1, MV_MASK_B);
    cc.L.L = getColor(c, ECUBIE_BL, 1, MV_MASK_L);
            
    // set corner cubies
    cc.U.DR = getColor(c, CCUBIE_UFR, 0, MV_MASK_U);
    cc.F.UR = getColor(c, CCUBIE_UFR, 0, MV_MASK_F);
    cc.R.UL = getColor(c, CCUBIE_UFR, 0, MV_MASK_R);
    cc.U.DL = getColor(c, CCUBIE_UFL, 0, MV_MASK_U);
    cc.F.UL = getColor(c, CCUBIE_UFL, 0, MV_MASK_F);
    cc.L.UR = getColor(c, CCUBIE_UFL, 0, MV_MASK_L);
    cc.U.UR = getColor(c, CCUBIE_UBR, 0, MV_MASK_U);
    cc.B.UL = getColor(c, CCUBIE_UBR, 0, MV_MASK_B);
    cc.R.UR = getColor(c, CCUBIE_UBR, 0, MV_MASK_R);
    cc.U.UL = getColor(c, CCUBIE_UBL, 0, MV_MASK_U);
    cc.B.UR = getColor(c, CCUBIE_UBL, 0, MV_MASK_B);
    cc.L.UL = getColor(c, CCUBIE_UBL, 0, MV_MASK_L);
    cc.D.UR = getColor(c, CCUBIE_DFR, 0, MV_MASK_D);
    cc.F.DR = getColor(c, CCUBIE_DFR, 0, MV_MASK_F);
    cc.R.DL = getColor(c, CCUBIE_DFR, 0, MV_MASK_R);
    cc.D.UL = getColor(c, CCUBIE_DFL, 0, MV_MASK_D);
    cc.F.DL = getColor(c, CCUBIE_DFL, 0, MV_MASK_F);
    cc.L.DR = getColor(c, CCUBIE_DFL, 0, MV_MASK_L);
    cc.D.DR = getColor(c, CCUBIE_DBR, 0, MV_MASK_D);
    cc.B.DL = getColor(c, CCUBIE_DBR, 0, MV_MASK_B);
    cc.R.DR = getColor(c, CCUBIE_DBR, 0, MV_MASK_R);
    cc.D.DL = getColor(c, CCUBIE_DBL, 0, MV_MASK_D);
    cc.B.DR = getColor(c, CCUBIE_DBL, 0, MV_MASK_B);
    cc.L.DL = getColor(c, CCUBIE_DBL, 0, MV_MASK_L);
    
    return cc;
}

/* Returns Cube representation of ColorCube `cc`. */
Cube convertColorCube(ColorCube cc) {
    Cube c = { 0, 0 };

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        setCubie(&c.edges, i, getColorCubie(cc, i, 1));
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        setCubie(&c.corners, i, getColorCubie(cc, i, 0));
    }

    return c;
}

/* Print colored square to console. */
static void printColor(FaceMask color) {
    if (color & MV_MASK_U) {
        printf("\033[0;37m"); // white
    }
    else if (color & MV_MASK_D) {
        printf("\033[0;33m"); // yellow
    }
    else if (color & MV_MASK_F) {
        printf("\033[0;32m"); // green
    }
    else if (color & MV_MASK_B) {
        printf("\033[0;34m"); // blue
    }
    else if (color & MV_MASK_R) {
        printf("\033[0;31m"); // red
    }
    else if (color & MV_MASK_L) {
        printf("\033[0;35m"); // purple
    }
    printf("▣\033[0m");
}

/* Print ColorCube graphic to console. */
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

/* Print Cube representation to console. */
static void printCubeEncoding(Cube c) {
    int i;
    printf("EDGES:   ");
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

/* Print Cube graphic to console. */
void printCube(Cube c) {
    ColorCube cc = convertCube(c);
    printColorCube(cc);
    //printCubeEncoding(c);
}

/* Print single Move to console without terminating characters. */ 
void printMove(Move m) {
    if (m & MV_MASK_U) {
        printf("U");
    }
    else if (m & MV_MASK_D) {
        printf("D");
    }
    else if (m & MV_MASK_F) {
        printf("F");
    }
    else if (m & MV_MASK_B) {
        printf("B");
    }
    else if (m & MV_MASK_R) {
        printf("R");
    }
    else if (m & MV_MASK_L) {
        printf("L");
    }
    else {
        printf("NOP");
        return;
    }

    if (m & MV_MASK_H) {
        printf("2");
    }
    else if (m & MV_MASK_I) {
        printf("'");
    }
}

/* Print list of Moves to console. */
void printMoves(Move *ms, int n) {
    while(n-- > 0) {
        printMove(*ms++);
        printf(n == 0 ? "\n" : ", ");
    }
}