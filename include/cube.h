#define NUM_EDGES 12
#define NUM_CORNERS 8
#define NUM_MOVES 18

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

int initFromString(Cube128 *c, char cs[54]);

void setRandomMoves(Move *ms, int n);

void setRandomMovesSeeded(Move *ms, int n, unsigned int seed);

Cube128 applyMove(Cube128 c, Move m);

Cube128 applyMoves(Cube128 c, Move *ms, int n);

Int8 getCubie(Int64 encoding, CubieEnum ce);

Int8 getPermutation(Int8 cubie, int isEdge);

Int8 getOrientation(Int8 cubie, int isEdge);

void setCubie(Int64 *encoding, CubieEnum ce, Int8 cubie);

void setPermutation(Int8 *cubie, int isEdge, Int8 permutation);

void setOrientation(Int8 *cubie, int isEdge, Int8 orientation);

int areEqual(Cube128 c1, Cube128 c2);

void printCube(Cube128 c);

void printMove(Move m);

void printMoves(Move *ms, int n);