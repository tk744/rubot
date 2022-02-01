#include "cube.h"
#include "types.h"

#define RPM 60          // desired speed of revolution; independent of motor
#define MOTOR_STEPS 200 // number of steps per revolution; dependent on motor

void executeMoves(Move *ms, int n);