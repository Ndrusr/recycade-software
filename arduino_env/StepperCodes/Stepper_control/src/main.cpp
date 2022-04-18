#include <Arduino.h>
#include <Defines.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <stdio.h>

//#define DEBUG
#define LAPTOP_MANUAL

/*
  X - STEPPER
    ACTIVATION PIN: 38
    STEP PIN:       54
    DIR PIN:        55
  
  Y - STEPPER
    ACTIVATION PIN: 56
    STEP PIN:       60
    DIR PIN:        61
  
  SLIDER STEPPER
    ACTIVATION PIN: 30
    STEP PIN:       36
    DIR PIN:        34

  SCANNING STEPPER
    ACTIVATION PIN: 24
    STEP PIN:       26
    DIR PIN:        28
*/

AccelStepper *gameSteppers[2] = {new AccelStepper(1, X_STEP_PIN, X_DIR_PIN), new AccelStepper(1, Y_STEP_PIN, Y_DIR_PIN)};
AccelStepper *scanSteppers[2] = {new AccelStepper(1, SLIDER_STEP_PIN, SLIDER_DIR_PIN), new AccelStepper(1, SCAN_STEP_PIN, SCAN_DIR_PIN)};

MultiStepper coreSteppers;
AccelStepper *allSteppers[4];

byte inputBytes[BYTE_COUNT];
#ifdef LAPTOP_MANUAL
char USBBytes[BYTE_COUNT_USB];
#endif

int16_t positions[2]{0,0};


#ifdef DEBUG
template<typename R>
void sendStuffToSerial(R a, R b){
  Serial.print("(");
  Serial.print(a);
  Serial.print(", ");
  Serial.print(b);
  Serial.print(")\n");
}
template<typename T>
void sendstuffToSerial(T a[], int len){
  Serial.print("(");
  for(int i = 0; i < len; i++){
    Serial.print(a[i]);
    Serial.print(", ");
  }
  Serial.print(")\n");
}
#endif
void calibMotors(){
  //Serial.print("beginning calibration step\n");
  long target[2]{-100000, -100000};
  coreSteppers.moveTo(target);
  #ifdef DEBUG
  sendStuffToSerial<float>(gameSteppers[0]->targetPosition(), gameSteppers[1]->targetPosition());
  #endif
  for(auto st : gameSteppers){
    st->enableOutputs();
    st->setSpeed(-100.0);
  }
  gameSteppers[1]->setSpeed(200.0);
  bool xStop = false, yStop = false;
  #ifdef DEBUG
  Serial.print("Beginning move ");
  
  sendStuffToSerial<float>(gameSteppers[0]->speed(), gameSteppers[1]->speed());
  #endif

  while (!(xStop)){
    gameSteppers[0]->runSpeed();
    if(!digitalRead(X_STOP) && !xStop){
      xStop = !xStop;
      Serial.print(0);
      gameSteppers[0]->stop();
      gameSteppers[0]->disableOutputs();
    }
  }
  Serial1.begin(115200);
  while(!(Serial1.available() > 0));
  Serial.println("Beginning y move");
  //digitalWrite(Y_STOP, HIGH);
  while(!yStop){
    if(Serial1.read() == HEADER) { /*assess data package frame header 0x59*/
      uart[0]=HEADER;
      //Serial.println("Header get");
      if (Serial1.read() == HEADER) { /*assess data package frame header 0x59*/
        uart[1] = HEADER;
        //Serial.println("Len clear");
        for (i = 2; i < 9; i++) { /*save data in array*/
        uart[i] = Serial1.read();
        if(uart[i] == -1){
          i -= 1;
        }
        }
        //sendstuffToSerial<int>(uart, 9);
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        //Serial.println(check & 0xff);
        //Serial.println(uart[8]);
        if (uart[8] == (check & 0xff)){ /*verify the received data as per protocol*/
          //Serial.println("chksum clr");
          dist = uart[2] + uart[3] * 256;
          //Serial.println(dist);
          if(dist <= 4){
            Serial.print(1);
            yStop = !yStop;
            gameSteppers[1]->stop();
            gameSteppers[1]->disableOutputs();
          }
        }
      }
     }
    //Serial.println(2);
    gameSteppers[1]->runSpeed();
    delay(1);

    // if(!digitalRead(Y_STOP) && !yStop){
    //   yStop = !yStop;
    //   Serial.print(1);
    //   gameSteppers[1]->stop();
    //   gameSteppers[1]->disableOutputs();
    // }
  }
  //digitalWrite(Y_STOP, LOW);
  #ifdef DEBUG
  Serial.print("Zero Found\n");
  #endif
  for(auto st: gameSteppers){
    st->setCurrentPosition(0);
  }
}

