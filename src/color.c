#include "cube.h"
#include <stdio.h>

typedef MoveMask Color; // to reuse cubie enums

typedef struct {
    Color C, U, D, R, L, UR, UL, DR, DL;
} ColorFace;

typedef struct {
    ColorFace U, D, F, B, R, L;
} ColorCube;

static Int8 cornerTetrad(CubieEnum ce) {
    if (ce == UFR || ce == DFL || ce == UBL || ce == DBR) {
        return 1;
    }
    else {
        return 0;
    }
}

static Color encodeCubie(Cube128 c, CubieEnum ce, int isEdge, Color fm) {
    Int8 cubie = getCubie(isEdge ? c.edges : c.corners, ce);
    Int8 permutation = getPermutation(cubie, isEdge);
    Int8 orientation = getOrientation(cubie, isEdge);

    Color colors[3] = {};
    int idx = 0;

    if (isEdge) {
        // set color array
        if (permutation == UF) {
            colors[0] = U; colors[1] = F;
        }
        else if (permutation == UB) {
            colors[0] = U; colors[1] = B;
        }
        else if (permutation == UR) {
            colors[0] = U; colors[1] = R;
        }
        else if (permutation == UL) {
            colors[0] = U; colors[1] = L;
        }
        else if (permutation == DF) {
            colors[0] = D; colors[1] = F;
        }
        else if (permutation == DB) {
            colors[0] = D; colors[1] = B;
        }
        else if (permutation == DR) {
            colors[0] = D; colors[1] = R;
        }
        else if (permutation == DL) {
            colors[0] = D; colors[1] = L;
        }
        else if (permutation == FR) {
            colors[0] = F; colors[1] = R;
        }
        else if (permutation == FL) {
            colors[0] = F; colors[1] = L;
        }
        else if (permutation == BR) {
            colors[0] = B; colors[1] = R;
        }
        else if (permutation == BL) {
            colors[0] = B; colors[1] = L;
        }

        // set color index
        idx = (colors[1] & (L|R)) ? !orientation : orientation;
        if ((fm & (F|B)) || ((fm & (U|D)) && (ce == UR || ce == UL || ce == DR || ce == DL))) {
            idx = !idx;
        }
    }
    else {
        // set color array
        if (permutation == UFR) {
            colors[0] = F; colors[1] = R; colors[2] = U;
        }
        else if (permutation == UFL) {
            colors[0] = F; colors[1] = L; colors[2] = U;
        }
        else if (permutation == UBR) {
            colors[0] = B; colors[1] = R; colors[2] = U;
        }
        else if (permutation == UBL) {
            colors[0] = B; colors[1] = L; colors[2] = U;
        }
        else if (permutation == DFR) {
            colors[0] = F; colors[1] = R; colors[2] = D;
        }
        else if (permutation == DFL) {
            colors[0] = F; colors[1] = L; colors[2] = D;
        }
        else if (permutation == DBR) {
            colors[0] = B; colors[1] = R; colors[2] = D;
        }
        else if (permutation == DBL) {
            colors[0] = B; colors[1] = L; colors[2] = D;
        }

        // set color index
        Int8 true_tetrad = cornerTetrad(permutation);
        Int8 curr_tetrad = cornerTetrad(ce);

        if (fm & (F|B)) {
            if (orientation == 0) {
                idx = 0;
            }
            else if (orientation == 1) {
                if (curr_tetrad == 0) {
                    idx = (true_tetrad == curr_tetrad) ? 1 : 2;
                }
                else {
                    idx = (true_tetrad == curr_tetrad) ? 2 : 1;
                }
            }
            else if (orientation == 2) {
                if (curr_tetrad == 1) {
                    idx = (true_tetrad == curr_tetrad) ? 1 : 2;
                }
                else {
                    idx = (true_tetrad == curr_tetrad) ? 2 : 1;
                }
            }
        }
        else if (fm & (R|L)) {
            if (orientation == 0) {
                idx = (true_tetrad == curr_tetrad) ? 1 : 2;
            }
            else if (orientation == 1) {
                if (curr_tetrad == 0) {
                    idx = (true_tetrad == curr_tetrad) ? 2 : 1;
                }
                else {
                    idx = 0;
                }
            }
            else if (orientation == 2) {
                if (curr_tetrad == 1) {
                    idx = (true_tetrad == curr_tetrad) ? 2 : 1;
                }
                else {
                    idx = 0;
                }
            }
        }
        else if (fm & (U|D)) {
            if (orientation == 0) {
                idx = (true_tetrad == curr_tetrad) ? 2 : 1;
            }
            else if (orientation == 1) {
                if (curr_tetrad == 0) {
                    idx = 0;
                }
                else {
                    idx = (true_tetrad == curr_tetrad) ? 1 : 2;
                }
            }
            else if (orientation == 2) {
                if (curr_tetrad == 1) {
                    idx = 0;
                }
                else {
                    idx = (true_tetrad == curr_tetrad) ? 1 : 2;
                }
            }
        }
    }

    return colors[idx];
}

