#include "types.h"

#define PIN_SHIFT4_SER      0
#define PIN_SHIFT4_SRCLK    0
#define PIN_SHIFT4_RCLK     0

#define PIN_SHIFT2_SER      0
#define PIN_SHIFT2_SRCLK    0
#define PIN_SHIFT2_RCLK     0

// pins for OV7670 camera
#define PIN_CAMERA_SCL      0
#define PIN_CAMERA_SDA      0
#define PIN_CAMERA_VS       0
#define PIN_CAMERA_HS       0
#define PIN_CAMERA_PLK      0
#define PIN_CAMERA_XLK      0
#define PIN_CAMERA_D0       0
#define PIN_CAMERA_D1       0
#define PIN_CAMERA_D2       0
#define PIN_CAMERA_D3       0
#define PIN_CAMERA_D4       0
#define PIN_CAMERA_D5       0
#define PIN_CAMERA_D6       0
#define PIN_CAMERA_D7       0

typedef struct {
    Int8 SER, SRCLK, RCLK;
} ShiftRegister;

static ShiftRegister SR4 = { PIN_SHIFT4_SER, PIN_SHIFT4_SRCLK, PIN_SHIFT4_RCLK },
                     SR2 = { PIN_SHIFT2_SER, PIN_SHIFT2_SRCLK, PIN_SHIFT2_RCLK };

void writeShiftRegister(ShiftRegister sr, Int8 data);
