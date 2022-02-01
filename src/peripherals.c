#include "peripherals.h"

static void digitalWrite(int pin, int x) {
    
}

void writeShiftRegister(ShiftRegister *sr, Int8 data) {
    printf("%d\n", data);
    // update internal state
    sr->data = data;
    // write shift register
    int i; 
    for (i=0 ; i<8 ; i++) {
        digitalWrite(sr->pins.SER, !!(data & (1 << i)));
        digitalWrite(sr->pins.SRCLK, 1);
        digitalWrite(sr->pins.SRCLK, 0);
    }
    // write output
    digitalWrite(sr->pins.RCLK, 1);
    digitalWrite(sr->pins.RCLK, 0);
}