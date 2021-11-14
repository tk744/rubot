/**
 * This library solves Rubik's cubes.
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "cube.h"

typedef int (*Solver)(Cube, Move *, int);

Solver solverFactory();

#endif