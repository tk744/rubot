#include "pins.h"
#include "types.h"

typedef struct {
    Int8 SER, SRCLK, RCLK;
} ShiftRegisterPins;

typedef struct {
    ShiftRegisterPins pins;
    Int8 data;
} ShiftRegister;

static ShiftRegister
    RotarySR = { { PIN_SR_ORIENTED_SER, PIN_SR_ORIENTED_SRCLK, PIN_SR_ORIENTED_RCLK }, 0 },
    LinearSR = { { PIN_SR_EXTENDED_SER, PIN_SR_EXTENDED_SRCLK, PIN_SR_EXTENDED_RCLK }, 0 };

void writeShiftRegister(ShiftRegister *sr, Int8 data);