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
        { U, U|I,      D, D|I,      R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
        { U, U|I,      D, D|I,      R, R|I,      L, L|I,      F, F|I, F|H, B, B|I, B|H },
        { U, U|I,      D, D|I,      R, R|I,      L, L|I,      F, F|I,      B, B|I      } };

    if (1 <= phase && phase <= 4) {
        ms = phase_moveset[phase-1];

        int n;
        for (n=0 ; ms[n] != NOP ; n++);
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
    Node ns[STACK_SIZE] = {};
    Stack s = { ns, 0 };

    // iterate through phases
    int phase = 0, offset_depth = 0;
    next_phase: while (++phase <= 4) {
        // create goal cube and root node
        Cube goal_cube = phaseCube(phase, cubeFactory());
        Cube root_cube = phaseCube(phase, c);
        Node root_node = { NOP, root_cube, 0 };

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
                    int ms_idx = offset_depth + node.depth-1;
                    *(ms+ms_idx) = node.move;
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
                        Node next_node = { m, next_cube, node.depth+1 };
                        push(&s, next_node);
                    }
                }
            }
        }
        return 0;
    }
    return offset_depth;
}

int main() {
    printf("%d", phaseMoveset(3, NULL));
    return 0;
}