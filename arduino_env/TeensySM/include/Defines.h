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
#ifndef ARDUINO_H
#include <Arduino.h>
#endif
#define INPUT_DIR A0
#define STEP_PEDAL 6

const static float rampAngle = 15;
    enum STPMM{
        X = 10,
        Y = 20
    };


    namespace LIMITS{
        const uint16_t X = 8758;
        const u_int16_t Y = 13574; 
    }
    enum DETECT{
        BOTTLE = 1,
        CAN = 0
    };

volatile bool detected;

byte writeBytesBuffer[8] {0,0,0,0,0,0,0,0};
byte writeBytes[8] {0,0,0,0,0,0,0,0};   

void tellMega(){
    for(byte byte: writeBytes){
        Serial2.write(byte);
    }
    
}

const double milliG = -209.00592334;
const double stepG = milliG*STPMM::Y;

volatile bool game_over{false};