#include <Arduino.h>
#include"StateMachine.h"
#include"Defines.h"
#include<AccelStepper.h>
#include<MultiStepper.h>

StateMachine::RecSM recycade;
AccelStepper xStepper(1, STEPPER_STEP_X, STEPPER_DIR_X);
AccelStepper yStepper(1, STEPPER_STEP_Y, STEPPER_DIR_Y);
MultiStepper coreSteppers;
const char* debug;

bool pinSet{false};

void scanning(){
  Serial1.begin(9600);
  
}

void game(){
  

}

void debugMotors(){
  if (!pinSet){
    digitalWrite(STEPPER_ACTIVATION, HIGH);
    pinSet = !pinSet;
  }
  if(debug != nullptr){
    xStepper.runSpeed();
    yStepper.runSpeed();
    // Serial.print("Running motors");
    // delay(1000);
  }
}

void setup() {
  
  debug = recycade.debugState();
  Serial.begin(9600);
  
  pinMode(STEPPER_ACTIVATION, OUTPUT);

  xStepper.setMaxSpeed(400);
  xStepper.setSpeed(400);
  
  yStepper.setMaxSpeed(400);
  yStepper.setSpeed(400
  );
  
  coreSteppers.addStepper(xStepper);
  coreSteppers.addStepper(yStepper);
}

void loop() {
  // put your main code here, to run repeatedly:
  debugMotors();

  
}