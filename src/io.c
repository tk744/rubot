#include "rubot.h"
#include <stdio.h>

typedef Move Color; // to reuse cubie enums

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

static Int8 cubieFromColor(ColorCube cc, CubieEnum ce, int isEdge) {
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

static Color cubieToColor(Cube128 c, CubieEnum ce, int isEdge, Color f) {
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
        if ((f & (F|B)) || ((f & (U|D)) && (ce == UR || ce == UL || ce == DR || ce == DL))) {
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

        if (f & (F|B)) {
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
        else if (f & (R|L)) {
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
        else if (f & (U|D)) {
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

static Cube128 fromColor(ColorCube cc) {
    Cube128 c = { 0, 0 };

    Int64 i;
    for(i=0 ; i<NUM_EDGES ; i++) {
        setCubie(&c.edges, i, cubieFromColor(cc, i, 1));
    }
    for(i=0 ; i<NUM_CORNERS ; i++) {
        setCubie(&c.corners, i, cubieFromColor(cc, i, 0));
    }

    return c;
}

static ColorCube toColor(Cube128 c) {
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
    int i, j; // i = row, j = column
    Color up_face[3][3] = {
        { cc.U.UL, cc.U.U, cc.U.UR },
        { cc.U.L, cc.U.C, cc.U.R },
        { cc.U.DL, cc.U.D, cc.U.DR }
    };
    printf("        ┌───────┐\n");
    for (i=0 ; i<3 ; i++) {
        printf("        │ ");
        for (j=0 ; j<3 ; j++) {
            printColor(up_face[i][j]);
            printf(" ");
        }
        printf("│\n");
    }

    printf("┌───────┼───────┼───────┬───────┐\n");
    ColorFace faces[4] = { cc.L, cc.F, cc.R, cc.B };
    int k; // k = face
    for (i=0 ; i<3 ; i++) {
        for (k=0 ; k<4 ; k++) {
            printf("│ ");
            for (j=0 ; j<3 ; j++) {
                if (i==0 && j==0) {
                    printColor(faces[k].UL);
                }
                else if (i==0 && j==1) {
                    printColor(faces[k].U);
                }
                else if (i==0 && j==2) {
                    printColor(faces[k].UR);
                }
                else if (i==1 && j==0) {
                    printColor(faces[k].L);
                }
                else if (i==1 && j==1) {
                    printColor(faces[k].C);
                }
                else if (i==1 && j==2) {
                    printColor(faces[k].R);
                }
                else if (i==2 && j==0) {
                    printColor(faces[k].DL);
                }
                else if (i==2 && j==1) {
                    printColor(faces[k].D);
                }
                else if (i==2 && j==2) {
                    printColor(faces[k].DR);
                }
                printf(" ");
            }
        }
        printf("│\n");
    }
    printf("└───────┼───────┼───────┴───────┘\n");

    Color down_face[3][3] = {
        { cc.D.UL, cc.D.U, cc.D.UR },
        { cc.D.L, cc.D.C, cc.D.R },
        { cc.D.DL, cc.D.D, cc.D.DR }
    };
    for (i=0 ; i<3 ; i++) {
        printf("        │ ");
        for (j=0 ; j<3 ; j++) {
            printColor(down_face[i][j]);
            printf(" ");
        }
        printf("│\n");
    }

    printf("        └───────┘\n");
}

void printCube(Cube128 c) {
    ColorCube cc = toColor(c);
    printColorCube(cc);
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
        printf(n == 0 ? "" : " ");
    }
    printf("\n");
}

static Color charToColor(char x, char cU, char cL, char cF, char cR, char cB, char cD) {
    if (x == cU) {
        return U;
    }
    else if (x == cL) {
        return L;
    }
    else if (x == cF) {
        return F;
    }
    else if (x == cR) {
        return R;
    }
    else if (x == cB) {
        return B;
    }
    else if (x == cD) {
        return D;
    }
    return NOP;
}

int decodeCube(Cube128 *c, char *str) {
    ColorCube cc = toColor(*c);
    int i;
    for (i=0 ; i<54 ; i++) {
        ColorFace *face;
        switch (i/9) {
            case 0: face = &cc.U; break;
            case 1: face = &cc.L; break;
            case 2: face = &cc.F; break;
            case 3: face = &cc.R; break;
            case 4: face = &cc.B; break;
            case 5: face = &cc.D; break;
        }
        
        Color *color;
        switch (i%9) {
            case 0: color = &face->UL; break;
            case 1: color = &face->U; break;
            case 2: color = &face->UR; break;
            case 3: color = &face->L; break;
            case 4: color = &face->C; break;
            case 5: color = &face->R; break;
            case 6: color = &face->DL; break;
            case 7: color = &face->D; break;
            case 8: color = &face->DR; break;
        }

        *color = charToColor(*(str+i), *(str+4), *(str+13), *(str+22), *(str+31), *(str+40), *(str+49));
        if (*color == NOP) {
            return i;
        }
    }

    *c = fromColor(cc);
    return 0;
}

void encodeCube(Cube128 *c, char *str) {
    ColorCube cc = toColor(*c);
    int i;
    for (i=0 ; i<54 ; i++) {
        ColorFace *face;
        switch (i/9) {
            case 0: face = &cc.U; break;
            case 1: face = &cc.L; break;
            case 2: face = &cc.F; break;
            case 3: face = &cc.R; break;
            case 4: face = &cc.B; break;
            case 5: face = &cc.D; break;
        }
        
        Color *color;
        switch (i%9) {
            case 0: color = &face->UL; break;
            case 1: color = &face->U; break;
            case 2: color = &face->UR; break;
            case 3: color = &face->L; break;
            case 4: color = &face->C; break;
            case 5: color = &face->R; break;
            case 6: color = &face->DL; break;
            case 7: color = &face->D; break;
            case 8: color = &face->DR; break;
        }

        *str++ = *color == U ? 'U' : *color == L ? 'L' : *color == F ? 'F' : *color == R ? 'R' : *color == B ? 'B' : *color == D ? 'D' : '?';
    }
}

int decodeMove(Move *m, char *str) {
    Move mm = 0;

    // check first character
    switch (*str++) {
        case 'U': mm |= U; break;
        case 'D': mm |= D; break;
        case 'F': mm |= F; break;
        case 'B': mm |= B; break;
        case 'R': mm |= R; break;
        case 'L': mm |= L; break;
        default: 
            return -1;
    }

    // check second character
    if (*str == '\0') {
        *m = mm;
        return 0;
    }
    else if (*str == '\'') {
        mm |= I;
    }
    else if (*str == '2') {
        mm |= H;
    }
    else {
        return -1;
    }
    
    // check for trailing characters
    if (*(str+1) != '\0') {
        return -1;
    }

    *m = mm;
    return 0;
}