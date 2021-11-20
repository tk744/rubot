#include "cube.h"

/* DATA STRUCTURE CONSTANTS */

#define TABLE_SIZE 100
#define QUEUE_SIZE 1000000

#define NULL ((void *)0)

/* TYPE DEFINITIONS */

typedef struct Node {
    struct Node *prev_node;
    Move prev_move;
    Cube cube;
} Node;

typedef struct {
    Node **ns;
    int size;
} Table;

typedef struct {
    Cube *cs;
    int front;
    int rear;
} Queue;

/* FUNCTION PROTOTYPES */

void insert(Table *t, Node *n);

Node *lookup(Table *t, Cube c);

int isEmpty(Queue *q);

void enqueue(Queue *q, Cube c);

Cube dequeue(Queue *q);