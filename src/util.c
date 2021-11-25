#include "util.h"

/* FUNCTION IMPLEMENTATION */

// static int hash(Cube c) {
//     return c.edges * c.corners;
// }

// static int tableIndex(Table *t, Cube c) {
//     int idx = hash(c) % TABLE_SIZE;
//     for ( ; t->ns[idx] != NULL && !areEqual(t->ns[idx]->cube, c) ; idx = (idx+1) % TABLE_SIZE);
//     return idx;
// }

// void insert(Table *t, Node *n) {
//     t->ns[tableIndex(t, n->cube)] = n;
//     t->size++;
// }

// Node *lookup(Table *t, Cube c) {
//     return t->ns[tableIndex(t, c)];
// }

void push(Stack *s, Node n) {
    if (s->size < STACK_SIZE) {
        s->ns[s->size++] = n;
    }
}

Node pop(Stack *s) {
    if (s->size > 0) {
        return s->ns[s->size--];
    }
}

void clear(Stack *s) {
    s->size = 0;
}