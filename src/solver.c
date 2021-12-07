#include <stdio.h>
#include "solver.h"
#include "util.h"

static int phaseMaxDepth(int phase) {
    static int phase_maxdepth[4] = { 7, 10, 14, 15 };
    return (1 <= phase && phase <= 4) ? phase_maxdepth[phase-1] : 0;
}

static int phaseMoveset(int phase, Move *ms) {
    static Move phase_moveset[4][NUM_MOVES] = { 
    { MV_MASK_U, MV_MASK_U|MV_MASK_I, MV_MASK_U|MV_MASK_H, MV_MASK_D, MV_MASK_D|MV_MASK_I, MV_MASK_D|MV_MASK_H, MV_MASK_R, MV_MASK_R|MV_MASK_I, MV_MASK_R|MV_MASK_H, MV_MASK_L, MV_MASK_L|MV_MASK_I, MV_MASK_L|MV_MASK_H, MV_MASK_F, MV_MASK_F|MV_MASK_I, MV_MASK_F|MV_MASK_H, MV_MASK_B, MV_MASK_B|MV_MASK_I, MV_MASK_B|MV_MASK_H },
    {         MV_MASK_U|MV_MASK_H,         MV_MASK_D|MV_MASK_H, MV_MASK_R, MV_MASK_R|MV_MASK_I, MV_MASK_R|MV_MASK_H, MV_MASK_L, MV_MASK_L|MV_MASK_I, MV_MASK_L|MV_MASK_H, MV_MASK_F, MV_MASK_F|MV_MASK_I, MV_MASK_F|MV_MASK_H, MV_MASK_B, MV_MASK_B|MV_MASK_I, MV_MASK_B|MV_MASK_H },
    {         MV_MASK_U|MV_MASK_H,         MV_MASK_D|MV_MASK_H,         MV_MASK_R|MV_MASK_H,         MV_MASK_L|MV_MASK_H, MV_MASK_F, MV_MASK_F|MV_MASK_I, MV_MASK_F|MV_MASK_H, MV_MASK_B, MV_MASK_B|MV_MASK_I, MV_MASK_B|MV_MASK_H },
    {         MV_MASK_U|MV_MASK_H,         MV_MASK_D|MV_MASK_H,         MV_MASK_R|MV_MASK_H,         MV_MASK_L|MV_MASK_H,         MV_MASK_F|MV_MASK_H,         MV_MASK_B|MV_MASK_H } };

    if (1 <= phase && phase <= 4) {
        int n;
        for (n=0 ; phase_moveset[phase-1][n] != MV_MASK_NOP && n < NUM_MOVES ; n++) {
            *(ms+n) = *(phase_moveset[phase-1]+n);
        }
        return n;
    }
    return 0;
}

