#include <stdio.h>
#include "solver.h"
#include "util.h"

/*
TODO:
1. phaseCube() phases 3
*/

static int phaseMaxDepth(int phase) {
    static int phase_maxdepth[4] = { 7, 13, 15, 17 };
    return (1 <= phase && phase <= 4) ? phase_maxdepth[phase-1] : 0;
}

static int phaseMoveset(int phase, Move *ms) {
    static Move phase_moveset[4][NUM_MOVES] = { 
        { U, U|I, U|H, D, D|I, D|H, R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
        {         U|H,         D|H, R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
        {         U|H,         D|H, R, R|I, R|H, L, L|I, L|H,         F|H,         B|H },
        {         U|H,         D|H,         R|H,         L|H,         F|H,         B|H } };

    if (1 <= phase && phase <= 4) {
        int n;
        for (n=0 ; phase_moveset[phase-1][n] != NOP && n < NUM_MOVES ; n++) {
            *(ms+n) = *(phase_moveset[phase-1]+n);
        }
        return n;
    }
    return 0;
}

static Cube phaseCube(int phase, Cube c) {
    if (phase == 1) {       // edge orientations
        // zero edge permutations
        int i;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            setPermutation(&cubie, 1, 0);
            setCubie(&c.edges, i, cubie);
        }
        // zero corner permutations and orientations
        c.corners = 0;
    }
    else if (phase == 2) {  // corner orientations, E-slice edges
        // zero edge permutations except for FR, FL, BR, BL
        int i;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == FR || p == FL || p == BR || p == BL) {
                continue;
            }
            else {
                setPermutation(&cubie, 1, 0);
                setCubie(&c.edges, i, cubie);
            }
        }

        // zero corner permutations
        for(i=0 ; i<NUM_CORNERS ; i++) {
            Int8 cubie = getCubie(c.corners, i);
            setPermutation(&cubie, 0, 0);
            setCubie(&c.corners, i, cubie);
        }
    }
    else if (phase == 3) {  // M/S-slice edges, corner tetrad, even parity
        // TODO
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
        // create goal cube and root node
        Cube goal_cube = phaseCube(phase, cubeFactory());
        Cube root_cube = phaseCube(phase, c);
        Node root_node = { root_cube, NOP, 0 };

        // iterative deepening
        int max_depth = 0;
        while (++max_depth <= phaseMaxDepth(phase)) {
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
                    // FOR DEBUGGING PURPOSES ONLY
                    printf("\nPhase %d: depth %d\n", phase, node.depth);
                    printMoves(ms+offset_depth, node.depth);
                    printf(" - Cube (phase):\n");
                    printCube(node.cube);

                    c = applyMoves(c, ms+offset_depth, node.depth);

                    // FOR DEBUGGING PURPOSES ONLY
                    printf(" - Cube (original):\n");
                    printCube(c);

                    offset_depth += node.depth;
                    goto next_phase;
                }

                // add children to stack if max depth is not reached
                if (node.depth < max_depth) {
                    Move moveset[NUM_MOVES];
                    int i = 0, n = phaseMoveset(phase, moveset);
                    while(i < n) {
                        Move m = moveset[i++];
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

int main() {
    // print stats
    printf("Cube size:    %u bytes\n", sizeof(Cube));
    printf("Node size:    %u bytes\n", sizeof(Node));
    printf("Stack size: %u bytes \n", STACK_SIZE * sizeof(Node));

    // initialize
    Cube c = cubeFactory();
    Move ms[MAX_MOVES] = { U, R|I, D|H, L, F, U|I, B, F|H, U };
    int n = 8; // breaks if set to 9

    // scramble
    c = applyMoves(c, ms, n);
    
    // solve
    Move solved_ms[MAX_MOVES];
    int solved_n = solve(c, solved_ms);

    // display details
    printf("\n-- INITIAL STATE --\n");
    printCube(c);
    printf("\n-- SCRAMBLE MOVES --\n");
    printf("[%u]: ", n);
    printMoves(ms, n);
    printf("\n-- SOLVE MOVES --\n");
    printf("[%u]: ", solved_n);
    printMoves(solved_ms, solved_n);

    return 0;
}