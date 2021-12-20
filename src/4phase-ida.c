#include "solver.h"
#include "util.h"
#include <stdio.h> // FOR DEBUGGING

#define P1_TABLE_SIZE 2048      // 2^11
#define P2_TABLE_SIZE 1082565   // 3^7 * 12C4
#define P3_TABLE_SIZE 352800/2    // 8C4 * (8C2 * 6C2 * 4C2) 
#define P4_TABLE_SIZE 663552    // 
#define TABLE_SIZE P1_TABLE_SIZE + P2_TABLE_SIZE + P3_TABLE_SIZE + P4_TABLE_SIZE

#define EMPTY 0xFF

typedef struct {
    Int8 *data;
    int size;
} Table;

static void initTable(Table *t) {
    int i;
    for (i=0 ; i<TABLE_SIZE ; i++) {
        t->data[i] = EMPTY;
    }
    t->size = 0;
}

static Int8 lookup(Table *t, int index) {
    if (0 <= index && index < TABLE_SIZE) {
        return t->data[index];
    }
    return EMPTY;
}

static void insert(Table *t, int index, Int8 value) {
    if (0 <= index && index < TABLE_SIZE && t->size < TABLE_SIZE) {
        t->size += (t->data[index] == EMPTY);
        t->data[index] = value;
    }
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
    int rank, i;
    for (rank=binomial(n, k), i=0 ; i<k ; rank -= binomial(n-(x[i]+1), k-i), i++);
    return rank-1;
}

