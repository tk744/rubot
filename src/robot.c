#include "robot.h"

#define RPM 60          // desired speed of revolution; independent of motor
#define MOTOR_STEPS 200 // number of steps per revolution; dependent on motor

static MOTOR_DELAY = (1000*60)/(2*RPM*MOTOR_STEPS);

static void rotateMotor(Motor m, Int8 isCW, Int8 isH) {
    // set motor direction
    digitalWrite(m.dir_pin, isCW);
    
    // send step pulses to motor
    int i;
    for (i = 0 ; i < MOTOR_STEPS/(isH ? 2 : 4) ; i++) {
        digitalWrite(m.step_pin, 1);
        delay(MOTOR_DELAY);
        digitalWrite(m.step_pin, 0);
        delay(MOTOR_DELAY);
    }
}

void rotateCW(Motor m) {
    rotateMotor(m, 1, 0);
}

void rotateCCW(Motor m) {
    rotateMotor(m, 0, 0);
}

void rotateCW2(Motor m) {
    rotateMotor(m, 1, 1);
}

void rotateCCW2(Motor m) {
    rotateMotor(m, 0, 1);
}