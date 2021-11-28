/**
 * This library solves Rubik's cubes.
 */

#ifndef SOLVER_H
#define SOLVER_H

#define MAX_MOVES 52

#include "cube.h"

int solve(Cube c, Move *ms);

#endif