#define NUM_EDGES 12
#define NUM_CORNERS 8
#define NUM_MOVES 18
#define MAX_MOVES 46

typedef unsigned char Int4;
typedef unsigned char Int8;
typedef unsigned long long Int64;

typedef Int8 Move;
typedef Int8 CubieEnum;

static const Move
    NOP=0, U=1, D=2, F=4, B=8, R=16, L=32, I=64, H=128;
static const CubieEnum
    UF=0, UB=1, UR=2, UL=3, DF=4, DB=5, DR=6, DL=7, FR=8, FL=9, BR=10, BL=11;
static const CubieEnum
    UFR=0, UFL=1, UBR=2, UBL=3, DFR=4, DFL=5, DBR=6, DBL=7;

typedef struct {
    Int64 edges;
    Int64 corners;
} Cube;

// Return a solved Cube.
Cube cubeSolved();

// Find a sequence of moves which solve Cube `c` and store them in `ms`.
// Return the number of moves in `ms`, or -1 if no solution was found.
int solve(Cube c, Move *ms);

// Return 1 if `c1` and `c2` are equal, 0 otherwise.
int equalCubes(Cube c1, Cube c2);

// Returns a new Cube with the move `m` applied to Cube `c`.
Cube applyMove(Cube c, Move m);

// Returns a new Cube with the moves `ms` applied to Cube `c`.
// `n` is the number of moves in `ms`.
Cube applyMoves(Cube c, Move *ms, int n);

// Return the cubie at index `ce` in the encoding.
Int8 getCubie(Int64 encoding, CubieEnum ce);

// Set the cubie at index `ce` in the encoding to `cubie`.
void setCubie(Int64 *encoding, CubieEnum ce, Int8 cubie);

// Return the permutation from the cubie encoding.
Int8 getPermutation(Int8 cubie, int isEdge);

// Set the permutation of the cubie encoding.
void setPermutation(Int8 *cubie, int isEdge, Int8 permutation);

// Return the orientation from the cubie encoding.
Int8 getOrientation(Int8 cubie, int isEdge);

// Set the orientation of the cubie encoding.
void setOrientation(Int8 *cubie, int isEdge, Int8 orientation);

// Generate a random sequence of `n` non-redundant moves and store them in `ms`.
// `ms` must be an array of length `n`.
void setRandomMoves(Move *ms, int n);

// Parse the string representation of a cube `str` into a Cube `c`.
// `str` must contain exactly 54 characters.
// Returns 0 on success, or 1+index of the first invalid character if the representation is invalid.
int parseCubeStr(Cube *c, char *str);

// Parse the string representation of a move `str` into a Move `m`.
// Returns 0 on success, or -1 if the representation is invalid.
int parseMoveStr(Move *m, char *str);

// Print the string representation of a Cube `c`.
void printCube(Cube c);

// Print the string representation of the move sequence `ms`.
// `n` is the number of moves in `ms`.
void printMoves(Move *ms, int n);

// Print an ANSII graphic representation of a Cube `c`.
// Requires a terminal with ANSI escape codes.
void drawCube(Cube c);