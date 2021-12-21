#include "cube.h"
#include "types.h"

/* DATA STRUCTURE CONSTANTS */

#define STACK_SIZE 270 // 15 (max depth) * 18 (max branching factor) = 270

// #define TABLE_SIZE 1000
// #define NULL ((void *)0)

/* TYPE DEFINITIONS */

typedef struct {
    Cube cube;
    Move move;
    Int8 depth;
} Node;

// typedef struct {
//     Node **ns;
//     int size;
// } Table;

typedef struct {
    Node *ns;
    int size;
} Stack;

/* FUNCTION PROTOTYPES */

// void insert(Table *t, Node *n);

// Node *lookup(Table *t, Cube c);

void push(Stack *s, Node n);

Node pop(Stack *s);

void clear(Stack *s);