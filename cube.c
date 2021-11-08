#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* This source file implements the state and behavior of a Rubik's cube. */

/* TODO:
 * 1. Finish X, Y, Z transformations in move() function.
 * 1. Implement orientation in move() function.
 * 2. Implement is_solved() function. 
 * 3. Fix and test cube state save and load functions.
 * 4. Documentation about how enums relate to state. */

/* Global constants for individual cubie information. 
 * Type uint8_t instead of enum because it reduces cube state memory by 4x. */

// center cubie positions
uint8_t UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5;
// edge cubie positions
uint8_t UL = 0, UR = 1, UF = 2, UB = 3, DL = 4, DR = 5, \
        DF = 6, DB = 7, LF = 8, LB = 9, RF = 10, RB = 11;
// corner cubie positions
uint8_t ULF = 0, ULB = 1, URF = 2, URB = 3, \
        DLF = 4, DLB = 5, DRF = 6, DRB = 7;

// center cubie colors
uint8_t WHITE = 0, YELLOW = 1, GREEN = 2, \
        BLUE = 3, RED = 4, ORANGE = 5;
// edge cubie colors
uint8_t WG = 0, WB = 1, WR = 2, WO = 3, YG = 4, YB = 5, \
        YR = 6, YO = 7, GR = 8, GO = 9, BR = 10, BO = 11;
// corner cubie colors
uint8_t WRG = 0, WRB = 1, WOG = 2, WOB = 3, \
        YRG = 4, YRB = 5, YOG = 6, YOB = 7;

// edge (x2) and corner (x3) orientations
uint8_t O1 = 1, O2 = 2, O3 = 3;

/* Cube state definition.
 * Represented by lists of center, edge, and corner cubies.
 * Each list is indexed by valid cubie position.
 * E.g. centers[UP], edges[UL], and corners[ULF] are valid. */
struct Cube {
    struct Center {
        uint8_t color;
    } centers[6];
    
    struct Edge {
        uint8_t color;
        uint8_t orientation;
    } edges[12];

    struct Corner {
        uint8_t color;
        uint8_t orientation;
    } corners[8];
};

/* Convert a cube state into a string. */
/*
char *save_cube(struct Cube *st) {
    char str[sizeof(struct Cube)/sizeof(uint8_t)+1]; // +1 for terminal character

    short int i = 0;
    for ( ; i<6+(2*12)+(2*8) ; i++ ) {
        if (i<6) {                  // center cubie colors
            sprintf(str+i, "%c", 'a' + st->centers[i].color);
        }
        else if (i<6+12) {          // edge cubie colors
            sprintf(str+i, "%c", 'a' + st.edges[i-6].color);
        }
        else if (i<6+(2*12)) {      // edge cubie orientations
            sprintf(str+i, "%c", 'a' + st.edges[i-(6+12)].orientation);
        }
        else if (i<6+(2*12)+8) {    // corner cubie colors
            sprintf(str+i, "%c", 'a' + st.corners[i-(6+(2*12))].color);
        }
        else {                      // corner cubie orientations
            sprintf(str+i, "%c", 'a' + st.corners[i-(6+(2*12)+8)].orientation);
        }
    }

    return str;
}
*/

/* Convert a string into a cube state. */
/*
struct Cube load_cube(char *str) {
    struct Cube st;

    short int i = 0, v;
    for ( ; i<6+(2*12)+(2*8) ; i++ ) {
        // TODO: fix second argument of this line
        v = strtol(str+i, str+(i+1), 0) - 'a';
        if (i<6) {                  // center cubie colors
            st.centers[i].color = v;
        }
        else if (i<6+12) {          // edge cubie colors
            st.edges[i-6].color = v;
        }
        else if (i<6+(2*12)) {      // edge cubie orientations
            st.edges[i-(6+12)].orientation = v;
        }
        else if (i<6+(2*12)+8) {    // corner cubie colors
            st.corners[i-(6+(2*12))].color = v;
        }
        else {                      // corner cubie orientations
            st.corners[i-(6+(2*12)+8)].orientation = v;
        }
    }

    return st;
}
*/

/* TODO: Returns 1 if cube `st` is in a solved state, 0 otherwise. */
int is_solved(struct Cube st) {
    return 0;
}

/* Set of all possible transformations on a cube state. */
enum Move {
    // face rotations
    U, UI, U2, UI2,
    D, DI, D2, DI2,
    L, LI, L2, LI2,
    R, RI, R2, RI2,
    F, FI, F2, FI2,
    B, BI, B2, BI2,
    
    // cube rotations
    X, XI,
    Y, YI,
    Z, ZI
};

/* Returns the cube state obtained by applying the move `mv` to cube `st`.
 * Does not modify the original cube `st`. */
