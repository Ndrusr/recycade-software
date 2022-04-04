// #define STEPPER_ACTIVATION_X 12
// #define STEPPER_ACTIVATION_Y 9
// #define STEPPER_DIR_X 6
// #define STEPPER_STEP_X 5
// #define STEPPER_DIR_Y 4
// #define STEPPER_STEP_Y 3

// #define SCAN_ACTIVATION 2
// #define SCAN_DIR_SORTING 18
// #define SCAN_STEP_SORTING 19
// #define SCAN_DIR_SLIDER 22
// #define SCAN_STEP_SLIDER 23

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#define E0_CS_PIN          42

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
#define E1_CS_PIN          44

#define INPUT_DIR 
#define STEP_PEDAL

    enum LIMITS{
        X = 860,
        Y = 760,
        STEP_X = 68800,
        STEP_Y = 60800
    };

    enum DETECT{
        BOTTLE = 1,
        CAN = 0
    };

volatile bool detected;