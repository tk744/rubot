#include "solver.h"
#include "util.h"
#include <stdio.h> // FOR DEBUGGING

#define P1_TABLE_SIZE 2048
#define P2_TABLE_SIZE 1082565
#define P3_TABLE_SIZE 352800
#define P4_TABLE_SIZE 663552
#define TABLE_SIZE P1_TABLE_SIZE + P2_TABLE_SIZE + P3_TABLE_SIZE + P4_TABLE_SIZE

typedef struct {
    Int8 *data;
    int size;
} Table;

Int8 lookup(Table *t, int index) {
    if (index < t->size) {
        return t->data[index];
    }
    return 0;
}

void insert(Table *t, int index, Int8 value) {
    if (t->size < TABLE_SIZE) {
        t->data[index] = value;
    }
}

static void printCubeEncoding(Cube c) {
    int i;
    printf("EDGES: ");
    for(i=0 ; i<NUM_EDGES ; i++) {
        Int8 cubie = getCubie(c.edges, i);
        printf("%d:%d " , getPermutation(cubie, 1), getOrientation(cubie, 1));
    }
    printf("\nCORNERS: ");
    for(i=0 ; i<NUM_CORNERS ; i++) {
        Int8 cubie = getCubie(c.corners, i);
        printf("%d:%d " , getPermutation(cubie, 0), getOrientation(cubie, 0));
    }
    printf("\n");
}

static int phaseTableSize(int phase) {
    if (phase == 1) { 
        return phaseTableSize(phase-1) + P1_TABLE_SIZE;
    }
    else if (phase == 2) {
        return phaseTableSize(phase-1) + P2_TABLE_SIZE;
    }
    else if (phase == 3) {
        return phaseTableSize(phase-1) + P3_TABLE_SIZE;
    }
    else if (phase == 4) {
        return phaseTableSize(phase-1) + P4_TABLE_SIZE;
    }
    return 0;
}

static int phaseIndex(int phase, Cube c) {
    int index = 0;
    if (phase == 1) {
        int i;
        for (i=0 ; i<NUM_EDGES-1 ; i++) {
            index = (index << 1) | getOrientation(getCubie(c.edges, i), 1);
        }
    }
    // 
    else if (phase == 2) {
        // TODO
    }
    else if (phase == 3) {
        // TODO
    }
    else if (phase == 4) {
        // TODO
    }
    return index + phaseTableSize(phase-1);
}

static int phaseMoveset(int phase, Move *ms) {
    static Move phase_moveset[4][NUM_MOVES] = { 
        { U, U|I, U|H, D, D|I, D|H, R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
        {         U|H,         D|H, R, R|I, R|H, L, L|I, L|H, F, F|I, F|H, B, B|I, B|H },
        {         U|H,         D|H,         R|H,         L|H, F, F|I, F|H, B, B|I, B|H },
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

void generateTable(Table *t) {
    // initialize lookup table
    int i;
    for (i=0 ; i<TABLE_SIZE ; i++) {
        t->data[i] = 0xFF;
    }
    t->size = 0;

    // create DFS stack
    static Node ns[STACK_SIZE] = {};
    static Stack s = { ns, 0 };

    // iterate through phases
    int phase = 0;
    while (++phase <= 4) {
        // create phase root node from goal
        Node root_node = { cubeFactory(), NOP, 0 };
        int index = phaseIndex(phase, root_node.cube);

        // insert root index into table
        insert(t, index, root_node.depth);

        // initialize DFS stack wit root node
        clear(&s);
        push(&s, root_node);

        // iterate until stack is empty
        while(s.size) {
            // pop node from stack
            root_node = pop(&s);

            // iterate through children
            Move moveset[NUM_MOVES];
            int i = 0, n = phaseMoveset(phase, moveset);
            while(i < n) {
                Move m = moveset[i++];

                // skip redundant moves
                if ((m & ~(H|I)) & (root_node.move & ~(H|I))) {
                    continue;
                }

                // create child node
                Cube cube = applyMove(root_node.cube, m);
                Node node = { cube, m, root_node.depth+1 };
                int index = phaseIndex(phase, cube);
                
                // update table and push to stack if new index or better depth
                if (lookup(t, index) == 0xFF || node.depth < lookup(t, index)) {
                    insert(t, index, node.depth);
                    push(&s, node);
                }

            }

        }

        printf("phase %d t->size == phaseTableSize(phase): %d", phase, t->size == phaseTableSize(phase));

    }   

}

int main() {
    Cube c = cubeFactory();
    // c = applyMove(c, R);
    
    Int8 cubie = getCubie(c.edges, UR);
    setOrientation(&cubie, 1, 1);
    setCubie(&c.edges, UR, cubie);
    setCubie(&c.edges, UL, cubie);
    
    printCube(c);
    
    printCubeEncoding(c);
    printf("%d %d", phaseIndex(1, c), phaseIndex(2, c));
    return 0;
}

void saveTable(Table *t);

void loadTable(Table *t);

int solve_ida(Cube c, Move *ms, Table *t) {
    int num_moves = 0;
    int phase = 0;
    while (++phase <= 4) {
        // iterate until phase depth is 0
        while (lookup(t, phaseIndex(phase, c))) {
            // TODO: possibly exclude U/D moves? see how that affects move length
            Move best_move = NOP;
            int best_depth;

            // iterate through moveset to find a depth reduction
            Move moveset[NUM_MOVES];
            int i = 0, n = phaseMoveset(phase, moveset);
            while(i < n) {
                Move m = moveset[i++];
                Cube child_cube = applyMove(c, m);
                int child_depth = lookup(t, phaseIndex(phase, child_cube));

                if (best_move == NOP || child_depth < best_depth) {
                    best_move = m;
                    best_depth = child_depth;
                }
            }

            // apply move
            c = applyMove(c, best_move);
            ms[num_moves++] = best_move;
        }
    }
    return num_moves;
}

// create lookup table
// static Int8 data[TABLE_SIZE];
// static Table t = { data, 0 };
// int a = initTable(&t);