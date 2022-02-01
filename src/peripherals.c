#include <time.h>
#include "peripherals.h"

#define SHIFT_REGISTER_DELAY 10

void delay(int ms) {
    clock_t start = clock(), end;
    do {
        end = clock();
    } while (end-start < CLOCKS_PER_SEC/1000 * ms);
}

void writeShiftRegister(ShiftRegister sr, Int8 data) {
    // write data to internal memory of shift register
    int i; 
    for (i=0 ; i<8 ; i++) {
        delay(SHIFT_REGISTER_DELAY);
        digitalWrite(sr.SER, (data >= 1));
        delay(SHIFT_REGISTER_DELAY);
        digitalWrite(sr.SRCLK, 1);
        delay(SHIFT_REGISTER_DELAY);
        digitalWrite(sr.SRCLK, 0);
    }
    // update output of shift register with internal memory
    delay(SHIFT_REGISTER_DELAY);
    digitalWrite(sr.RCLK, 1);
    delay(SHIFT_REGISTER_DELAY);
    digitalWrite(sr.RCLK, 0);
}

#define RPM 60          // desired speed of revolution; independent of motor
#define MOTOR_STEPS 200 // number of steps per revolution; dependent on motor

static MOTOR_DELAY = (1000*60)/(2*RPM*MOTOR_STEPS);