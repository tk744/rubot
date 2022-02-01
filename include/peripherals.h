#include "pins.h"
#include "types.h"

typedef struct {
    Int8 SER, SRCLK, RCLK;
} ShiftRegister;

static ShiftRegister
    RotarySR = { PIN_SR_ROTARY_SER, PIN_SR_ROTARY_SRCLK, PIN_SR_ROTARY_RCLK },
    LinearSR = { PIN_SR_LINEAR_SER, PIN_SR_LINEAR_SRCLK, PIN_SR_LINEAR_RCLK };

void writeShiftRegister(ShiftRegister sr, Int8 data);