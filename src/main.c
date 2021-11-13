#include <stdio.h>
#include "cube.h"
#include "solver.h"
#include "optimizer.h"

void cube_demo() {
    Cube c0 = solvedCubeFactory();

    int n = 15;
    Move ms[n];
    Cube c1 = scrambledCubeFactory(ms, n);

    printf("Solved cube:\n");
    printCube(c0);
    printf("Scrambed %d moves:\n", n);
    printCube(c1);
    printf("Moves:\n");
    printMoves(ms, n);
}

void degree_demo() {
    Move ms[] = { M_R, M_U };
    int n = sizeof(ms) / sizeof(ms[0]);

    printf("Move sequence: ");
    printMoves(ms, n);
    printf("Degree: %d", degree(ms, n));
}

void solver_demo() {

}

void trimxyz_demo() {

}

int main() {
    degree_demo();
}