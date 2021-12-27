#include "util.h"
#include <stdio.h>

/* FUNCTION IMPLEMENTATION */

void push(Stack *s, Node n) {
    if (s->size < STACK_SIZE) {
        s->ns[s->size++] = n;
    }
    else {
        printf("WARNING: attempted push on full stack.\n");
    }
}

Node pop(Stack *s) {
    if (s->size > 0) {
        return s->ns[--s->size];
    }
    else {
        printf("WARNING: attempted pop from empty stack.\n");
    }
}

void clear(Stack *s) {
    s->size = 0;
}