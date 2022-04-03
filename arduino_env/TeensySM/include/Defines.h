#define STEPPER_ACTIVATION_X 12
#define STEPPER_ACTIVATION_Y 9
#define STEPPER_DIR_X 6
#define STEPPER_STEP_X 5
#define STEPPER_DIR_Y 4
#define STEPPER_STEP_Y 3

#define SCAN_ACTIVATION 2
#define SCAN_DIR_SORTING 18
#define SCAN_STEP_SORTING 19
#define SCAN_DIR_SLIDER 22
#define SCAN_STEP_SLIDER 23

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