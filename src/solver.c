#include "rubot.h"
#include <stdio.h>

#define BIN_FILE "4PHASE.bin"
#define NUM_STATES 2100965
/*
Phase 1: 2^11 = 2048
Phase 2: 3^7 * 12C4 = 1082565
Phase 3: 8C4 * (8C2 * 6C2 * 4C2) = 352800
Phase 4: (4! * 4! * 4P2) * (4! * 4P1) = 663552
*/

typedef struct {
    Cube128 cube;
    Move move;
    Int4 depth;
} Node;

typedef struct {
    Node *ns;
    int size;
} Stack;

static int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    else {
        return n * factorial(n-1);
    }
}

static int permutation(int n, int k) {
    return (int)(((float)factorial(n)) / factorial(n-k));
}

static int combination(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return 0;
    }
    int b, i;
    for (b=1, i=0; i<k ; b*=((float)n-i)/(i+1), i++);
    return b;
}

static int binaryOnes(int b) {
    int n = 0;
    while (b > 0) {
        n += b % 2;
        b /= 2;
    }
    return n;
}

static int permutationRank(Int8 *x, int n, int k) {
    int rank = 0, bits = 0, i;
    for (i=0 ; i<n ; i++) {
        bits |= (1 << (n-1-x[i]));
        rank += permutation(n-1-i, k-1-i) * (x[i] - binaryOnes(bits >> (n-x[i])));
    }

    return rank;
}

static int combinationRank(Int8 *x, int n, int k) {
    int rank, i;
    for (rank=combination(n, k), i=0 ; i<k ; rank -= combination(n-(x[i]+1), k-i), i++);
    return rank-1;
}

static int combinationPairRank(Int8 *x, int n) {
    int rank = 0, pair_rank, i, j;
    Int8 exclusions[n-2];
    
    for (i=n ; i>2 ; i-=2) {
        // set variable base
        for (pair_rank=1, j=i; j>2 ; pair_rank*=combination(j-2, 2), j-=2);
        
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
        pair_rank *= combinationRank(pair_reduced, i, 2);
        rank += pair_rank;
    }
    return rank;
}

