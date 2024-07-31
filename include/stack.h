#include "cube.h"
#include <stdio.h>

#define STACK_SIZE 270 // 15 (max depth) * 18 (max branching factor) = 270

typedef struct {
    Cube128 cube;
    Move move;
    Int4 depth;
} Node;

typedef struct {
    Node *ns;
    int size;
} Stack;

void push(Stack *s, Node n) {
    if (s->size < STACK_SIZE) {
        s->ns[s->size++] = n;
    }
    else {
        printf("ERROR: Push onto full stack!\n");
    }
}

Node pop(Stack *s) {
    if (s->size > 0) {
        return s->ns[--s->size];
    }
    else {
        printf("ERROR: Pop from empty stack!\n");
    }
}

void clear(Stack *s) {
    s->size = 0;
}