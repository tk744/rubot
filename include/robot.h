#include "solver.h"

// REP: x1, x2, f1a, f1b, f2a, f2b

// x_[2]: 0-stay 1-extend 2-retreat
// f_[2]: 0-stay 1-90R 2-90L 3-180
// 2*6 = 12 bits < 2 bytes
typedef unsigned short MotorMove;

// x_[1]: 0-extended 1-retreated
// f_[1]: 0-horizontal 1-vertical
// 1*6 = 6 bits < 1 byte
typedef char MotorState;

static MotorMove convertMove(Move m);

static MotorMove *convertMoves(Move* ms, int n);

MotorState executeMove(Move m);

MotorState executeMoves(Move *ms, int n);
