#include "pins.h"
#include "types.h"

typedef struct {
    Int8 step_pin, dir_pin;
} Motor;

static Motor F_MOTOR = { PIN_MOTOR_F_STEP, PIN_MOTOR_F_DIR },
             B_MOTOR = { PIN_MOTOR_B_STEP, PIN_MOTOR_B_DIR },
             R_MOTOR = { PIN_MOTOR_R_STEP, PIN_MOTOR_R_DIR }, 
             L_MOTOR = { PIN_MOTOR_L_STEP, PIN_MOTOR_L_DIR },
             FB_MOTOR = { PIN_MOTOR_FB_STEP, PIN_MOTOR_FB_DIR }, 
             RL_MOTOR = { PIN_MOTOR_RL_STEP, PIN_MOTOR_RL_DIR };

void rotateCW(Motor m);

void rotateCCW(Motor m);