struct Cube move(struct Cube st, enum Move mv) {
    // half-turn transformations
    if (mv == U2) {
        return move(move(st, U), U);
    }
    else if (mv == UI2) {
        return move(move(st, UI), UI);
    }
    else if (mv == D2) {
        return move(move(st, D), D);
    }
    else if (mv == DI2) {
        return move(move(st, DI), DI);
    }
    else if (mv == L2) {
        return move(move(st, L), L);
    }
    else if (mv == LI2) {
        return move(move(st, LI), LI);
    }
    else if (mv == R2) {
        return move(move(st, R), R);
    }
    else if (mv == RI2) {
        return move(move(st, RI), RI);
    }
    else if (mv == F2) {
        return move(move(st, F), F);
    }
    else if (mv == FI2) {
        return move(move(st, FI), FI);
    }
    else if (mv == B2) {
        return move(move(st, B), B);
    }
    else if (mv == BI2) {
        return move(move(st, BI), BI);
    }
    
    // center cubie transformations
    uint8_t tmp_center;
    if (mv == X) {
        tmp_center = st.centers[UP].color;
        st.centers[UP].color = st.centers[FRONT].color;
        st.centers[FRONT].color = st.centers[DOWN].color;
        st.centers[DOWN].color = st.centers[BACK].color;
        st.centers[BACK].color = tmp_center;
    }
    else if (mv == XI) {
        tmp_center = st.centers[UP].color;
        st.centers[UP].color = st.centers[BACK].color;
        st.centers[BACK].color = st.centers[DOWN].color;
        st.centers[DOWN].color = st.centers[FRONT].color;
        st.centers[FRONT].color = tmp_center;
    }
    else if (mv == Y) {

    }
    else if (mv == YI) {

    }
    else if (mv == Z) {

    }
    else if (mv == ZI) {

    }
    
    // edge and corner cubie transformations
    uint8_t tmp_edge, tmp_corner;
    if (mv == U || mv == Y) {
        tmp_edge = st.edges[UF].color;
        st.edges[UF].color = st.edges[UR].color;
        st.edges[UR].color = st.edges[UB].color;
        st.edges[UB].color = st.edges[UL].color;
        st.edges[UL].color = tmp_edge;

        tmp_corner = st.corners[URF].color;
        st.corners[URF].color = st.corners[URB].color;
        st.corners[URB].color = st.corners[ULB].color;
        st.corners[ULB].color = st.corners[ULF].color;
        st.corners[ULF].color = tmp_corner;
    }
    if (mv == UI || mv == YI) {
        tmp_edge = st.edges[UF].color;
        st.edges[UF].color = st.edges[UL].color;
        st.edges[UL].color = st.edges[UB].color;
        st.edges[UB].color = st.edges[UR].color;
        st.edges[UR].color = tmp_edge;

        tmp_corner = st.corners[URF].color;
        st.corners[URF].color = st.corners[ULF].color;
        st.corners[ULF].color = st.corners[ULB].color;
        st.corners[ULB].color = st.corners[URB].color;
        st.corners[URB].color = tmp_corner;
    }
    if (mv == D || mv == YI) {
        tmp_edge = st.edges[DF].color;
        st.edges[DF].color = st.edges[DL].color;
        st.edges[DL].color = st.edges[DB].color;
        st.edges[DB].color = st.edges[DR].color;
        st.edges[DR].color = tmp_edge;

        tmp_corner = st.corners[DRF].color;
        st.corners[DRF].color = st.corners[DLF].color;
        st.corners[DLF].color = st.corners[DLB].color;
        st.corners[DLB].color = st.corners[DRB].color;
        st.corners[DRB].color = tmp_corner;
    }
    if (mv == DI || mv == Y) {
        tmp_edge = st.edges[DF].color;
        st.edges[DF].color = st.edges[DR].color;
        st.edges[DR].color = st.edges[DB].color;
        st.edges[DB].color = st.edges[DL].color;
        st.edges[DL].color = tmp_edge;

        tmp_corner = st.corners[DRF].color;
        st.corners[DRF].color = st.corners[DRB].color;
        st.corners[DRB].color = st.corners[DLB].color;
        st.corners[DLB].color = st.corners[DLF].color;
        st.corners[DLF].color = tmp_corner;
    }
    if (mv == R || mv == X) {
        tmp_edge = st.edges[RF].color;
        st.edges[RF].color = st.edges[DR].color;
        st.edges[DR].color = st.edges[RB].color;
        st.edges[RB].color = st.edges[UR].color;
        st.edges[UR].color = tmp_edge;

        tmp_corner = st.corners[DRF].color;
        st.corners[DRF].color = st.corners[DRB].color;
        st.corners[DRB].color = st.corners[URB].color;
        st.corners[URB].color = st.corners[URF].color;
        st.corners[URF].color = tmp_corner;
    }
    if (mv == RI || mv == XI) {
        tmp_edge = st.edges[RF].color;
        st.edges[RF].color = st.edges[UR].color;
        st.edges[UR].color = st.edges[RB].color;
        st.edges[RB].color = st.edges[DR].color;
        st.edges[DR].color = tmp_edge;

        tmp_corner = st.corners[URF].color;
        st.corners[URF].color = st.corners[URB].color;
        st.corners[URB].color = st.corners[DRB].color;
        st.corners[DRB].color = st.corners[DRF].color;
        st.corners[DRF].color = tmp_corner;
    }
    if (mv == L || mv == XI) {
        tmp_edge = st.edges[LF].color;
        st.edges[LF].color = st.edges[UL].color;
        st.edges[UL].color = st.edges[LB].color;
        st.edges[LB].color = st.edges[DL].color;
        st.edges[DL].color = tmp_edge;

        tmp_corner = st.corners[ULF].color;
        st.corners[ULF].color = st.corners[ULB].color;
        st.corners[ULB].color = st.corners[DLB].color;
        st.corners[DLB].color = st.corners[DLF].color;
        st.corners[DLF].color = tmp_corner;
    }
    if (mv == LI || mv == X) {
        tmp_edge = st.edges[LF].color;
        st.edges[LF].color = st.edges[DL].color;
        st.edges[DL].color = st.edges[LB].color;
        st.edges[LB].color = st.edges[UL].color;
        st.edges[UL].color = tmp_edge;

        tmp_corner = st.corners[DLF].color;
        st.corners[DLF].color = st.corners[DLB].color;
        st.corners[DLB].color = st.corners[ULB].color;
        st.corners[ULB].color = st.corners[ULF].color;
        st.corners[ULF].color = tmp_corner;
    }
    if (mv == F || mv == Z) {
        tmp_edge = st.edges[UF].color;
        st.edges[UF].color = st.edges[LF].color;
        st.edges[LF].color = st.edges[DF].color;
        st.edges[DF].color = st.edges[RF].color;
        st.edges[RF].color = tmp_edge;

        tmp_corner = st.corners[URF].color;
        st.corners[URF].color = st.corners[ULF].color;
        st.corners[ULF].color = st.corners[DLF].color;
        st.corners[DLF].color = st.corners[DRF].color;
        st.corners[DRF].color = tmp_corner;
    }
    if (mv == FI || mv == ZI) {
        tmp_edge = st.edges[UF].color;
        st.edges[UF].color = st.edges[RF].color;
        st.edges[RF].color = st.edges[DF].color;
        st.edges[DF].color = st.edges[LF].color;
        st.edges[LF].color = tmp_edge;

        tmp_corner = st.corners[ULF].color;
        st.corners[ULF].color = st.corners[URF].color;
        st.corners[URF].color = st.corners[DRF].color;
        st.corners[DRF].color = st.corners[DLF].color;
        st.corners[DLF].color = tmp_corner;
    }
    if (mv == B || mv == ZI) {
        tmp_edge = st.edges[UB].color;
        st.edges[UB].color = st.edges[RB].color;
        st.edges[RB].color = st.edges[DB].color;
        st.edges[DB].color = st.edges[LB].color;
        st.edges[LB].color = tmp_edge;

        tmp_corner = st.corners[ULB].color;
        st.corners[ULB].color = st.corners[URB].color;
        st.corners[URB].color = st.corners[DRB].color;
        st.corners[DRB].color = st.corners[DLB].color;
        st.corners[DLB].color = tmp_corner;
    }
    if (mv == BI || mv == Z) {
        tmp_edge = st.edges[UB].color;
        st.edges[UB].color = st.edges[LB].color;
        st.edges[LB].color = st.edges[DB].color;
        st.edges[DB].color = st.edges[RB].color;
        st.edges[RB].color = tmp_edge;

        tmp_corner = st.corners[URB].color;
        st.corners[URB].color = st.corners[ULB].color;
        st.corners[ULB].color = st.corners[DLB].color;
        st.corners[DLB].color = st.corners[DRB].color;
        st.corners[DRB].color = tmp_corner;
    }
    
