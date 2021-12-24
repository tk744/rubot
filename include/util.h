#include "cube.h"
#include "types.h"

/* DATA STRUCTURE CONSTANTS */

#define STACK_SIZE 270 // 15 (max depth) * 18 (max branching factor) = 270

/* TYPE DEFINITIONS */

typedef struct {
    Cube cube;
    Move move;
    Int4 depth;
} Node;

typedef struct {
    Node *ns;
    int size;
} Stack;

/* FUNCTION PROTOTYPES */

void push(Stack *s, Node n);

Node pop(Stack *s);

void clear(Stack *s);