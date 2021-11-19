#include <stddef.h>
#include <stdio.h>
#include "solver.h"

#define NUM_MOVES 18

/* DATA STRUCTURE CONSTANTS */

#define QUEUE_SIZE 1000000
#define MAP_SIZE 1000000

/* TYPE DEFINITIONS */

typedef uint32_t Hash;

typedef struct Node {
    struct Node *parent;    // pointer to parent node
    Move move;              // move applied to parent state
    Cube cube;              // cube state
    Hash hash;              // hash value of cube and phase
    int dir;                // search direction for bi-directional BFS
} Node;

typedef struct {
    Node *ns;
    int head;
} NodeQueue;

typedef struct {
    Node *ns;
} NodeMap;

/* DATA STRUCTURE FUNCTION IMPLEMENTATION */

static NodeQueue queueFactory() {
    Node ns[QUEUE_SIZE];
    NodeQueue q = { ns, 0 };
    return q;
}

static int isEmpty(NodeQueue *q) {
    return (q->head <= 0);
}

static Node dequeue(NodeQueue *q) {
    if (!isEmpty(q)) {
        return q->ns[--q->head];
    }
}

static int enqueue(NodeQueue *q, Node n) {
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
    // TODO: collision handling
    return 0;
}

static Node get(NodeMap *m, Hash k) {
    // TODO: collision handling
    return m->ns[k];
}

static int insert(NodeMap *m, Node n) {
    // TODO: collision handling
    m->ns[n.hash] = n;
    return 1;
}

/* ALGORITHM FUNCTION IMPLEMENTATION */

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
}

int solve(Cube c, Move *ms, int n) {
    // define init and goal states
    Cube init_cube = c;
    Cube goal_cube = solvedCubeFactory();

    // iterate through phases
    int phase = 0;
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

        // initialize BFS hash table
        // to check in constant time if a node has already been reached
        NodeMap m = mapFactory();
        insert(&m, init_node);
        insert(&m, goal_node);

        // initialize BFS queue
        NodeQueue q = queueFactory();
        enqueue(&q, init_node);
        enqueue(&q, goal_node);

        // iterate through BFS queue
        while(!isEmpty(&q)) {
            // dequeue parent node from queue
            Node prev_node = dequeue(&q);

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
                    Node next_node = { &prev_node, mv, next_cube, next_hash, prev_node.dir };
                    insert(&m, next_node);
                    enqueue(&q, next_node);
                }
                // child state explored from reverse search direction
                else if ((get(&m, next_hash)).dir != prev_node.dir) {
                    // TODO: solved this phase
                    return 1;
                }

            }            

        }

    }

    return 0;
}

int main() {
    printf("compiled without errors");
}