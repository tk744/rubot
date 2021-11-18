#include <stddef.h>
#include <stdio.h>
#include "solver.h"

#define NUM_MOVES 18

#define QUEUE_SIZE 10000
#define HASHTABLE_SIZE 10000

// typedef struct TreeNode {
//     struct TreeNode *parent;
//     Cube c;
//     Move m;
//     int depth;
//     int heuristic;
// } TreeNode;

typedef struct {
    Cube *cs;
    int head;
} CubeQueue;

static int isEmpty(CubeQueue *cq) {
    return (cq->head <= 0);
}

static Cube pop(CubeQueue *cq) {
    if (!isEmpty(cq)) {
        return cq->cs[--cq->head];
    }
}

static int push(CubeQueue *cq, Cube c) {
    if (cq->head < QUEUE_SIZE) {
        cq->cs[cq->head++] = c;
        return 1;
    }
    return 0;
}

static int hash(Cube c, int phase) {
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

typedef struct {
    void *ps;
} Map;

static int insert(Map m, int k, Cube c, void * p) {

}

static void *get(Map m, int k, Cube c) {
    
}

int main() {
    int moves[QUEUE_SIZE];
    CubeQueue q = { moves, 0 };
    // printf("%d ",push(&q, 4));
    // printf("%d ",push(&q, 6));
    printf("%d ",pop(&q));
    printf("%d ",pop(&q));
    printf("%d ",pop(&q));
}

static int getMoveset(Move *ms, int phase) {
    if (phase == 1) {       // g0 group
        Move p1_moveset[] = 
            { M_U, M_UI, M_U2, 
              M_D, M_DI, M_D2, 
              M_R, M_RI, M_R2, 
              M_L, M_LI, M_L2, 
              M_F, M_FI, M_F2, 
              M_B, M_BI, M_B2 };
        ms = p1_moveset;
        return sizeof(p1_moveset) / sizeof(ms[0]);
    }
    else if (phase == 2) {  // g1 group
        Move p2_moveset[] = 
            { M_U2, 
              M_D2, 
              M_R, M_RI, M_R2, 
              M_L, M_LI, M_L2, 
              M_F, M_FI, M_F2, 
              M_B, M_BI, M_B2 };
        ms = p2_moveset;
        return sizeof(p2_moveset) / sizeof(ms[0]);
    }
    else if (phase == 3) {  // g2 group
        Move p3_moveset[] = 
            { M_U2, 
              M_D2, 
              M_R2, 
              M_L2, 
              M_F, M_FI, M_F2, 
              M_B, M_BI, M_B2 };
        ms = p3_moveset;
        return sizeof(p3_moveset) / sizeof(ms[0]);
    }
    else if (phase == 4) {  // g3 group
        Move p4_moveset[] = 
            { M_U2, 
              M_D2, 
              M_R2, 
              M_L2, 
              M_F2, 
              M_B2 };
        ms = p4_moveset;
        return sizeof(p4_moveset) / sizeof(ms[0]);
    }
}

int solve(Cube c, Move *ms, int n) {
    // define goal state
    Cube goal = solvedCubeFactory();

    // BFS queue
    Cube cs[QUEUE_SIZE] = { c, goal };
    CubeQueue q = { cs, 2 };

    // // bi-directional BFS hash tables
    // Map parents = {}
    // Map prev_move = {}
    // Map direction = {}
    // insert(&direction, c, 0);
    // insert(&direction, goal, 1);

    int phase = 0;
    while (++phase <= 4) {
        if (hash(c, phase) == hash(goal, phase)) {
            continue;
        }

        while(!isEmpty(&q)) {
            // pop current state from queue
            Cube old_c = pop(&q);

            // get phase move set
            Move moveset[NUM_MOVES];
            int n = getMoveset(moveset, phase);

            // iterate through phase move set
            while(n-- > 0) {
                // apply move to current state
                Move m = moveset[n];
                Cube new_c = applyMove(old_c, m);

                // // explore new state
                // if (!contains(parents, new_c)) {
                //     push(&q, &new_c);
                //     insert(&parents, &new_c, old_c);
                //     insert(&prev_move, &new_c, &m);
                //     insert(&direction, &new_c, get(&direction, old_c));
                // }
                // // new state already explored from opposite search direction
                // else if (get(&direction, &new_c) != get(&direction, old_c)) {
                //     // TODO: solved!
                // }
            }
        }

    }

    return 0;
}
