#include "pins.h"
#include "types.h"

typedef struct {
    Int8 SER, SRCLK, RCLK;
} ShiftRegister;

static ShiftRegister
    RotarySR = { PIN_SR_ORIENTED_SER, PIN_SR_ORIENTED_SRCLK, PIN_SR_ORIENTED_RCLK },
    LinearSR = { PIN_SR_EXTENDED_SER, PIN_SR_EXTENDED_SRCLK, PIN_SR_EXTENDED_RCLK };

void writeShiftRegister(ShiftRegister sr, Int8 data);