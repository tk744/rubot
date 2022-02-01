#include "peripherals.h"

static void digitalWrite(int pin, int x) {
    
}

void writeShiftRegister(ShiftRegister sr, Int8 data) {
    // shift in
    int i; 
    for (i=0 ; i<8 ; i++) {
        digitalWrite(sr.SER, !!(data & (1 << i)));
        digitalWrite(sr.SRCLK, 1);
        digitalWrite(sr.SRCLK, 0);
    }
    // parallel out
    digitalWrite(sr.RCLK, 1);
    digitalWrite(sr.RCLK, 0);
}