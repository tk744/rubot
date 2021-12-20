#include "solver.h"
#include "util.h"
#include <stdio.h> // FOR DEBUGGING

#define P1_TABLE_SIZE 2048
#define P2_TABLE_SIZE 1082565
#define P3_TABLE_SIZE 352800
#define P4_TABLE_SIZE 663552
#define TABLE_SIZE P1_TABLE_SIZE + P2_TABLE_SIZE + P3_TABLE_SIZE + P4_TABLE_SIZE

#define EMPTY 0xFF

typedef struct {
    Int8 *data;
    int size;
} Table;

void initTable(Table *t) {
    int i;
    for (i=0 ; i<TABLE_SIZE ; i++) {
        t->data[i] = EMPTY;
    }
    t->size = 0;
}

Int8 lookup(Table *t, int index) {
    if (0 <= index && index < TABLE_SIZE) {
        return t->data[index];
    }
    return EMPTY;
}

void insert(Table *t, int index, Int8 value) {
    if (0 <= index && index < TABLE_SIZE && t->size < TABLE_SIZE) {
        t->size += (t->data[index] == EMPTY);
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

static int phaseMaxDepth(int phase) {
    static int phase_maxdepth[4] = { 7, 10, 14, 15 };
    return (1 <= phase && phase <= 4) ? phase_maxdepth[phase-1] : 0;
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

static int binomial(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return 0;
    }
    int b, i;
    for (b=1, i=0; i<k ; b*=((float)n-i)/(i+1), i++);
    return b;
}

static int combinationRank(int n, int k, Int8 *x) {
    int rank = binomial(n, k);
    int i;
    for (i=0 ; i<k ; i++) {
        rank -= binomial(n-(x[i]+1), k-i);
    }
    return rank-1;
}

static int phaseIndex(int phase, Cube c) {
    int i, index = 0;
    // rank by edge orientation
    if (phase == 1) {
        //
        for (i=0 ; i<NUM_EDGES-1 ; i++) {
            index = (index << 1) | getOrientation(getCubie(c.edges, i), 1);
        }
    }
    // rank by corner orientation and middle-slice edge permutation
    else if (phase == 2) {
        // 
        for (i=0 ; i<NUM_CORNERS-1 ; i++) {
            index *= 3;
            index += getOrientation(getCubie(c.corners, i), 0);
        }

        // get middle-slice edge permutations in ascending order
        Int8 slice_edges[4];
        int slice_idx = 0;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == UR || p == UL || p == DR || p == DL) {
                slice_edges[slice_idx++] = i;
            }
        }
        // map middle-slice edge permutations to linear rank
        int slice_rank = combinationRank(12, 4, slice_edges);

        // 
        index += slice_rank * 2187;
    }
    // rank by 
    else if (phase == 3) {
        // TODO
    }
    // rank by 
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
    initTable(t);

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

            // discard nodes that are too deep
            if (root_node.depth >= phaseMaxDepth(phase)) {
                continue;
            }

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
                if (lookup(t, index) == EMPTY || node.depth < lookup(t, index)) {
                    // printf("Updating index %d from %d to %d\n", index, lookup(t, index), node.depth);
                    insert(t, index, node.depth);
                    push(&s, node);
                }

            }

            // printf("table size: %d, stack size: %d\n", t->size, s.size);

        }

        printf("phase %d | table size %d \t| correct size? %d | max depth %d\n", phase, t->size, t->size == phaseTableSize(phase), phaseMaxDepth(phase));

    }

}

void saveTable(Table *t);

void loadTable(Table *t);

int solve_a(Cube c, Move *ms, Table *t) {
    int num_moves = 0;
    int phase = 0;
    while (++phase <= 4) {
        // iterate until phase depth is 0
        while (lookup(t, phaseIndex(phase, c))) {
            Move best_move = NOP;
            int best_depth;

            // iterate through moveset to find a depth reduction
            Move moveset[NUM_MOVES];
            int i = 0, n = phaseMoveset(phase, moveset);
            while(i < n) {
                Move m = moveset[i++];
                Cube child_cube = applyMove(c, m);
                int child_depth = lookup(t, phaseIndex(phase, child_cube));

                // printf("best d: %d child d: %d\n", best_depth, child_depth);

                if (best_move == NOP || child_depth < best_depth) {
                    best_move = m;
                    best_depth = child_depth;
                }

                // TODO: EXPERIMENTAL OPTIMIZATION 1
                // if best_depth > current_depth: break
                // TODO: EXPERIMENTAL OPTIMIZATION 2
                // possibly exclude U/D moves? see how that affects move length
            }

            // apply move
            c = applyMove(c, best_move);
            ms[num_moves++] = best_move;
        }
    }
    return num_moves;
}

int main() {
    Cube c = cubeFactory();
    c = applyMove(c, U);

    printCube(c);
    printCubeEncoding(c);

    Int8 data[TABLE_SIZE];
    Table t = { data, 0 };
    generateTable(&t);
    
    printf("\nTable size: %d \n", t.size);
    printf("Cube index: %d \n", phaseIndex(2, c));
    printf("Cube depth: %d \n", lookup(&t, phaseIndex(1, c)));

    printf("\nSolve:\n");
    Move ms[MAX_MOVES];
    int n = solve_a(c, ms, &t);
    printMoves(ms, n);
    
    return 0;
}

// create lookup table
// static Int8 data[TABLE_SIZE];
// static Table t = { data, 0 };
// int a = initTable(&t);