static int combinationPairRank(int n, Int8 *x) {
    int rank = 0, pair_rank, i, j;
    Int8 exclusions[n-2];
    
    for (i=n ; i>2 ; i-=2) {
        // set variable base
        for (pair_rank=1, j=i; j>2 ; pair_rank*=binomial(j-2, 2), j-=2);
        
        // get pair
        Int8 pair[2] = { x[n-i], x[n-i+1] };

        // update exclusions
        exclusions[n-i] = pair[0];
        exclusions[n-i+1] = pair[1];

        // reduce combination values if greater than excluded values
        Int8 pair_reduced[2] = { pair[0], pair[1] };
        for (j=0; j<n-i ; j++) {
            pair_reduced[0] -= exclusions[j] < pair[0] ? 1 : 0;
            pair_reduced[1] -= exclusions[j] < pair[1] ? 1 : 0;
        }

        // compute pair rank and updated total rank
        pair_rank *= combinationRank(i, 2, pair_reduced);
        rank += pair_rank;
    }
    return rank;
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

static int phaseTableIndex(int phase, Cube c) {
    int i, index = 0;
    // rank by edge orientation
    if (phase == 1) {
        // rank by edge orientation
        int edge_rank = 0;
        for (i=0 ; i<NUM_EDGES-1 ; i++) {
            edge_rank = (edge_rank << 1) | getOrientation(getCubie(c.edges, i), 1);
        }
        index = edge_rank;
    }
    // rank by corner orientation and S-slice edge permutations
    else if (phase == 2) {
        // rank by corner orientation
        int corner_rank = 0;
        for (i=0 ; i<NUM_CORNERS-1 ; i++) {
            corner_rank *= 3;
            corner_rank += getOrientation(getCubie(c.corners, i), 0);
        }

        // get S-slice edge permutations in ascending order
        Int8 slice_edges[4];
        int idx = 0;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == UR || p == UL || p == DR || p == DL) {
                slice_edges[idx++] = i;
            }
        }
        // map S-slice edge permutations to linear rank
        int slice_rank = combinationRank(12, 4, slice_edges);

        // TODO: comment
        index = slice_rank * 2187 + corner_rank;
    }
    // rank by corner tetrad pairs, M-slice edge permutations, and parity
    // note: this fixes all edge slices since E-slice edges fall into place
    else if (phase == 3) {
        // TODO: comment
        Int8 pair_corners[8];
        Int8 idx0=0, idx1=2, idx2=4, idx3=6;
        for (i=0 ; i<NUM_CORNERS ; i++) {
            Int8 cubie = getCubie(c.corners, i);
            Int8 p = getPermutation(cubie, 0);
            if (p == UFR || p == UBL) {
                pair_corners[idx0++] = i;
            }
            else if (p == UFL || p == UBR) {
                pair_corners[idx1++] = i;
            }
            else if (p == DFR || p == DBL) {
                pair_corners[idx2++] = i;
            }
            else if (p == DFL || p == DBR) {
                pair_corners[idx3++] = i;
            }
        }
        // TODO: comment
        int tetrad_rank = combinationPairRank(8, pair_corners);

        // get M-slice edge permutations in ascending order
        Int8 slice_edges[4];
        int idx = 0;
        for (i=0 ; i<NUM_EDGES ; i++) {
            // ignore S-slice edges
            if (i == UR || i == UL || i == DR || i == DL) {
                continue;
            }
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == UF || p == UB || p == DF || p == DB) {
                // map M/E-slice edges to 0-7 for combination indexing
                if (i == UF) {
                    slice_edges[idx++] = 0;
                }
                else if (i == UB) {
                    slice_edges[idx++] = 1;
                }
                else if (i == DF) {
                    slice_edges[idx++] = 2;
                }
                else if (i == DB) {
                    slice_edges[idx++] = 3;
                }
                else if (i == FR) {
                    slice_edges[idx++] = 4;
                }
                else if (i == FL) {
                    slice_edges[idx++] = 5;
                }
                else if (i == BR) {
                    slice_edges[idx++] = 6;
                }
                else if (i == BL) {
                    slice_edges[idx++] = 7;
                }
            }
        }
        // map M-slice edge permutations to linear rank
        int slice_rank = combinationRank(8, 4, slice_edges);

        // TODO: parity

        index = slice_rank * 2520 + tetrad_rank;
    }
    // rank by edge slice permutations and corner tetrad permutations
    else if (phase == 4) {
        // TODO
    }
    return index + phaseTableSize(phase-1);
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
        int index = phaseTableIndex(phase, root_node.cube);

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
                int index = phaseTableIndex(phase, cube);

                // update table and push to stack if new index or better depth
                if (lookup(t, index) == EMPTY || node.depth < lookup(t, index)) {
                    // printf("Updating index %d from %d to %d\n", index, lookup(t, index), node.depth);
                    insert(t, index, node.depth);
                    push(&s, node);
                }

            }

            // printf("table size: %d, stack size: %d\n", t->size, s.size);

        }

        printf("phase %d | table size %7d | correct size? %d | max depth %d\n", phase, t->size, t->size == phaseTableSize(phase), phaseMaxDepth(phase));
        // assert t->size == phaseTableSize(phase)
    }

}

void saveTable(Table *t);

void loadTable(Table *t);

int solve_a(Cube c, Move *ms, Table *t) {
    int num_moves = 0;
    int phase = 0;
    while (++phase <= 4) {
        // iterate until phase depth is 0
        while (lookup(t, phaseTableIndex(phase, c))) {
            Move best_move = NOP;
            int best_depth;

            // iterate through moveset to find a depth reduction
            Move moveset[NUM_MOVES];
            int i = 0, n = phaseMoveset(phase, moveset);
            while(i < n) {
                Move m = moveset[i++];
                Cube child_cube = applyMove(c, m);
                int child_depth = lookup(t, phaseTableIndex(phase, child_cube));

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
    c = applyMove(c, R);
    c = applyMove(c, F);

    printCube(c);
    printCubeEncoding(c);

    // Int8 pairs[8] = { 0, 1, 2, 3, 4, 5 };
    // printf("%d", combinationPairRank(8, pairs));

    Int8 data[TABLE_SIZE];
    Table t = { data, 0 };
    generateTable(&t);
    
    printf("\nTable size: %d \n", t.size);
    printf("Cube index: %d \n", phaseTableIndex(3, c));
    printf("Cube depth: %d \n", lookup(&t, phaseTableIndex(1, c)));

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