static int setPhaseMoves(Move *ms, int phase) {
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

static int phaseDepth(int phase) {
    static int phase_depth[4] = { 7, 10, 14, 15 };
    return (1 <= phase && phase <= 4) ? phase_depth[phase-1] : 0;
}

static int phaseSize(int phase) {
    static int phase_size[4] = { 2048, 1082565, 352800, 663552 };
    return (1 <= phase && phase <= 4) ? phase_size[phase-1] : 0;
}

static int cubeRank(Cube128 c, int phase) {
    int i;
    // rank by edge orientation
    if (phase == 1) {
        // rank by edge orientation
        int edge_rank = 0;
        for (i=0 ; i<NUM_EDGES-1 ; i++) {
            edge_rank = (edge_rank << 1) | getOrientation(getCubie(c.edges, i), 1);
        }
        return edge_rank;
    }
    // rank by corner orientation and S-slice edge combinations
    else if (phase == 2) {
        // rank by corner orientation
        int corner_rank = 0;
        for (i=0 ; i<NUM_CORNERS-1 ; i++) {
            corner_rank *= 3;
            corner_rank += getOrientation(getCubie(c.corners, i), 0);
        }

        // get S-slice edge combinations in ascending order
        Int8 slice_edges[4];
        int idx = 0;
        for(i=0 ; i<NUM_EDGES ; i++) {
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == UR || p == UL || p == DR || p == DL) {
                slice_edges[idx++] = i;
            }
        }
        // map S-slice edge combinations to linear rank
        int slice_rank = combinationRank(slice_edges, 12, 4);

        // TODO: comment
        return slice_rank * 2187 + corner_rank;
    }
    // rank by corner tetrad pairs, M-slice edge combinations, and parity
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
        int tetrad_rank = combinationPairRank(pair_corners, 8);

        // get M-slice edge combinations in ascending order
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
                // map M/E-slice edges to 0-7 for ranking
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
        // map M-slice edge combinations to linear rank
        int slice_rank = combinationRank(slice_edges, 8, 4);

        // compute corner parity
        int j, parity = 0;
        for (i=0 ; i<NUM_CORNERS ; i++) {
            for (j=i+1 ; j<NUM_CORNERS ; j++) {
                int p1, p2;
                p1 = getPermutation(getCubie(c.corners, i), 0);
                p2 = getPermutation(getCubie(c.corners, j), 0);
                parity ^= p1 > p2;
            }
        }

        // TODO: comment
        return (slice_rank * 2520 + tetrad_rank) * 2 + parity;
    }
    // rank by edge slice permutations and corner tetrad permutations
    else if (phase == 4) {
        // get all slice edge permutations
        Int8 s_edges[4], m_edges[4], e_edges[2];
        int s_idx = 0, m_idx = 0, e_idx = 0;
        for (i=0 ; i<NUM_EDGES ; i++) {
            if (i == BR || i == BL) {
                continue;
            }

            // map slice edges to 0-3 for ranking
            Int8 cubie = getCubie(c.edges, i);
            Int8 p = getPermutation(cubie, 1);
            if (p == UR) {
                s_edges[s_idx++] = 0;
            }
            else if (p == UL) {
                s_edges[s_idx++] = 1;
            }
            else if (p == DR) {
                s_edges[s_idx++] = 2;
            }
            else if (p == DL) {
                s_edges[s_idx++] = 3;
            }
            else if (p == UF) {
                m_edges[m_idx++] = 0;
            }
            else if (p == UB) {
                m_edges[m_idx++] = 1;
            }
            else if (p == DF) {
                m_edges[m_idx++] = 2;
            }
            else if (p == DB) {
                m_edges[m_idx++] = 3;
            }
            else if (p == FR) {
                e_edges[e_idx++] = 0;
            }
            else if (p == FL) {
                e_edges[e_idx++] = 1;
            }
            else if (p == BR) {
                e_edges[e_idx++] = 2;
            }
            else if (p == BL) {
                e_edges[e_idx++] = 3;
            }
        }

        // map slice edge permutations to linear rank
        int s_rank = permutationRank(s_edges, 4, 4);
        int m_rank = permutationRank(m_edges, 4, 4);
        int e_rank = permutationRank(e_edges, 4, 2);

        // combine edge ranks: 4! * 4P2 = 288; 4P2 = 12
        int edge_rank = s_rank * 288 + m_rank * 12 + e_rank;

        // get one tetrad corner permutations
        Int8 tetrad[4];
        int idx = 0;
        for (i=0 ; i<NUM_CORNERS ; i++) {
            // ignore other tetrad
            if (i == UBR || i == UFL || i == DBL || i == DFR) {
                continue;
            }

            // map tetrad corners to 0-3 for ranking
            Int8 cubie = getCubie(c.corners, i);
            Int8 p = getPermutation(cubie, 0);
            if (p == UBL) {
                tetrad[idx++] = 0;
            }
            else if (p == UFR) {
                tetrad[idx++] = 1;
            }
            else if (p == DFL) {
                tetrad[idx++] = 2;
            }
            else if (p == DBR) {
                tetrad[idx++] = 3;
            }
        }

        // map tetrad corner permutations to linear rank
        int tetrad_rank = permutationRank(tetrad, 4, 4);

        // get rank of one corner from other tetrad
        int ubr_rank;
        Int8 cubie = getCubie(c.corners, UBR);
        Int8 p = getPermutation(cubie, 0);
        if (p == UBR) {
            ubr_rank = 0;
        }
        else if (p == UFL) {
            ubr_rank = 1;
        }
        else if (p == DBL) {
            ubr_rank = 2;
        }
        else if (p == DFR) {
            ubr_rank = 3;
        }

        // combine corner ranks
        int corner_rank = tetrad_rank * 4 + ubr_rank;
        
        // TODO: comment
        return edge_rank * 96 + corner_rank;
    }
    return 0;
}

static int cubeIndex(Cube128 c, int phase) {
    int i, size = 0;
    for (i=1 ; i<phase ; i++) {
        size += phaseSize(i);
    }
    return size + cubeRank(c, phase);
}

