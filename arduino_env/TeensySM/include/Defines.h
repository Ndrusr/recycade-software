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

#define HEADER 0x5A

const static float rampAngle = 15;
    enum STPMM{
        X = 10,
        Y = 20
    };


    namespace LIMITS{
        const uint16_t X = 8758;
        const uint16_t Y = 13574; 
    }
    enum DETECT{
        BOTTLE = 1,
        CAN = 0
    };

volatile bool detected;

byte writeBytesBuffer[8] {HEADER,0,0,0,0,0,0,0x0A};
byte writeBytes[8] {HEADER,0,0,0,0,0,0,0x0A};   

byte readBytes[8];

void tellMega(){
    while(Serial.availableForWrite() < 8);
    Serial.write(writeBytes, sizeof(writeBytes));
    return;
}

#define POTMIN 520
#define POTMAX 540
#define POTAVG 530


const double milliG = -320.5128;
const double stepG = milliG*STPMM::Y;

volatile bool game_over{false};

class Ramp{

private:
int startX;
int startY;
int dir;
const double grad{tan(radians(15))};
int rampEnd;
const double length{700};

public:

Ramp(int xPos, int yPos, bool LR):
    startX(xPos), startY(yPos)
{
    dir = (LR)*(-1) + (!LR)*(1);
    rampEnd = dir*sin(length) + xPos;
}

int getDir(){
    return dir;
}

int checkPos(int pos){
    return static_cast<int>((dir*grad*((pos/STPMM::X) - startX) + startY)*STPMM::Y);
}

int getStartY(){
    return startY;
}

int getStartX(){
    return startX;
}

};