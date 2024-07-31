#include "cube.h"
#include <stdio.h>

typedef MoveMask Color; // to use same enums

typedef struct {
    Color C, U, D, R, L, UR, UL, DR, DL;
} ColorFace;

typedef struct {
    ColorFace U, D, F, B, R, L;
} ColorCube;

static Int8 colorToCubie(ColorCube cc, CubieEnum ce, int isEdge) {
    Int8 permutation, orientation;
    
    Color colors[3] = {};

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
    Int8 cubie;
    setPermutation(&cubie, isEdge, permutation);
    setOrientation(&cubie, isEdge, orientation);
    return cubie;
}

static Cube128 convertColorCube(ColorCube cc) {
    Cube128 c = { 0, 0 };

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        setCubie(&c.edges, i, colorToCubie(cc, i, 1));
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        setCubie(&c.corners, i, colorToCubie(cc, i, 0));
    }

    return c;
}

static Color cubieToColor(Cube128 c, CubieEnum ce, int isEdge, Color fm) {
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
        int permutation_tetrad, ce_tetrad;

        if (permutation == UFR || permutation == DFL || permutation == UBL || permutation == DBR) {
            permutation_tetrad = 1;
        }
        else {
            permutation_tetrad = 0;
        }

        if (ce == UFR || ce == DFL || ce == UBL || ce == DBR) {
            ce_tetrad = 1;
        }
        else {
            ce_tetrad = 0;
        }

        if (fm & (F|B)) {
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
        else if (fm & (R|L)) {
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
        else if (fm & (U|D)) {
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

static ColorCube convertCube(Cube128 c) {
    ColorCube cc;
    
    // set center cubies
    cc.U.C = U; 
    cc.D.C = D; 
    cc.F.C = F; 
    cc.B.C = B; 
    cc.R.C = R; 
    cc.L.C = L;
    
    // set edge cubies
    cc.U.D = cubieToColor(c, UF, 1, U);
    cc.F.U = cubieToColor(c, UF, 1, F);
    cc.U.U = cubieToColor(c, UB, 1, U);
    cc.B.U = cubieToColor(c, UB, 1, B);
    cc.U.R = cubieToColor(c, UR, 1, U);
    cc.R.U = cubieToColor(c, UR, 1, R);
    cc.U.L = cubieToColor(c, UL, 1, U);
    cc.L.U = cubieToColor(c, UL, 1, L);
    cc.D.U = cubieToColor(c, DF, 1, D);
    cc.F.D = cubieToColor(c, DF, 1, F);
    cc.D.D = cubieToColor(c, DB, 1, D);
    cc.B.D = cubieToColor(c, DB, 1, B);
    cc.D.R = cubieToColor(c, DR, 1, D);
    cc.R.D = cubieToColor(c, DR, 1, R);
    cc.D.L = cubieToColor(c, DL, 1, D);
    cc.L.D = cubieToColor(c, DL, 1, L);
    cc.F.R = cubieToColor(c, FR, 1, F);
    cc.R.L = cubieToColor(c, FR, 1, R);
    cc.F.L = cubieToColor(c, FL, 1, F);
    cc.L.R = cubieToColor(c, FL, 1, L);
    cc.B.L = cubieToColor(c, BR, 1, B);
    cc.R.R = cubieToColor(c, BR, 1, R);
    cc.B.R = cubieToColor(c, BL, 1, B);
    cc.L.L = cubieToColor(c, BL, 1, L);
            
    // set corner cubies
    cc.U.DR = cubieToColor(c, UFR, 0, U);
    cc.F.UR = cubieToColor(c, UFR, 0, F);
    cc.R.UL = cubieToColor(c, UFR, 0, R);
    cc.U.DL = cubieToColor(c, UFL, 0, U);
    cc.F.UL = cubieToColor(c, UFL, 0, F);
    cc.L.UR = cubieToColor(c, UFL, 0, L);
    cc.U.UR = cubieToColor(c, UBR, 0, U);
    cc.B.UL = cubieToColor(c, UBR, 0, B);
    cc.R.UR = cubieToColor(c, UBR, 0, R);
    cc.U.UL = cubieToColor(c, UBL, 0, U);
    cc.B.UR = cubieToColor(c, UBL, 0, B);
    cc.L.UL = cubieToColor(c, UBL, 0, L);
    cc.D.UR = cubieToColor(c, DFR, 0, D);
    cc.F.DR = cubieToColor(c, DFR, 0, F);
    cc.R.DL = cubieToColor(c, DFR, 0, R);
    cc.D.UL = cubieToColor(c, DFL, 0, D);
    cc.F.DL = cubieToColor(c, DFL, 0, F);
    cc.L.DR = cubieToColor(c, DFL, 0, L);
    cc.D.DR = cubieToColor(c, DBR, 0, D);
    cc.B.DL = cubieToColor(c, DBR, 0, B);
    cc.R.DR = cubieToColor(c, DBR, 0, R);
    cc.D.DL = cubieToColor(c, DBL, 0, D);
    cc.B.DR = cubieToColor(c, DBL, 0, B);
    cc.L.DL = cubieToColor(c, DBL, 0, L);
    
    return cc;
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
    ColorCube cc = convertCube(c);
    printColorCube(cc);
}

int main() {
    int n = 1524; // todo - shouldn't have to write 10 twice
    Move ms_scramble[n];
    Cube128 c1 = scramble(solvedCube(), ms_scramble, n);
    ColorCube cc = convertCube(c1);
    Cube128 c2 = convertColorCube(cc);
    
    printCube(c1);
    printColorCube(cc);
    printCube(c2);
}