static Int8 decodeCubie(ColorCube cc, CubieEnum ce, int isEdge) {
    Int8 permutation, orientation;
    
    Color colors[3] = {};

    // get colors
    if (isEdge) {
        if (ce == FR) {
            colors[0] = cc.F.R; colors[1] = cc.R.L;
        }
        else if (ce == FL) {
            colors[0] = cc.F.L; colors[1] = cc.L.R;
        }
        else if (ce == UF) {
            colors[0] = cc.F.U; colors[1] = cc.U.D;
        }
        else if (ce == DF) {
            colors[0] = cc.F.D; colors[1] = cc.D.U;
        }
        else if (ce == BR) {
            colors[0] = cc.B.L; colors[1] = cc.R.R;
        }
        else if (ce == BL) {
            colors[0] = cc.B.R; colors[1] = cc.L.L;
        }
        else if (ce == UB) {
            colors[0] = cc.B.U; colors[1] = cc.U.U;
        }
        else if (ce == DB) {
            colors[0] = cc.B.D; colors[1] = cc.D.D;
        }
        else if (ce == UR) {
            colors[0] = cc.U.R; colors[1] = cc.R.U;
        }
        else if (ce == UL) {
            colors[0] = cc.U.L; colors[1] = cc.L.U;
        }
        else if (ce == DR) {
            colors[0] = cc.D.R; colors[1] = cc.R.D;
        }
        else if (ce == DL) {
            colors[0] = cc.D.L; colors[1] = cc.L.D;
        }
    }
    else {
        if (ce == UFR) {
            colors[0] = cc.F.UR; colors[1] = cc.R.UL; colors[2] = cc.U.DR;
        }
        else if (ce == UFL) {
            colors[0] = cc.F.UL; colors[1] = cc.L.UR; colors[2] = cc.U.DL;
        }
        else if (ce == UBR) {
            colors[0] = cc.B.UL; colors[1] = cc.R.UR; colors[2] = cc.U.UR;
        }
        else if (ce == UBL) {
            colors[0] = cc.B.UR; colors[1] = cc.L.UL; colors[2] = cc.U.UL;
        }
        else if (ce == DFR) {
            colors[0] = cc.F.DR; colors[1] = cc.R.DL; colors[2] = cc.D.UR;
        }
        else if (ce == DFL) {
            colors[0] = cc.F.DL; colors[1] = cc.L.DR; colors[2] = cc.D.UL;
        }
        else if (ce == DBR) {
            colors[0] = cc.B.DL; colors[1] = cc.R.DR; colors[2] = cc.D.DR;
        }
        else if (ce == DBL) {
            colors[0] = cc.B.DR; colors[1] = cc.L.DL; colors[2] = cc.D.DL;
        }
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
        if ((colors[0]|colors[1]|colors[2]) == (U|F|R)) {
            permutation = UFR;
        }
        else if ((colors[0]|colors[1]|colors[2]) == (U|F|L)) {
            permutation = UFL;
        }
        else if ((colors[0]|colors[1]|colors[2]) == (U|B|R)) {
            permutation = UBR;
        }
        else if ((colors[0]|colors[1]|colors[2]) == (U|B|L)) {
            permutation = UBL;
        }
        else if ((colors[0]|colors[1]|colors[2]) == (D|F|R)) {
            permutation = DFR;
        }
        else if ((colors[0]|colors[1]|colors[2]) == (D|F|L)) {
            permutation = DFL;
        }
        else if ((colors[0]|colors[1]|colors[2]) == (D|B|R)) {
            permutation = DBR;
        }
        else if ((colors[0]|colors[1]|colors[2]) == (D|B|L)) {
            permutation = DBL;
        }
    }

    // set orientation
    if (isEdge) {
        orientation = (colors[0] & (L|R) || ((colors[0] & (U|D)) && (colors[1] & (F|B))));
    }
    else {
        Int8 true_tetrad = cornerTetrad(permutation);
        Int8 curr_tetrad = cornerTetrad(ce);

        if (colors[0] & (F|B)) {
            orientation = 0;
        }
        else if (colors[0] & (R|L)) {
            orientation = (true_tetrad == 1) ? 2 : 1;
        }
        else {
            orientation = (true_tetrad == 1) ? 1 : 2;
        }
    }

    // return cubie encoding
    Int8 cubie;
    setPermutation(&cubie, isEdge, permutation);
    setOrientation(&cubie, isEdge, orientation);
    return cubie;
}

