#include "hardware.h"
#include "motors.h"
#include "peripherals.h"

static int MOTOR_DELAY = (1000*60)/(2*RPM*MOTOR_STEPS);

typedef Int8 RotaryTransform, LinearTransform;
typedef Int4 RotaryEnum;

typedef struct {
    RotaryTransform rt;
    LinearTransform lt;
} Transform;

static RotaryEnum CW=1, CCW=2;

static Transform convertMove(Move m) {
    RotaryTransform rt;
    LinearTransform lt;

    RotaryEnum re = (m & I) ? CCW : CW; 

    if (m & U || m & D) {
        // TODO
    }
    else if (m & F) {
        rt = re << 0;
    }
    else if (m & B) {
        rt = re << 2;
    }
    else if (m & R) {
        rt = re << 4;
    }
    else if (m & L) {
        rt = re << 6;
    }

    Transform t = { rt, lt };
    return t;
}

static void executeRotaryTransform(RotaryTransform rt) {
    // get directions and activations of motors for shift register
    Int8 s=0, d=0;
    int i;
    for (i=0 ; i<8 ; i+=2) {
        RotaryEnum re = (rt >> i) & 0b11;
        s |= re ? (0b01 << i) : 0;
        d |= (re & CCW) ? (0b10 << i) : 0;
    }

    // set directions of motors
    writeShiftRegister(RotarySR, d);

    // send step pulses to motors
    for (i = 0 ; i < MOTOR_STEPS/4 ; i++) {
        delay(MOTOR_DELAY);
        writeShiftRegister(RotarySR, d^=s);
    }
}

static void executeLinearTransform(LinearTransform lt) {
    
}

void executeMoves(Move *ms, int n) {
    Transform t;
    while(n-- > 0) {
        t = convertMove(*ms++);
        if (t.lt) {
            executeLinearTransform(t.lt);
        }
        if (t.rt) {
            executeRotaryTransform(t.rt);
        }
    }
}

// int main() {
//     Transform t = convertMove(F);
//     executeRotaryTransform(t.rt);
// }