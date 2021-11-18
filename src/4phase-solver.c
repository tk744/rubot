#include <stddef.h>
#include <stdio.h>
#include "solver.h"

#define NUM_MOVES 18

#define QUEUE_SIZE 1000000
#define MAP_SIZE 1000000

// typedef struct TreeNode {
//     struct TreeNode *parent;
//     Cube c;
//     Move m;
//     int depth;
//     int heuristic;
// } TreeNode;

// typedef struct {
//     Cube *cs;
//     int head;
// } CubeQueue;

// static int isEmpty(CubeQueue *q) {
//     return (q->head <= 0);
// }

// static Cube pop(CubeQueue *q) {
//     if (!isEmpty(q)) {
//         return q->cs[--q->head];
//     }
// }

// static int push(CubeQueue *q, Cube c) {
//     if (q->head < QUEUE_SIZE) {
//         q->cs[q->head++] = c;
//         return 1;
//     }
//     return 0;
// }

// static CubeQueue queueFactory() {
//     Cube cs[QUEUE_SIZE];
//     CubeQueue q = { cs, 0 };
//     return q;
// }

typedef uint32_t Hash;

static Hash hash(Cube c, int phase) {
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

// typedef struct {
//     Hash *ks;
//     void **vs;
// } HashMap;

// static int contains(HashMap *m, Hash k) {
//     // TODO: collision handling
//     return (m->ks[k] == k);
// }

// static void *get(HashMap *m, Hash k) {
//     // TODO: collision handling
//     return m->vs[k];
// }

// static int insert(HashMap *m, Hash k, void *v) {
//     // TODO: collision handling
//     m->ks[k] = k;
//     m->vs[k] = v;
// }

// static HashMap mapFactory() {
//     Hash ks[MAP_SIZE];
//     void *vs[MAP_SIZE];
//     HashMap m = { ks, vs };
//     return m;
// }

int main() {
    int moves[QUEUE_SIZE];
    // CubeQueue q = { moves, 0 };
    // // printf("%d ",push(&q, 4));
    // // printf("%d ",push(&q, 6));
    // printf("%d ",pop(&q));
    // printf("%d ",pop(&q));
    // printf("%d ",pop(&q));
}

typedef struct Node {
    struct Node *parent;
    Move move;
    Cube cube;
    Hash hash;
    int dir;
} Node;

typedef struct {
    Node *ns;
    int head;
} NodeQueue;

typedef struct {
    Node *ns;
} NodeMap;

// Node nodeFactory(Cube c, Hash h) {
//     Node n = { NULL, NULL, c, h };
//     return n;
// }

static NodeQueue queueFactory() {
    Node ns[QUEUE_SIZE];
    NodeQueue q = { ns, 0 };
    return q;
}

static int isEmpty(NodeQueue *q) {
    return (q->head <= 0);
}

static Node pop(NodeQueue *q) {
    if (!isEmpty(q)) {
        return q->ns[--q->head];
    }
}

static int push(NodeQueue *q, Node n) {
    if (q->head < QUEUE_SIZE) {
        q->ns[q->head++] = n;
        return 1;
    }
    return 0;
}

static NodeMap mapFactory() {
    Node ns[MAP_SIZE];
    NodeMap m = { ns };
    return m;
}

static int contains(NodeMap *m, Hash k) {
    // TODO
    return 0;
}

static Node get(NodeMap *m, Hash k) {
    // TODO
    return (*m).ns[k];
}

static int insert(NodeMap *m, Node n) {
    // TODO: insert according to v.hash;
    return 0;
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
    int phase = 0;

    // define init and goal states
    Cube init_cube = c;
    Cube goal_cube = solvedCubeFactory();

    while (++phase <= 4) {
        // generate init and goal hashes
        Hash init_hash = hash(init_cube, phase);
        Hash goal_hash = hash(goal_cube, phase);
        if (init_hash == goal_hash) {
            continue;
        }

        // construct init and goal nodes
        // TODO: replace M_U with NOP
        Node init_node = { NULL, M_U, init_cube, init_hash, 1 };
        Node goal_node = { NULL, M_U, goal_cube, goal_hash, 2 };

        // initialize bi-directional BFS queue
        NodeQueue q = queueFactory();
        push(&q, init_node);
        push(&q, goal_node);

        // initialize BFS hash table
        NodeMap m = mapFactory();

        while(!isEmpty(&q)) {
            // pop parent node from queue
            Node prev_node = pop(&q);

            // get phase moveset
            Move moveset[NUM_MOVES];
            int n = getMoveset(moveset, phase);

            // iterate through phase moveset
            while(n-- > 0) {
                // get child state by applying move to parent state
                Move mv = moveset[n];
                Cube next_cube = applyMove(prev_node.cube, mv);
                Hash next_hash = hash(next_cube, phase);

                // explore new child state
                if (!contains(&m, next_hash)) {
                    // get child node
                    Node next_node = { &prev_node, mv, next_cube, next_hash, prev_node.dir };
                    push(&q, next_node);
                    insert(&m, next_node);
                }
                // child state explored from reverse search direction
                else if ((get(&m, next_hash)).dir != prev_node.dir) {

                }

            }            

        }

    }

/*
    // bi-directional BFS queues
    CubeQueue fq = queueFactory();  // forward search queue
    CubeQueue bq = queueFactory();  // backward search queue
    push(&fq, c);
    push(&bq, goal);
    int dir = 0;    // search direction: 1 if forward, 0 if backward

    // bi-directional BFS hash tables
    HashMap parents = {};   // maps cube hash to parent cube hash
    HashMap moves = {};     // maps cube hash to parent move
    // HashMap dirs = {};      // maps cube hash to search direction
    // insert(&direction, c, phase, 0);
    // insert(&direction, goal, phase, 1);
*/
    // attempt 2
    NodeQueue fq = queueFactory();  // forward search queue
    NodeQueue bq = queueFactory();  // backward search queue
    int dir = 0;

    // while (++phase <= 4) {
    //     if (hash(c, phase) == hash(goal, phase)) {
    //         continue;
    //     }

    //     while(!isEmpty((dir = 1-dir) ? &fq : &bq)) {
    //         // pop current state from queue
    //         Cube old_c = pop(dir ? &fq : &bq);

    //         // get phase moveset
    //         Move moveset[NUM_MOVES];
    //         int n = getMoveset(moveset, phase);

    //         // iterate through phase moveset
    //         while(n-- > 0) {
    //             // apply move to current state
    //             Move m = moveset[n];
    //             Cube new_c = applyMove(old_c, m);

    //             // // explore new state
    //             // if (!contains(parents, new_c, )) {
    //             //     push(&q, &new_c);
    //             //     insert(&parents, &new_c, old_c);
    //             //     insert(&prev_move, &new_c, &m);
    //             //     insert(&direction, &new_c, get(&direction, old_c));
    //             // }
    //             // // new state already explored from opposite search direction
    //             // else if (get(&direction, &new_c) != get(&direction, old_c)) {
    //             //     // TODO: solved!
    //             // }
    //         }
    //     }

    // }

    return 0;
}
