#include <stdio.h>
#include "solver.h"
#include "util.h"

/* FUNCTION IMPLEMENTATION */

static Cube compress(Cube c, int phase) {
    switch (phase) {
        case 1: // edge orientations
            
            break;
        case 2: // corner orientations, E slice edges

            break;
        case 3: // M/S slice edges, corner tetrads, parity

            break;
        case 4: // entire state
            
            break;
    }
    return c;
}

static int getMoveset(Move *ms, int phase) {
    if (phase == 1) {       // g0 group
        Move p1_moveset[] = 
            { U, U|I, U|H, 
              D, D|I, D|H, 
              R, R|I, R|H, 
              L, L|I, L|H, 
              F, F|I, F|H, 
              B, B|I, B|H };
        ms = p1_moveset;
        return sizeof(p1_moveset) / sizeof(ms[0]);
    }
    else if (phase == 2) {  // g1 group
        Move p2_moveset[] = 
            { U, U|I, 
              D, D|I, 
              R, R|I, R|H, 
              L, L|I, L|H, 
              F, F|I, F|H, 
              B, B|I, B|H };
        ms = p2_moveset;
        return sizeof(p2_moveset) / sizeof(ms[0]);
    }
    else if (phase == 3) {  // g2 group
        Move p3_moveset[] = 
            { U, U|I, 
              D, D|I, 
              R, R|I, 
              L, L|I, 
              F, F|I, F|H, 
              B, B|I, B|H };
        ms = p3_moveset;
        return sizeof(p3_moveset) / sizeof(ms[0]);
    }
    else if (phase == 4) {  // g3 group
        Move p4_moveset[] = 
            { U, U|I, 
              D, D|I, 
              R, R|I, 
              L, L|I, 
              F, F|I, 
              B, B|I };
        ms = p4_moveset;
        return sizeof(p4_moveset) / sizeof(ms[0]);
    }
    return 0;
}

int solve(Cube c, Move *ms, int n) {
    // define init and goal states
    Cube init_cube = c;
    Cube goal_cube = cubeFactory();

    // iterate through phases
    int phase = 0;
    while (++phase <= 4) {
        // create init and goal hashes
        Cube init_cube = compress(init_cube, phase);
        Cube goal_cube = compress(goal_cube, phase);

        // DEBUGGING PURPOSES ONLY
        printf("-- PHASE %d --\n", phase);
        printf("INIT -- hash: %d\n", init_cube);
        printCube(init_cube);
        printf("GOAL -- hash: %d\n", goal_cube);
        printCube(goal_cube);

        if (areEqual(init_cube, goal_cube)) {
            continue;
        }

        // create init and goal nodes
        Node init_node = { NULL, NOP, init_cube /*, 1 */ };
        Node goal_node = { NULL, NOP, goal_cube /*, 2 */ };

        // initialize BFS queue
        Cube cs[QUEUE_SIZE] = {};
        Queue q = { cs, 0, 0 };
        enqueue(&q, init_cube);
        enqueue(&q, goal_cube);

        // initialize BFS lookup table
        Node *ns[TABLE_SIZE] = {};
        Table t = { ns, 0 };
        insert(&t, &init_node);
        insert(&t, &goal_node);

        // iterate through BFS queue
        while(!isEmpty(&q)) {
            // get new frontier node
            Cube prev_cube = dequeue(&q);
            Node *prev_node = lookup(&t, prev_cube);

            // get phase moveset
            Move moveset[NUM_MOVES];
            int i = getMoveset(moveset, phase);

            // DEBUGGING PURPOSES ONLY
            printf("MOVESET[%d]: ", i);
            int j = i;
            while (j-- > 0) {
                // printMove(moveset[j]);
                printf(j == 0 ? "\n" : ", ");
            }

            // iterate through phase moveset
            while(i-- > 0) {
                // lookup child state by applying move to parent state
                Move m = moveset[i];
                Cube next_cube = applyMove(prev_cube, m);
                next_cube = compress(next_cube, phase);

                // explore new child state
                if (lookup(&t, next_cube) == NULL) {
                    Node next_node = { prev_node, m, next_cube };
                    enqueue(&q, next_cube);
                    insert(&t, &next_node);
                }
                // // child state explored from reverse search direction
                // else if ((lookup(&m, next_hash))->dir != prev_node->dir) {
                //     // TODO: solved this phase
                //     // walk along path and:
                //     // 1. apply moves to c
                //     // 2. add moves to ms
                //     // printMove(m);
                //     printf(", hash table size: %d \n", m.size);
                //     printCube(next_cube);
                //     return 1;
                // }

            }            

        }

    }

    return 0;
}

// int main() {
//     int n = 100;
//     Move ms[n];
//     Cube c = solvedCubeFactory();
//     c = applyMove(c, M_R);
//     printCube(c);

//     int l = solve(c, ms, n);
//     printf("moves: %d\n", l);
// }