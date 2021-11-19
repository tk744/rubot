/**
 * This library solves Rubik's cubes.
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "cube.h"

// typedef int (*Solver)(Cube, Move *, int);

// Solver solverFactory();

int solve(Cube c, Move *ms, int n);

#endif