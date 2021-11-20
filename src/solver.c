#include <stddef.h>
#include <stdio.h>
#include "solver.h"

/* DATA STRUCTURE CONSTANTS */

#define QUEUE_SIZE 1000000
#define MAP_SIZE 100

/* TYPE DEFINITIONS */

typedef long Hash;

typedef struct Node {
    Hash prev_hash;         // pointer to previous node
    Move prev_move;         // move applied to previous state
    Cube cube;              // cube state
    Hash hash;              // hash value of cube and phase
    int dir;                // search direction for bi-directional BFS
} Node;

typedef struct {
    Hash *ks;
    Node **vs;
    int size;
} NodeMap;

typedef struct {
    Hash *hs;
    int front;
    int rear;
} HashQueue;

/* DATA STRUCTURE FUNCTION IMPLEMENTATION */

static int mapIndex(NodeMap *m, Hash k) {
    int idx = k % MAP_SIZE;
    for ( ; m->vs[idx] != NULL && m->vs[idx]->hash != k ; idx = (idx+1) % MAP_SIZE);
    return idx;
}

static int contains(NodeMap *m, Hash k) {
    return (m->vs[mapIndex(m, k)] != NULL);
}

static Node *lookup(NodeMap *m, Hash k) {
    return m->vs[mapIndex(m, k)];
}

static void insert(NodeMap *m, Node *n) {
    m->vs[mapIndex(m, n->hash)] = n;
    m->size++;
}

static int isEmpty(HashQueue *q) {
    return (q->front == q->rear);
}

static Hash dequeue(HashQueue *q) {
    if (q->front < q->rear) {
        return q->hs[q->front++];
    }
    return -1;
}

static void enqueue(HashQueue *q, Hash h) {
    if (q->rear < QUEUE_SIZE) {
        q->hs[q->rear++] = h;
    }
}

/* ALGORITHM FUNCTION IMPLEMENTATION */

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
    return phase;
}

int solve(Cube c, Move *ms, int n) {
    // define init and goal states
    Cube init_cube = c;
    Cube goal_cube = cubeFactory();

    // iterate through phases
    int phase = 0;
    while (++phase <= 4) {
        // create init and goal hashes
        Hash init_hash = hash(init_cube, phase);
        Hash goal_hash = hash(goal_cube, phase);

        // DEBUGGING PURPOSES ONLY
        printf("-- PHASE %d --\n", phase);
        printf("INIT -- hash: %d\n", init_hash);
        printCube(init_cube);
        printf("GOAL -- hash: %d\n", goal_hash);
        printCube(goal_cube);

        if (init_hash == goal_hash) {
            continue;
        }

        // create init and goal nodes
        Node init_node = { 0, NOP, init_cube, init_hash, 1 };
        Node goal_node = { 0, NOP, goal_cube, goal_hash, 2 };

        // initialize BFS queue
        // stores a list of hashes in the order to be explored
        Hash hs[QUEUE_SIZE] = {};
        HashQueue q = { hs, 0, 0 };
        enqueue(&q, init_hash);
        enqueue(&q, goal_hash);

        // initialize BFS map
        // maps hashes to search nodes
        Hash ks[MAP_SIZE] = {};
        Node *vs[MAP_SIZE] = {};
        NodeMap m = { ks, vs, 0 };
        insert(&m, &init_node);
        insert(&m, &goal_node);

        // iterate through BFS queue
        while(!isEmpty(&q)) {
            // dequeue parent node from queue
            Hash prev_hash = dequeue(&q);
            Node *prev_node = lookup(&m, prev_hash);

            // lookup phase moveset
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
                Move mv = moveset[i];
                Cube next_cube = applyMove(prev_node->cube, mv);
                Hash next_hash = hash(next_cube, phase);

                // explore new child state
                if (!contains(&m, next_hash)) {
                    Node next_node = { prev_hash, mv, next_cube, next_hash, prev_node->dir };
                    enqueue(&q, next_hash);
                    insert(&m, &next_node);
                }
                // child state explored from reverse search direction
                else if ((lookup(&m, next_hash))->dir != prev_node->dir) {
                    // TODO: solved this phase
                    // walk along path and:
                    // 1. add moves to ms
                    // 2. apply moves to init_cube
                    // printMove(mv);
                    printf(", hash table size: %d \n", m.size);
                    printCube(next_cube);
                    return 1;
                }

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