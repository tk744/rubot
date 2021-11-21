#include <stdio.h>
#include "solver.h"
#include "util.h"

/* PHASE DEFINITIONS */

static int phase_maxdepth[4] = { 7, 13, 15, 17 };

static Move phase_moveset[4][NUM_MOVES] = { 
    { U, U|I, U|H, D, D|I, D|H, R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
    { U, U|I, D, D|I, R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
    { U, U|I, D, D|I, R, R|I, L, L|I, F, F|I, F|H, B, B|I, B|H },
    { U, U|I, D, D|I, R, R|I, L, L|I, F, F|I, B, B|I } };

/* FUNCTION IMPLEMENTATION */

static int phaseMaxDepth(int phase) {
    return (1 <= phase && phase <= 4) ? phase_maxdepth[phase-1] : 0;
}

static int phaseMoveset(int phase, Move *ms) {
    if (1 <= phase && phase <= 4) {
        ms = phase_moveset[phase-1];

        int i;
        for (i=0 ; ms[i] != NOP ; i++);
        return i;
    }
    return 0;
}

static Cube phaseCube(int phase, Cube c) {
    switch (phase) {
        case 1: // edge orientations
            c.corners = 0;
            break;
        case 2: // corner orientations, E-slice edges

            break;
        case 3: // M/S-slice edges, corner tetrad, even parity

            break;
        case 4: // entire state
            break;
    }
    return c;
}

int solve(Cube c, Move *ms, int n) {
    // iterate through phases
    int phase = 0;
    while (++phase <= 4) {
        // create root and goal cubes
        Cube root_cube = phaseCube(phase, c);
        Cube goal_cube = phaseCube(phase, cubeFactory());

        // create IDDFS stack
        Node *ns[STACK_SIZE] = {};
        Stack s = { ns, 0 };

        // iterative deepening
        int max_depth = 0;
        while (++max_depth <= phaseMaxDepth(phase)) {
            // create root node
            Node root_node = { NULL, NOP, root_cube, 0 };

            // initialize IDDFS stack
            empty(&s);
            push(&s, &root_node);

            // iterate through stack
            while (s.size > 0) {
                // get frontier node
                Node *prev_node = pop(&s);

                // iterate through moveset
                Move moveset[NUM_MOVES];
                int i = phaseMoveset(phase, moveset);
                while(--i >= 0) {
                    // create child node
                    Move m = moveset[i];
                    Cube next_cube = applyMove(prev_node->cube, m);
                    next_cube = phaseCube(phase, next_cube);

                    if (areEqual(next_cube, goal_cube)) {
                        // TODO: phase solved
                        // 1. reconstruct moves
                        // 2. apply moves to c
                        // 3. store moves in ms
                        // 4. increment n
                    }
                    else if (prev_node->depth+1 < max_depth) {
                        Node next_node = { prev_node, m, next_cube, prev_node->depth+1 };
                        push(&s, &next_node);
                    }
                }
            }
        }

    }
}

int main() {
    printf("%d", phaseMoveset(4, NULL));
}

// int bibfs_solve(Cube c, Move *ms, int n) {
//     // define init and goal states
//     Cube init_cube = c;
//     Cube goal_cube = cubeFactory();

//     // iterate through phases
//     int phase = 0;
//     while (++phase <= 4) {
//         // create init and goal hashes
//         Cube init_cube = phaseCube(phase, init_cube);
//         Cube goal_cube = phaseCube(phase, goal_cube);

//         // DEBUGGING PURPOSES ONLY
//         printf("-- PHASE %d --\n", phase);
//         printf("INIT -- hash: %d\n", init_cube);
//         printCube(init_cube);
//         printf("GOAL -- hash: %d\n", goal_cube);
//         printCube(goal_cube);

//         if (areEqual(init_cube, goal_cube)) {
//             continue;
//         }

//         // create init and goal nodes
//         Node init_node = { NULL, NOP, init_cube /*, 1 */ };
//         Node goal_node = { NULL, NOP, goal_cube /*, 2 */ };

//         // initialize BFS queue
//         Cube cs[QUEUE_SIZE] = {};
//         Queue q = { cs, 0, 0 };
//         enqueue(&q, init_cube);
//         enqueue(&q, goal_cube);

//         // initialize BFS lookup table
//         Node *ns[TABLE_SIZE] = {};
//         Table t = { ns, 0 };
//         insert(&t, &init_node);
//         insert(&t, &goal_node);

//         // iterate through BFS queue
//         while(!isEmpty(&q)) {
//             // get new frontier node
//             Cube prev_cube = dequeue(&q);
//             Node *prev_node = lookup(&t, prev_cube);

//             // get phase moveset
//             Move moveset[NUM_MOVES];
//             int i = phaseMoveset(moveset, phase);

//             // DEBUGGING PURPOSES ONLY
//             printf("MOVESET[%d]: ", i);
//             int j = i;
//             while (j-- > 0) {
//                 // printMove(moveset[j]);
//                 printf(j == 0 ? "\n" : ", ");
//             }

//             // iterate through phase moveset
//             while(i-- > 0) {
//                 // lookup child state by applying move to parent state
//                 Move m = moveset[i];
//                 Cube next_cube = applyMove(prev_cube, m);
//                 next_cube = phaseCube(phase, next_cube);

//                 // explore new child state
//                 if (lookup(&t, next_cube) == NULL) {
//                     Node next_node = { prev_node, m, next_cube };
//                     enqueue(&q, next_cube);
//                     insert(&t, &next_node);
//                 }
//                 // // child state explored from reverse search direction
//                 // else if ((lookup(&m, next_hash))->dir != prev_node->dir) {
//                 //     // TODO: solved this phase
//                 //     // walk along path and:
//                 //     // 1. apply moves to c
//                 //     // 2. add moves to ms
//                 //     // printMove(m);
//                 //     printf(", hash table size: %d \n", m.size);
//                 //     printCube(next_cube);
//                 //     return 1;
//                 // }

//             }            

//         }

//     }

//     return 0;
// }