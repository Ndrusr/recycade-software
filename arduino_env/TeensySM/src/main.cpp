#include <Arduino.h>
#include"Defines.h"

#include<AccelStepper.h>
#include<MultiStepper.h>

AccelStepper gameSteppers[] = {AccelStepper(1, STEPPER_STEP_X, STEPPER_DIR_X), AccelStepper(1, STEPPER_STEP_Y,STEPPER_DIR_Y)};
AccelStepper scanSteppers[] = {AccelStepper(1, SCAN_STEP_SLIDER, SCAN_DIR_SLIDER), AccelStepper(1, SCAN_STEP_SORTING, SCAN_DIR_SORTING)};
MultiStepper coreSteppers;
bool debug{false};
AccelStepper *allSteppers[4];
float irReading{0};

bool whichMotor;

void idle(){
  Serial.print(0);
  while(true){
    
  }
}

void scanning(){
}

void game(){
  Serial.print(2);
  digitalWrite(STEPPER_ACTIVATION_X, HIGH);
  digitalWrite(STEPPER_ACTIVATION_Y, HIGH);
  static bool game_over = false;
  while(!game_over){
      
  }
}


void debugMotors(){
  
  if(debug){
    if(whichMotor){
      gameSteppers[0].enableOutputs();
      gameSteppers[1].disableOutputs();
      gameSteppers[0].runSpeed();

    }else{
      gameSteppers[1].enableOutputs();
      gameSteppers[0].disableOutputs();
      gameSteppers[1].runSpeed();
    }
    
    
    // Serial.print("Running motors");
    // delay(1000);
  }
}



void setup() {
  
  Serial.begin(57600);

  pinMode(STEPPER_ACTIVATION_X, OUTPUT);
  pinMode(STEPPER_ACTIVATION_Y, OUTPUT);
  pinMode(SCAN_ACTIVATION, OUTPUT);

  gameSteppers[0].setEnablePin(STEPPER_ACTIVATION_X);
  gameSteppers[1].setEnablePin(STEPPER_ACTIVATION_Y);
  scanSteppers[0].setEnablePin(SCAN_ACTIVATION);
  scanSteppers[1].setEnablePin(SCAN_ACTIVATION); 

  for(int i = 0; i < 2; i++){
    allSteppers[i] = &gameSteppers[i];
    allSteppers[i+2] = &scanSteppers[i];
  }

  for(AccelStepper *stp: allSteppers){
    stp->disableOutputs();
  }

  for(AccelStepper st: gameSteppers){
    st.setMaxSpeed(1000);
    st.setSpeed(400);
    coreSteppers.addStepper(st);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  debugMotors();

}