static Cube phaseCube(int phase, Cube c) {
    if (phase == 1) {       // fix edge orientations
        // create 1 edge permutation group
        int i;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            setPermutation(&cubie, 1, 0);
            setCubie(&c.edges, i, cubie);
        }
        // create 1 corner permutation and orientation group
        c.corners = 0;
    }
    else if (phase == 2) {  // fix corner orientations, 1 slice edges
        // create 2 edge permutation groups: { UR, UL, DR, DL} and rest
        int i;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == ECUBIE_UR || p == ECUBIE_UL || p == ECUBIE_DR || p == ECUBIE_DL) {
                setPermutation(&cubie, 1, ECUBIE_UR);
            }
            else {
                setPermutation(&cubie, 1, ECUBIE_UF);
            }
            setCubie(&c.edges, i, cubie);
        }

        // create 1 corner permutation group
        for(i=0 ; i<NUM_CORNERS ; i++) {
            Int8 cubie = getCubie(c.corners, i);
            setPermutation(&cubie, 0, 0);
            setCubie(&c.corners, i, cubie);
        }
    }
    else if (phase == 3) {  // fix edge slices, corner tetrad pairs
        // create 3 edge permutation groups: { UF, UB, DF, DB }, { UR, UL, DR, DL}, { FR, FL, BR, BL }
        int i;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == ECUBIE_UF || p == ECUBIE_UB || p == ECUBIE_DF || p == ECUBIE_DB) {
                setPermutation(&cubie, 1, ECUBIE_UF);
            }
            else if (p == ECUBIE_UR || p == ECUBIE_UL || p == ECUBIE_DR || p == ECUBIE_DL) {
                setPermutation(&cubie, 1, ECUBIE_UR);
            }
            else if (p == ECUBIE_FR || p == ECUBIE_FL || p == ECUBIE_BR || p == ECUBIE_BL) {
                setPermutation(&cubie, 1, ECUBIE_FR);
            }
            setCubie(&c.edges, i, cubie);
        }

        // create 4 corner permutation groups: { UFR, UBL }, { UFL, UBR }, { DFR, DBL }, { DFL, DBR }
        for(i=0 ; i<NUM_CORNERS ; i++) {
            Int8 cubie = getCubie(c.corners, i);
            Int8 p = getPermutation(cubie, 0);
            if (p == CCUBIE_UFR || p == CCUBIE_UBL) {
                setPermutation(&cubie, 0, CCUBIE_UFR);
            }
            else if (p == CCUBIE_UFL || p == CCUBIE_UBR) {
                setPermutation(&cubie, 0, CCUBIE_UFL);
            }
            else if (p == CCUBIE_DFR || p == CCUBIE_DBL) {
                setPermutation(&cubie, 0, CCUBIE_DFR);
            }
            else if (p == CCUBIE_DFL || p == CCUBIE_DBR) {
                setPermutation(&cubie, 0, CCUBIE_DFL);
            }
            setCubie(&c.corners, i, cubie);
        }
    }
    return c;
}

int solve(Cube c, Move *ms) {
    // create IDDFS stack
    static Node ns[STACK_SIZE] = {};
    static Stack s = { ns, 0 };

    // iterate through phases
    int phase = 0, offset_depth = 0;
    next_phase: while (++phase <= 4) {
        // FOR DEBUGGING PURPOSES ONLY
        printf(phase == 1 ? "" : "\n");
        Move null[MAX_MOVES];
        int num_moves = phaseMoveset(phase, null);

        // create goal cube and root node
        Cube goal_cube = phaseCube(phase, cubeFactory());
        Cube root_cube = phaseCube(phase, c);
        Node root_node = { root_cube, MV_MASK_NOP, 0 };

        // iterative deepening
        int max_depth = -1;
        while (++max_depth <= phaseMaxDepth(phase)) {
            // FOR DEBUGGING PURPOSES ONLY
            printf("\rPhase: %d | Branching factor: %2d | Max depth: %2d | Depth: %2d", phase, num_moves, phaseMaxDepth(phase), max_depth);
            fflush(stdout);

            // initialize IDDFS stack with root node
            clear(&s);
            push(&s, root_node);

            // iterate through stack
            while (s.size > 0) {
                // pop node from stack
                Node node = pop(&s);

                // update move sequence if node is not root
                if (node.depth > 0) {
                    int idx = offset_depth + node.depth-1;
                    ms[idx] = node.move;
                }

                // finish phase if node is goal
                if (areEqual(node.cube, goal_cube)) {
                    c = applyMoves(c, ms+offset_depth, node.depth);
                    offset_depth += node.depth;
                    goto next_phase;
                }

                // add children to stack if max depth is not reached
                if (node.depth < max_depth) {
                    Move moveset[NUM_MOVES];
                    int i = 0, n = phaseMoveset(phase, moveset);
                    while(i < n) {
                        Move m = moveset[i++];
                        
                        // skip redundant moves
                        if ((m & ~(MV_MASK_H|MV_MASK_I)) & (node.move & ~(MV_MASK_H|MV_MASK_I))) {
                            continue;
                        }

                        Cube next_cube = applyMove(node.cube, m);
                        next_cube = phaseCube(phase, next_cube);
                        Node next_node = { next_cube, m, node.depth+1 };
                        push(&s, next_node);
                    }
                }
            }
        }
        return -1;
    }
    return offset_depth;
}