void setup() {
  Serial.begin(115200);
  //Serial.begin(115200);

  while(!Serial);

  for(int i = 0; i < 2; i++){
    allSteppers[i] = gameSteppers[i];
    allSteppers[i+2] = scanSteppers[i];
  }

  Serial.print("Steppers Init\n");

  int activationPins[4] = {X_ENABLE_PIN, Y_ENABLE_PIN, SLIDER_ENABLE_PIN, SCAN_ENABLE_PIN};

  //pinMode(Y_STOP, OUTPUT);

  int count = 0;

  for(AccelStepper *stp: allSteppers){
    stp->setEnablePin(activationPins[count]);
    stp->setPinsInverted(false, false, true);
    stp->enableOutputs();
    count++;
  }

  Serial.print("Enable_set\n");

  count = 0;
  
  for(AccelStepper *st: gameSteppers){
    st->setMaxSpeed(3000);
    st->setSpeed(1500*(count+1));

    coreSteppers.addStepper(*st);
    count++ ;
  }
  #ifdef DEBUG
  sendStuffToSerial(gameSteppers[0]->maxSpeed(), gameSteppers[1]->maxSpeed());
  #endif
  for(AccelStepper *stp: allSteppers){
    stp->disableOutputs();
  }
  #ifdef DEBUG
  sendStuffToSerial(gameSteppers[0]->speed(), gameSteppers[1]->speed());
  #endif
  calibMotors();

}

void idle(){
  for(AccelStepper *st: allSteppers){
    st->disableOutputs();
  }
  memset(inputBytes, 0, sizeof(inputBytes));
  bool idle {true};
  while(idle){
    Serial.readBytes(inputBytes, BYTE_COUNT);
    if(inputBytes[0] == 0x5A){
      if(inputBytes[1] == 0x44){
        idle = !idle;
      }
    }
  }
  return;
}

void scanning(){
  for(AccelStepper *st: scanSteppers){
    st->enableOutputs();
  }
  while(true){
    Serial.readBytes(inputBytes, BYTE_COUNT);
    if(inputBytes[0] == 0x5A){
      if(inputBytes[1] == 0x44){
        return;
      }
    }
  }
}

void game_on(){
  #ifdef DEBUG
  Serial.print("game on!\n");
  #endif
  while(!Serial);
  for(AccelStepper *st: gameSteppers){
    st->enableOutputs();
  }
  scanSteppers[0]->enableOutputs();
  long target[2]{0, -StepPosY(60)};
  coreSteppers.moveTo(target);
  gameSteppers[1]->setSpeed(-500);
  while(gameSteppers[1]->distanceToGo() != 0){
    gameSteppers[1]->runSpeed();
  }

  bool game_over{false};
  int count{0};
  uint16_t absValue;
  while(!game_over){

    for(count = 0; count < 2; count++){
      positions[count] = gameSteppers[count]->currentPosition();
      absValue = (uint16_t)positions[count];
      conv16To8(absValue, sendGameBytes[1 + 2*count], sendGameBytes[2 + 2*count]);
    }
    Serial.write(sendBytes, 8);
    while(Serial.available() < 8);
    Serial.readBytes(inputBytes, BYTE_COUNT);
    if(inputBytes[0] == uint8_t(0x5B)){
      gameSteppers[0]->setSpeed(1500*(1*(!inputBytes[1])-1*(inputBytes[1]))*inputBytes[2]/255);
      gameSteppers[1]->setSpeed(-3000*(1*(!inputBytes[3])-1*(inputBytes[3]))*inputBytes[4]/255);
      #ifdef DEBUG

      sendStuffToSerial(inputBytes[1], inputBytes[4]);
      Serial.print("run!\n");
      #endif
      int test;
      if(positions[0] >= LIMITS::STEP_X || digitalRead(X_STOP)){
        if(digitalRead(X_STOP)){
          gameSteppers[0]->setSpeed(0);
        }else{
        test = positions[0] + gameSteppers[0]->speed();
          if(test >LIMITS::STEP_X){
            gameSteppers[0]->setSpeed(0);
          }
        }
      }
      if(positions[1] <= -LIMITS::STEP_Y || positions[1] >= 0){
        test = positions[1] + gameSteppers[1]->speed();
        if(test < LIMITS::STEP_Y || test > 0){
          gameSteppers[1]->setSpeed(0);
        }
      } //soft stops

      coreSteppers.run();
    
    }else if(inputBytes[0] == 0x5A){
      game_over = true;

    }
    
  }
  for(int i = 0; i<2; i++){
    target[i] = 0;
  } 
  coreSteppers.moveTo(target);
  gameSteppers[1]->runToPosition();
  gameSteppers[0]->runToPosition();
  sendBytes[1] = 0x45;
  Serial.write(sendBytes, 8);
}

void PANIC(){
  //Serial.print("KHAAAAAAAAAN!");
  //Serial.print(" Something has gone wrong - Serial has not responded for at least 10s!\n");
  sendBytes[1] = 0x44;
  Serial.write(sendBytes, 8);
}

void loop() {
  Serial.setTimeout(10000);
  #ifdef LAPTOP_MANUAL
  USBBytes[0] = '9';
  Serial.readBytes(USBBytes, BYTE_COUNT_USB);
  switch(USBBytes[0]){
    case('0'):
      idle();
      break;
    case('1'):
      scanning();
      break;
    case('2'):
      game_on();
      break;
    case('9'):
      PANIC();
  }
  #else
  while(Serial.available() < 8);
  Serial.readBytes(inputBytes, 8);
  if(inputBytes[0] == 0x5A){
    switch(inputBytes[1]){
      case 0x42:
        idle();
        break;
      case 0x41:
        scanning();
        break;
      case 0x43:
        game_on();
        break;
      case 0x44:
        break;
      case 0x45:
        break;
      default:
        PANIC();
    }
  }
  
  #endif
  for(AccelStepper *st: allSteppers){
    st->disableOutputs();
  }
}