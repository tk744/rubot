#include <stdio.h>
#include "solver.h"
#include "util.h"

/*
TODO:
1. phaseCube()
*/

static int phaseMaxDepth(int phase) {
    static int phase_maxdepth[4] = { 7, 13, 15, 17 };
    return (1 <= phase && phase <= 4) ? phase_maxdepth[phase-1] : 0;
}

static int phaseMoveset(int phase, Move *ms) {
    static Move phase_moveset[4][NUM_MOVES] = { 
        { U, U|I, U|H, D, D|I, D|H, R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
        { U, U|I, U|H, D, D|I, D|H, R, R|I, R|H, L, L|I, L|H, F, F|I,      B, B|I,     },
        { U, U|I, U|H, D, D|I, D|H, R, R|I,      L, L|I,      F, F|I,      B, B|I,     },
        { U, U|I,      D, D|I,      R, R|I,      L, L|I,      F, F|I,      B, B|I      } };

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
    switch (phase) {
        case 1: // edge orientations
            c.corners = 0;
            break;
        case 2: // corner orientations, E-slice edges
            c.edges = 0;
            break;
        case 3: // M/S-slice edges, corner tetrad, even parity

            break;
        case 4: // entire state
            break;
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
                    c = applyMoves(c, ms+offset_depth, node.depth);
                    offset_depth += node.depth;
                    goto next_phase;
                }

                // add children to stack if max depth is not reached
                if (node.depth < max_depth) {
                    Move moveset[NUM_MOVES];
                    int i = 0, n = phaseMoveset(phase, moveset);
                    while(i++ < n) {
                        Move m = moveset[i];
                        Cube next_cube = applyMove(node.cube, m);
                        next_cube = phaseCube(phase, next_cube);
                        Node next_node = { next_cube, m, node.depth+1 };
                        push(&s, next_node);
                    }
                }
            }
        }
        return 0;
    }
    return offset_depth;
}

// int main() {
//     printf("Cube size:    %u bytes\n", sizeof(Cube));
//     printf("Node size:    %u bytes\n", sizeof(Node));
//     printf("Stack size: %u bytes \n", STACK_SIZE * sizeof(Node));

//     Cube c0 = cubeFactory();        // -
//     Cube c1 = applyMove(c0, U|I);   // R R R
//     Cube c2 = applyMove(c1, U|I);   // R R
//     Cube c3 = applyMove(c2, U|H);   // -

//     printCube(c0);
//     printCube(c1);
//     printCube(c2);
//     printCube(c3);

//     return(0);
// }

int main() {
    Cube c = cubeFactory();
    c = applyMove(c, R|I);

    Move ms[MAX_MOVES] = {};
    int n = solve(c, ms);

    printMoves(ms, n);
}