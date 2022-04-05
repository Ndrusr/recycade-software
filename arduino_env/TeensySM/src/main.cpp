#include <Arduino.h>
#include"Defines.h"
/*
  X - STEPPER
    ACTIVATION PIN: 12
    STEP PIN: 5
    DIR PIN: 6
  
  Y - STEPPER
    ACTIVATION PIN: 9
    STEP PIN: 3
    DIR PIN: 4
  
  SLIDER STEPPER
    ACTIVATION PIN: 2
    STEP PIN: 23
    DIR PIN: 22

  SORTING STEPPER
    ACTIVATION PIN: 2
    STEP PIN: 19
    DIR PIN: 18

*/
#include<TeensyThreads.h>
// #include<AccelStepper.h>
// #include<MultiStepper.h>


bool debug{false};

float irReading{0};

int threadID;

bool whichMotor;

void idle(){
  Serial.print(0);
  while(true){
    
  }
}

// void scanning(){
//   if(detected){
//     Serial.print();
//     while(Serial.readBytes()!=)
//   }
// }

void game_input(){

}

void game(){
  Serial.print(2);
  // for(auto st: gameSteppers){
  //   st.enableOutputs();
  // }
  volatile bool game_over = false;
  threadID = threads.addThread(game_input);
  while(!game_over){
    // coreSteppers.run();
    if(false){
      game_over = !game_over;
    }
  }
  threads.kill(threadID);
  return;
}



// void debugMotors(){
//   whichMotor = false;
//   if(debug){
//     if(whichMotor){
//       gameSteppers[0].enableOutputs();
//       gameSteppers[1].disableOutputs();
//       gameSteppers[0].runSpeed();

//     }else{
//       gameSteppers[1].enableOutputs();
//       gameSteppers[0].disableOutputs();
      
//       while(gameSteppers[1].currentPosition()< LIMITS::STEP_Y){
//         gameSteppers[1].runSpeed();
//       }
//       delay(1000);
//       gameSteppers[1].setSpeed(gameSteppers[1].speed()*-1);
//     }
    
    
//     // Serial.print("Running motors");
//     // delay(1000);
//   }
// }



void setup() {
  
  Serial.begin(57600);

  // pinMode(STEPPER_ACTIVATION_X, OUTPUT);
  // pinMode(STEPPER_ACTIVATION_Y, OUTPUT);
  // pinMode(SCAN_ACTIVATION, OUTPUT);

  // gameSteppers[0].setEnablePin(STEPPER_ACTIVATION_X);
  // gameSteppers[1].setEnablePin(STEPPER_ACTIVATION_Y);
  // scanSteppers[0].setEnablePin(SCAN_ACTIVATION);
  // scanSteppers[1].setEnablePin(SCAN_ACTIVATION); 

  // for(int i = 0; i < 2; i++){
  //   allSteppers[i] = &gameSteppers[i];
  //   allSteppers[i+2] = &scanSteppers[i];
  // }

  // for(AccelStepper *stp: allSteppers){
  //   stp->setPinsInverted(false, false, false);
  //   stp->disableOutputs();
  // }
  // int count = 0;
  // for(AccelStepper st: gameSteppers){
  //   st.setMaxSpeed(1000);
  //   st.setSpeed(400*(1*(!count)+(count)*tan(0.1309)));

  //   coreSteppers.addStepper(st);
  //   count++ ;
  // }
  debug = true;
  
}

void loop() {
  //debugMotors();

}