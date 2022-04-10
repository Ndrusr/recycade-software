//#define MULT_LVL

#define DEBUG

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

int threadID[2]{0,66535};

float speed;

bool depressed = false;



void idle(){
  Serial.print(0);
  while(true){
    
  }
}

/*
  byte package:
  byte 1 - invert x
  byte 2 - x speed
  */

void jump(){
  
  float v = 3000;
  int elapsedTime = 0;
  elapsedMillis calculateStep = 0;

  Serial.println("Thread spun");

  while (elapsedTime < 1435){
    writeBytes[3] = (v < 0);
    writeBytes[4] =  static_cast<int>(abs(v));
    if (calculateStep > 50){
      elapsedTime += calculateStep;
      calculateStep = 0;
      v += stepG;
    }
    
  }
  Serial.println("Jump Terminated.");
}

void convertPotToSPEED(){
  while(true){
  
  speed = 255*((analogRead(INPUT_DIR)/(511.5))-1);
  #ifdef DEBUG
  Serial.println("Run");
  Serial.println(speed);
  #endif
  writeBytes[0] = (speed < 0);
  writeBytes[1] = static_cast<int>(abs(speed));
  Serial.println(writeBytes[1]);
  int threadSt = threads.getState(threadID[1]);
  if(digitalRead(STEP_PEDAL) && !depressed){
    depressed = true;
    #ifndef MULT_LVL
      #ifdef DEBUG
      //Serial.print("Jump triggered.\n");
      #endif
      
      if((threadSt != threads.RUNNING)){
        threads.kill(threadID[1]);
        threadID[1] = threads.addThread(jump);
      }
      #ifdef DEBUG 
      else{
        Serial.println("wtf");
      } 
      #endif
    #else
      
    #endif
  }else{
    depressed = false;
    #ifndef MULT_LVL
    if(threadSt != threads.RUNNING){
        writeBytes[3] = writeBytes[0];
        writeBytes[4] = static_cast<int>(writeBytes[1]*tan(radians(rampAngle)));
        
      }
      
    #else
      
    #endif
  }
  Serial.println(writeBytes[4]);
  }

}

// void scanning(){
//   if(detected){
//     Serial.print();
//     while(Serial.readBytes()!=)
//   }
// }



void game(){
  Serial.print(2);
  // for(auto st: gameSteppers){
  //   st.enableOutputs();
  // }
  volatile bool game_over = false;
  threadID[0] = threads.addThread(convertPotToSPEED);
  

  while(!game_over){
    // coreSteppers.run();
    tellMega();
  }
  Serial.print("Game Over!");
  for(auto thread: threadID){
    if(threads.getState(thread) == threads.RUNNING){
    threads.kill(thread);}
  }
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
  
  Serial.begin(115200);
  Serial2.begin(115200);
  while(!Serial);
  pinMode(INPUT_DIR, INPUT);
  debug = true;
  Serial.println("Setup cleared");
}

void loop() {
  //debugMotors();
  game();
}