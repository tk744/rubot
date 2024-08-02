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
} Cube128;

Cube128 cubeSolved();


// Decode the string representation of a cube `str` into a Cube128 `c`.
// `str` must be an array of length 54.
// Returns 0 on success, or the position of the first invalid character if the representation is invalid.
int decodeCube(Cube128 *c, char *str);

// Encode a Cube128 `c` into a string representation `str`.
// `str` must be an array of length 54.
void encodeCube(Cube128 *c, char *str);

// Decode the string representation of a move `str` into a Move `m`.
// Returns 0 on success, or -1 if the representation is invalid.
int decodeMove(Move *m, char *str);

// Find a sequence of moves which solve `c` and store them in `ms`.
// Return the number of moves in `ms`, or -1 if no solution was found.
int solve(Cube128 c, Move *ms);

// Generate a random sequence of `n` non-redundant moves and store them in `ms`.
// `ms` must be an array of length `n`.
void setRandomMoves(Move *ms, int n);

// Returns a new Cube128 with the move `m` applied to `c`.
Cube128 applyMove(Cube128 c, Move m);

// Returns a new Cube128 with the moves `ms` applied to `c`.
// `n` is the number of moves in `ms`.
Cube128 applyMoves(Cube128 c, Move *ms, int n);

// Return the cubie at position `ce` in the encoding.
Int8 getCubie(Int64 encoding, CubieEnum ce);

// Return the permutation from the cubie encoding.
// Edges have permutation 0-11, corners have permutation 0-7.
Int8 getPermutation(Int8 cubie, int isEdge);

// Return the orientation from the cubie encoding.
// Edges have orientation 0 or 1, corners have orientation 0, 1, or 2.
Int8 getOrientation(Int8 cubie, int isEdge);

// Set the cubie at position `ce` in the encoding to `cubie`.
void setCubie(Int64 *encoding, CubieEnum ce, Int8 cubie);

// Set the permutation of the cubie encoding.
// Edges have permutation 0-11, corners have permutation 0-7.
void setPermutation(Int8 *cubie, int isEdge, Int8 permutation);

// Set the orientation of the cubie encoding.
// Edges have orientation 0 or 1, corners have orientation 0, 1, or 2.
void setOrientation(Int8 *cubie, int isEdge, Int8 orientation);

// Return 1 if `c1` and `c2` are equal, 0 otherwise.
int areEqual(Cube128 c1, Cube128 c2);

void printCube(Cube128 c);

void printMove(Move m);

void printMoves(Move *ms, int n);