static void writeNibble(Int8 *table, int index, Int4 n) {
    Int8 byte = table[index/2];
    byte &= ~(0b1111 << 4 * (index % 2));
    byte |= n << 4 * (index % 2);
    table[index/2] = byte;
}

static Int4 readNibble(const Int8 *table, int index) {
    return table[index/2] >> 4 * (index % 2) & 0b1111;
}

static Int4 indexLT(FILE *lt, int index) {
    Int8 byte;
    fseek(lt, index/2, SEEK_SET);
    fread(&byte, 1, 1, lt);
    return byte >> 4 * (index % 2) & 0b1111;
}

static int buildLT(FILE *lt) {
    // initialize table
    int i, size = (NUM_STATES/2)+(NUM_STATES%2);
    Int8 table[size];
    for (i=0 ; i<size ; i++) {
        table[i] = 0xFF;
    }

    // initialize DFS stack, 15 (max depth) * 18 (max branching factor) = 270
    #define STACK_SIZE 270
    static Node ns[STACK_SIZE] = {};
    static Stack stack = { ns, 0 };

    // compute each phase and insert into table
    int phase = 0;
    while (++phase <= 4) {
        // create phase root node from goal
        Node root_node = { cubeSolved(), NOP, 0 };
        int index = cubeIndex(root_node.cube, phase);

        // insert root index into table
        writeNibble(table, index, root_node.depth);

        // initialize DFS stack with root node
        stack.size = 0;
        stack.ns[stack.size++] = root_node;

        // iterate until stack is empty
        while(stack.size) {
            // pop node from stack
            root_node = stack.ns[--stack.size];

            // discard nodes that are too deep
            if (root_node.depth >= phaseDepth(phase)) {
                continue;
            }

            // iterate through children
            Move moveset[NUM_MOVES];
            int i = 0, n = setPhaseMoves(moveset, phase);
            while(i < n) {
                Move m = moveset[i++];

                // skip redundant moves
                if ((m & ~(H|I)) & (root_node.move & ~(H|I))) {
                    continue;
                }

                // create child node
                Cube128 cube = applyMove(root_node.cube, m);
                Node node = { cube, m, root_node.depth+1 };
                int index = cubeIndex(cube, phase);

                // update table and push node to stack if shorter depth
                if (node.depth < readNibble(table, index)) {
                    writeNibble(table, index, node.depth);
                    stack.ns[stack.size++] = node;
                }
            }
        }
    }

    // write table to file
    return fwrite(table, 1, size, lt);
}

static FILE *openLT() {
    FILE *lt = fopen(BIN_FILE, "rb");
    if (!lt) {
        lt = fopen(BIN_FILE, "w+b");
        buildLT(lt);
        fclose(lt);
        lt = fopen(BIN_FILE, "rb");
    }
    return lt;
}

int solve(Cube128 c, Move *ms) {
    // open lookup table file
    FILE *lt = openLT();

    // iterate through phases
    int num_moves = 0, phase = 0;
    while (++phase <= 4) {
        // iterate until phase depth is 0
        while (indexLT(lt, cubeIndex(c, phase))) {
            Move best_move = NOP;
            Int4 best_depth;

            // iterate through moveset to find a depth reduction
            Move moveset[NUM_MOVES];
            int i = 0, n = setPhaseMoves(moveset, phase);
            while (i < n) {
                Move m = moveset[i++];
                Cube128 child_cube = applyMove(c, m);
                Int4 child_depth = indexLT(lt, cubeIndex(child_cube, phase));

                if (best_move == NOP || child_depth < best_depth) {
                    best_move = m;
                    best_depth = child_depth;
                }
            }

            // TODO: optimization
            // if best_move and parent move are on same face, then reduce
            // e.g. R and R2 -> R'
            // e.g. R' and R2 -> R
            // e.g. R and R' -> NOP (do num_moves--)
            // e.g. R2 and R2 -> NOP (do num_moves--)

            // apply move
            c = applyMove(c, best_move);
            ms[num_moves++] = best_move;

            // check if max moves are exceeded
            if (num_moves > MAX_MOVES) {
                goto exit;
            }
        }
    }

    exit: 
    fclose(lt);

    // check if solved
    return areEqual(c, cubeSolved()) ? num_moves : -1;
}