#include <stdio.h>
#include "solver.h"

void demo0() {
    // srand
    srand(&demo0);

    // scramble
    int n_scramble = 5;
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

void c2ccMapping() {
    Cube c0 = cubeFactory();
    Cube c1 = applyMove(c0, F);
    Cube c2 = applyMove(c0, R);
    Cube c3 = applyMove(c0, U|I);
    Cube c4 = applyMove(applyMove(c0, F), U);
    Cube c5 = applyMove(applyMove(c0, U), F|H);

    printf("-- ORIGINAL CUBE --\n");
    printCube(c0);
    printf("-- F MOVE --\n");
    printCube(c1);
    printf("-- R MOVE --\n");
    printCube(c2);
    printf("-- U' MOVE --\n");
    printCube(c3);
    printf("-- F U MOVE --\n");
    printCube(c4);
    printf("-- U F2 MOVE --\n");
    printCube(c5);
}

int main() {
    demo0();
    return 0;
}