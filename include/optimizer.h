/**
 * This library optimizes move sequences.
 */

#include "cube.h"

int degree(Move *ms, int n);

int trim_xyz(Cube c, Move *ms, int n);