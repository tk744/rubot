#include <stdio.h>
#include "solver.h"

void demo0() {
    // srand
    srand(&demo0);

    // scramble
    int n_scramble = 8;
    Move ms_scramble[n_scramble];
    Cube c_scramble = scramble(cubeFactory(), ms_scramble, n_scramble);

    // display
    printf("\n-- INITIAL STATE --\n");
    printCube(c_scramble);
    printf("\n-- SCRAMBLE MOVES --\n");
    printf("[%u]: ", n_scramble);
    printMoves(ms_scramble, n_scramble);

    // solve
    printf("\n-- SOLVER DEBUG --\n");
    Move ms_solution[MAX_MOVES];
    int n_solution = solve(c_scramble, ms_solution);
    Cube c_solution = applyMoves(c_scramble, ms_solution, n_solution);

    // display
    printf("\n");
    printf("\n-- SOLUTION MOVES --\n");
    printf("[%u]: ", n_solution);
    printMoves(ms_solution, n_solution);
    printf("\n-- FINAL STATE --\n");
    printCube(c_solution);
}

int main() {
    demo0();
    return 0;
}