//#define MULT_LVL

//#define DEBUG
//#define DIAGNOSTIC
//#define SERIAL_DEBUG
//#define JUMP_DEBUG
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

int threadID[2]{0,66535};

int32_t position[2]{0,0};

float speed;

bool depressed = false;

Ramp *ramps[3]{new Ramp(0,0,0), new Ramp(0,0,1), new Ramp(0,0,0)};

bool acptMsg;

void idle(){
  while(!digitalRead(STEP_PEDAL));
  writeBytes[1] = (byte)'A';
  tellMega();
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


  while (elapsedTime < 1435){
    
    writeBytes[3] = (v < 0);
    writeBytes[4] =  static_cast<int>(abs(v)/3000);
    if (calculateStep > 50){
      elapsedTime += calculateStep;
      v += stepG*calculateStep*0.001;
      calculateStep = 0;
      #ifdef JUMP_DEBUG
      Serial.println(v);
      #endif
    }
    
  }
  
}

void convertPotToSPEED(){
  elapsedMillis inputRun = 0;
  while(!game_over){
  speed = analogRead(INPUT_DIR);
  if((speed > (POTMIN) && (speed < (POTMAX)))){
    speed = 0;
  }else{
  speed = 255*((speed/(POTAVG))-1);
  }
  #ifndef DIAGNOSTIC
  #ifdef DEBUG
  Serial.println("Run");
  Serial.println(speed);
  #endif
  if (speed < 0){
    writeBytesBuffer[1] = byte(1);
  }else{
    writeBytesBuffer[1] = byte(0);
  }
  writeBytesBuffer[2] = static_cast<byte>(abs(speed));
  int threadSt = threads.getState(threadID[1]);
  #ifdef DEBUG
  Serial.print("ThreadState: ");
  Serial.println(threadSt);
  #endif
  if(digitalRead(STEP_PEDAL) && !depressed){
    
    #ifndef MULT_LVL
      #ifdef DEBUG
      //Serial.print("Jump triggered.\n");
      #endif
      
      if((threadSt != threads.RUNNING)){
        depressed = true;
        threads.kill(threadID[1]);
        threadID[1] = threads.addThread(jump);
      }
      #ifdef DEBUG 
      else{
        Serial.println("not starting new thread!");
      } 
      #endif
    #else
      
    #endif
  }else if(!digitalRead(STEP_PEDAL) && depressed){
    depressed = false;
  } 
  else {
      
    #ifndef MULT_LVL
    if(threadSt != threads.RUNNING){
      writeBytesBuffer[3] = writeBytes[1];
      writeBytesBuffer[4] = static_cast<int>(writeBytes[2]*tan(radians(rampAngle)));
        
      }
      
    #else
      
    #endif
  }
  #else
  bool xOrY{false};
  if(!xOrY){
    writeBytesBuffer[0] = (speed < 0);
    writeBytesBuffer[1] = static_cast<int>(abs(speed));
    writeBytesBuffer[3] = 0;
    writeBytesBuffer[4] = 0;
  }else{
    writeBytesBuffer[3] = (speed < 0);
    writeBytesBuffer[4] = static_cast<int>(abs(speed));
    writeBytesBuffer[0] = 0;
    writeBytesBuffer[1] = 0;
  }
  if(digitalRead(STEP_PEDAL)&&!depressed){
    xOrY = !xOrY;
    depressed = true;
  }else{
    depressed = false;
  }
  #endif
  if (inputRun > 50){
    for(int i = 0; i < 8; i++){
      writeBytes[i] = writeBytesBuffer[i];
    }
    inputRun = 0;
  }
  }

}

void scanning(){
//   if(detected){
//     Serial.print();
//     while(Serial.readBytes()!=)
//   }
 }


void game(){
  while(!(Serial));
  // for(auto st: gameSteppers){
  //   st.enableOutputs();
  // }
  game_over = false;
  threads.setSliceMillis(25);
  threadID[0] = threads.addThread(convertPotToSPEED);
  threadID[1] = threads.addThread(jump);
  elapsedMillis update_clock = 0;
  while(!game_over){
    //bool count = 0;
    
    // coreSteppers.run();
    if (update_clock > 25){
      
      tellMega();

      #ifdef SERIAL_DEBUG
      Serial.println(writeBytes[1]);
      Serial.println(writeBytes[4]);
      #endif
      // #ifdef DIAGNOSTIC
      // Serial.print("(");
      // Serial.print(position[0]);
      // Serial.print(", ");
      // Serial.print(position[1]);
      // Serial.println(")");
      // #endif
      update_clock = 0;
    }
    // while(Serial2.read() != 0x5A);
    // while(Serial2.read() != 0x5B){
    //   position[count] = Serial2.read();
    //   count = !count;
    // }
  
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
  //Serial2.begin(115200);
  while(!Serial);
  pinMode(INPUT_DIR, INPUT);
  debug = true;
  Serial.println("Setup cleared");
}

void loop() {
  //debugMotors();
  acptMsg = false;
  while(Serial.available() < 8 );
  if(Serial.read() == HEADER){
    readBytes[0] = HEADER;
    for (int i = 0; i < 8; i++){
      readBytes[i] = Serial.read();
      if(readBytes[i] < 0){
        i -= 1;
      }
    }
    if(readBytes[7] == 0x0A){
      acptMsg = true;
    }
  }
  if(acptMsg){
    switch (readBytes[1])
    {
    case 0x42:
      idle();
    
    case 0x41:
      scanning();

    case 0x43:
      game();

    default:
      idle();
    }
  }
  
}