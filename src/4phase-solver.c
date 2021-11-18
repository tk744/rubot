#include "solver.h"

#define NUM_MOVES 18

typedef struct TreeNode {
    
} Queue;

static Cube push(Queue q, Cube c) {

}

static Cube pop(Queue q) {

}

static int isEmpty(Queue q) {

}

static int getId(Cube c, int phase) {
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
}

static int getMoveSet(Move *ms, int phase) {
    if (phase == 1) {       // g0 group
        static Move p1_moveset[] = {};
        ms = p1_moveset;
    }
    else if (phase == 2) {  // g1 group
        static Move p2_moveset[] = {};
        ms = p2_moveset;
    }
    else if (phase == 3) {  // g2 group
        static Move p3_moveset[] = {};
        ms = p3_moveset;
    }
    else if (phase == 4) {  // g3 group
        static Move p4_moveset[] = {};
        ms = p4_moveset;
    }
    return sizeof(ms) / sizeof(ms[0]);
}

int solve(Cube c, Move *ms, int n) {

    Cube goal = solvedCubeFactory();
    int phase = 0;

    while (++phase < 5) {
        // get current state and goal state ids
        int c_id = getId(c, phase);
        int goal_id = getId(goal, phase);
        if (c_id == goal_id) {
            continue;
        }

        // initialize search (BFS or IDA*) queue
        Queue q = {};
        push(q, c);

        // initialize search tables


        // search
        while(!isEmpty(q)) {
            // pop cube from search queue
            Cube old_c = pop(q);
            
            // get phase moveset
            Move moveset[NUM_MOVES];
            int n = getMoveSet(moveset, phase);

            // 
            while(n-- > 0) {
                // apply all moves in moveset to cube
                Move m = moveset[n];
                c = applyMove(old_c, m);
                c_id = getId(c, phase);

                // found goal
                if (c_id == goal_id) {

                }
                // visit new state
                else {
                    push(q, c);
                }

            }

        }



    }

    return 0;
}



int phase = 0;

