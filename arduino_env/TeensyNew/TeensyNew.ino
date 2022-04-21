#include <TeensyThreads.h>

#include"teensydefs.h"
#include"StateMachine.h"

byte inputBuffer[BUFF_SIZE];
int readlen;
int threadIDs[2];
uint16_t positions[2];
Ramp ramps[2]{Ramp(210, 90, 1), Ramp(580, 350, -1)};
int currentRamp;
bool grounded{true};
int32_t vels[2];
int potRead;
bool jumping{false};
bool depressed{false};

StateMachine teensySM;

void setup() {
  // put your setup code here, to run once:
  pinMode(PEDAL_PIN, INPUT);
  pinMode(DIR_POT, INPUT);
  Serial.begin(115200);
  currentRamp = 0;
}

void gravity(){
  elapsedMillis timer = 0;
  while(true){
    if (timer > 50){
    if(jumping){
      vels[1] = 150000;
      jumping = false;
    }
    else if(!grounded){
      vels[1] -= (int)(320*timer);
    }else if(currentRamp == 0 || currentRamp == 3){
      vels[1] = 0;
    }else if(currentRamp == 1){
      vels[1] = (int)round(vels[0]*tan(radians(15)));
      
    }else {
      vels[1] = (int)round(vels[0]*(-1)*tan(radians(15)));
      
    }
    timer = 0;
    }
  }
}

void whatRamp(){
  switch(currentRamp){
    case 0:
      if(positions[1] <= 60){
        grounded = true;
      }else if(positions[0] > ramps[0].getX()){
        if(positions[1] >= ramps[0].checkPos(positions[0])){
          currentRamp = 1;
          if(positions[1] <= ramps[0].checkPos(positions[0])){
            grounded = true;
          }else{
            grounded = false;
           }
        }else{
          grounded = false;
        }
      }else{
        grounded = false;
      }
    case 1:
      if(positions[0] < ramps[0].getX()){
        currentRamp = 0;
        grounded = false;
       }else if( positions[1] <= ramps[0].checkPos(positions[0])){
        grounded = 1;
      }
      else if(positions[0] < ramps[1].getX()){
        
        if(positions[1] >= ramps[1].checkPos(positions[0])){
          
          currentRamp = 2;
          if(positions[1] == ramps[1].checkPos(positions[0])){
            grounded = true;
          }else{
            grounded = false;
          }
        }else{
          grounded = false;
        }
      }else{
        grounded = false;
        }
      break;
    case 2:
      if(positions[0] > ramps[1].getX()){
        currentRamp = 1;
        grounded = false;
      }else if(positions[1] <= ramps[1].checkPos(positions[0])){
        grounded = true;
      }else if(positions[0] > 260){
        if(positions[1] >= 540){
          currentRamp = 3;
          if(positions[1] <=540){
            grounded = true;
          }else{
            grounded = false;
          }
        }
       }else{
          grounded = false;
       }
       break;
    case 3:
      if(positions[0] > 260){
        if(positions[1] <= 540){
          grounded = true;
        }else{
          grounded = false;
        }
      }else{
        currentRamp = 2;
        grounded = false;
      }
      break;
    }
}
void inputs(){
  potRead = analogRead(DIR_POT);
  if(potRead < POTMAX && potRead > POTMIN){
    vels[0] = 0;
  }else if(potRead < POTMIN && positions[0] > 0){
    vels[0] = -150000;
  }else if(potRead > POTMAX && positions[0] < X_LIMIT){
    vels[0] = 150000;
  }else{
    vels[0] = 0;
  }
  if(!depressed && digitalRead(PEDAL_PIN)){
    jumping = true;
    depressed = false;
  }else if(grounded && !digitalRead(PEDAL_PIN)){
    depressed = false;
  }
}
void game(){
  bool game_over{false};
  Serial.print("ZG10000\n");
  threadIDs[0] = threads.addThread(gravity);
  while(!game_over){
    while (Serial.available()<8);
    readlen = Serial.readBytesUntil('\n', inputBuffer, BUFF_SIZE);
    if(inputBuffer[0] == (byte)'G' && readlen == 7){
      positions[0] = (inputBuffer[1]) +(inputBuffer[2] << 8);
      positions[1] = (inputBuffer[3]) +(inputBuffer[4] << 8);
      whatRamp();
      inputs();
      
      velMsg[1] = (byte)(positions[0]&0xff);
      velMsg[2] = (byte)((positions[0] >> 8) & 0xff);
      velMsg[3] = (byte)((positions[0] >> 16) & 0xff);
      velMsg[4] = (byte)((positions[0] >> 24) & 0xff);
      velMsg[5] = (byte)(positions[1]&0xff);
      velMsg[6] = (byte)((positions[1] >> 8) & 0xff);
      velMsg[7] = (byte)((positions[1] >> 16) & 0xff);
      velMsg[8] = (byte)((positions[1] >> 24) & 0xff);

      Serial.write(velMsg, 10);
      
    }else if(inputBuffer[0] == (byte)'Z' && readlen ==7){
      game_over = true;
      threads.kill(threadIDs[0]);
      
  }
}
}

void loop() {
  while(Serial.available ()< 8);
  readlen = Serial.readBytesUntil('\n', inputBuffer, BUFF_SIZE);
  if(inputBuffer[0] == (byte)'Z' && readlen == 7){
   teensySM.switchStates(inputBuffer[1]);
   switch(teensySM.getSMState()){
    case 0:
      idling();
      break;
    case 1:
      break;
    case 2:
      game();
      //Serial.print("PLACEHOLDER");
      break;
   }
  }

}
