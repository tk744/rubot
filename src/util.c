#include "util.h"

/* FUNCTION IMPLEMENTATION */

static int hash(Cube c) {
    return c.edges * c.corners;
}

static int tableIndex(Table *t, Cube c) {
    int idx = hash(c) % TABLE_SIZE;
    for ( ; t->ns[idx] != NULL && !areEqual(t->ns[idx]->cube, c) ; idx = (idx+1) % TABLE_SIZE);
    return idx;
}

// static int contains(Table *m, Cube c) {
//     return (m->xs[tableIndex(m, c)] != NULL);
// }

void insert(Table *t, Node *n) {
    t->ns[tableIndex(t, n->cube)] = n;
    t->size++;
}

Node *lookup(Table *t, Cube c) {
    return t->ns[tableIndex(t, c)];
}

int isEmpty(Queue *q) {
    return (q->front == q->rear);
}

void enqueue(Queue *q, Cube c) {
    if (q->rear < QUEUE_SIZE) {
        q->cs[q->rear++] = c;
    }
}

Cube dequeue(Queue *q) {
    if (q->front < q->rear) {
        return q->cs[q->front++];
    }
}