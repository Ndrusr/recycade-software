/*
    X-Stepper
*/
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53
/*
    Y-Stepper
*/
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49
//NOT IN USE
#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40
/*
    Scanning Stepper
*/
#define SCAN_STEP_PIN      26
#define SCAN_DIR_PIN       28
#define SCAN_ENABLE_PIN    24
#define SCAN_CS_PIN        42
/*
    Slider Stepper
*/
#define SLIDER_STEP_PIN    36
#define SLIDER_DIR_PIN     34
#define SLIDER_ENABLE_PIN  30
#define SLIDER_CS_PIN      44

// Endstop Pins
#define X_STOP              3
#define Y_STOP             14

//ByteBuffer

#define BYTE_COUNT          8
#define BYTE_COUNT_USB      1

    enum LIMITS{
        X = 876,
        Y = 679,
        STEP_X = 8758,
        STEP_Y = 13574
    };

volatile bool detected;

int dist;
int i;
int uart[9];
const int HEADER=0x59;
int check;