    return st;
}

/* Writes the shortest list of moves into `mvs` which orients the cube `st` 
 * with the white face down and red face FRONT.
 * Returns the number of moves in the sequence.
 * Requires that the length of `mvs` is at least 4. 
 */
int orient(enum Move *mvs, struct Cube st) {
    int i = 0;
    while (st.centers[DOWN].color != WHITE || st.centers[FRONT].color != RED) {
        // TODO: go thru all cases
        enum Move mv;
        // 1-move away
        if (st.centers[BACK].color == WHITE && st.centers[DOWN].color == RED) {
            mv = X;
        }
        else if (st.centers[FRONT].color == WHITE && st.centers[UP].color == RED) {
            mv = XI;
        }
        else if (st.centers[LEFT].color == WHITE && st.centers[FRONT].color == RED) {
            mv = ZI;
        }
        else if (st.centers[RIGHT].color == WHITE && st.centers[FRONT].color == RED) {
            mv = Z;
        }
        else if (st.centers[DOWN].color == WHITE && st.centers[LEFT].color == RED) {
            mv = YI;
        }
        else if (st.centers[DOWN].color == WHITE && st.centers[RIGHT].color == RED) {
            mv = Y;
        }
        else if (st.centers[UP].color == WHITE && st.centers[BACK].color == RED) {
            mv = X;
        }
        // 2-moves away ...
        // 3-moves away ...
        else {
            // TODO: error handling
            return -1;
        }
        st = move(st, (mvs[i++] = mv));
    }

    return i;
}

int main() {
    printf("%d bytes required to store cube state.", sizeof(struct Cube));
}
