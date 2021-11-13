#include "optimizer.h"

int degree(Move *ms, int n) {
    Cube c = solvedCubeFactory();
    int i = 0;
    do {
        c = applyMoves(c, ms, n);
        i++;
    } while (!isSolved(c));
    return i;
}