static ColorCube encodeCube(Cube128 c) {
    ColorCube cc;
    
    // set center cubies
    cc.U.C = U; 
    cc.D.C = D; 
    cc.F.C = F; 
    cc.B.C = B; 
    cc.R.C = R; 
    cc.L.C = L;
    
    // set edge cubies
    cc.U.D = encodeCubie(c, UF, 1, U);
    cc.F.U = encodeCubie(c, UF, 1, F);
    cc.U.U = encodeCubie(c, UB, 1, U);
    cc.B.U = encodeCubie(c, UB, 1, B);
    cc.U.R = encodeCubie(c, UR, 1, U);
    cc.R.U = encodeCubie(c, UR, 1, R);
    cc.U.L = encodeCubie(c, UL, 1, U);
    cc.L.U = encodeCubie(c, UL, 1, L);
    cc.D.U = encodeCubie(c, DF, 1, D);
    cc.F.D = encodeCubie(c, DF, 1, F);
    cc.D.D = encodeCubie(c, DB, 1, D);
    cc.B.D = encodeCubie(c, DB, 1, B);
    cc.D.R = encodeCubie(c, DR, 1, D);
    cc.R.D = encodeCubie(c, DR, 1, R);
    cc.D.L = encodeCubie(c, DL, 1, D);
    cc.L.D = encodeCubie(c, DL, 1, L);
    cc.F.R = encodeCubie(c, FR, 1, F);
    cc.R.L = encodeCubie(c, FR, 1, R);
    cc.F.L = encodeCubie(c, FL, 1, F);
    cc.L.R = encodeCubie(c, FL, 1, L);
    cc.B.L = encodeCubie(c, BR, 1, B);
    cc.R.R = encodeCubie(c, BR, 1, R);
    cc.B.R = encodeCubie(c, BL, 1, B);
    cc.L.L = encodeCubie(c, BL, 1, L);
            
    // set corner cubies
    cc.U.DR = encodeCubie(c, UFR, 0, U);
    cc.F.UR = encodeCubie(c, UFR, 0, F);
    cc.R.UL = encodeCubie(c, UFR, 0, R);
    cc.U.DL = encodeCubie(c, UFL, 0, U);
    cc.F.UL = encodeCubie(c, UFL, 0, F);
    cc.L.UR = encodeCubie(c, UFL, 0, L);
    cc.U.UR = encodeCubie(c, UBR, 0, U);
    cc.B.UL = encodeCubie(c, UBR, 0, B);
    cc.R.UR = encodeCubie(c, UBR, 0, R);
    cc.U.UL = encodeCubie(c, UBL, 0, U);
    cc.B.UR = encodeCubie(c, UBL, 0, B);
    cc.L.UL = encodeCubie(c, UBL, 0, L);
    cc.D.UR = encodeCubie(c, DFR, 0, D);
    cc.F.DR = encodeCubie(c, DFR, 0, F);
    cc.R.DL = encodeCubie(c, DFR, 0, R);
    cc.D.UL = encodeCubie(c, DFL, 0, D);
    cc.F.DL = encodeCubie(c, DFL, 0, F);
    cc.L.DR = encodeCubie(c, DFL, 0, L);
    cc.D.DR = encodeCubie(c, DBR, 0, D);
    cc.B.DL = encodeCubie(c, DBR, 0, B);
    cc.R.DR = encodeCubie(c, DBR, 0, R);
    cc.D.DL = encodeCubie(c, DBL, 0, D);
    cc.B.DR = encodeCubie(c, DBL, 0, B);
    cc.L.DL = encodeCubie(c, DBL, 0, L);
    
    return cc;
}

static Cube128 decodeCube(ColorCube cc) {
    Cube128 c = { 0, 0 };

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        setCubie(&c.edges, i, decodeCubie(cc, i, 1));
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        setCubie(&c.corners, i, decodeCubie(cc, i, 0));
    }

    return c;
}

static void printColor(Color color) {
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

static void printColorCube(ColorCube cc) {
    ColorFace faces[6] = { cc.U, cc.L, cc.F, cc.R, cc.B, cc.D };

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

void printCube(Cube128 c) {
    ColorCube cc = encodeCube(c);
    printColorCube(cc);
}