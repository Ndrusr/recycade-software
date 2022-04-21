#include <SpeedyStepper.h>
#include <Servo.h>
#include<SharpIR.h>
#include<AccelStepper.h>

#include "pindefs.h"
// #define SCAN_STEP_PIN      26
// #define SCAN_DIR_PIN       28
// #define SCAN_ENABLE_PIN    24
// #define SCAN_CS_PIN        42
#define homeSwitch         3
#define BUFFER_SIZE       50
int inputLen;

char input[BUFFER_SIZE];

byte game_msg[8]{byte('G'), 0, 0, 0 ,0, 0, 0, byte('\n')};

Servo tiltServo;

const float homingSpeed = 400;
const float maxHomingDistanceInMM = 600;
const int directionToHome = 1;

uint16_t positions[2]{0,0};

SpeedyStepper stepper;
AccelStepper* gameSteppers[2] {new AccelStepper(1, X_STEP_PIN, X_DIR_PIN), new AccelStepper(1, Y_STEP_PIN, Y_DIR_PIN)};

void push(){
  stepper.moveToPositionInMillimeters(-540);
  stepper.moveToHomeInMillimeters(directionToHome,homingSpeed,maxHomingDistanceInMM, homeSwitch);
}

void coreCalib(){
  for (auto st: gameSteppers){
    st->enableOutputs();
    st->moveTo(-10000);
    st->setSpeed(-200);
  }
  while(digitalRead(X_STOP)){
    gameSteppers[0]->runSpeed();
  }
  gameSteppers[0]->stop();
  gameSteppers[0]->disableOutputs();

  bool yStop{false};

  while(!yStop){
    gameSteppers[1]->runSpeed();
    //add conditional here
  }

  for(auto st: gameSteppers){
    st->setCurrentPosition(0);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SCAN_ENABLE_PIN, OUTPUT);
  pinMode(SCAN_STEP_PIN, OUTPUT);
  pinMode(SCAN_DIR_PIN,OUTPUT);
  pinMode(homeSwitch,INPUT);
  pinMode(X_STOP, INPUT);
  digitalWrite(SCAN_ENABLE_PIN,LOW);

  pinMode(PEDAL_PIN, INPUT);
  pinMode(STOP_LIFT, INPUT);
  tiltServo.attach(TWIST_SERVO);
  tiltServo.write(TWIST_BTL);

  gameSteppers[0]->setEnablePin(X_ENABLE_PIN);
  gameSteppers[1]->setEnablePin(Y_ENABLE_PIN);

  gameSteppers[0]->setPinsInverted(false, false, true);
  gameSteppers[1]->setPinsInverted(true, false, true);

  int count = 0;
  for(AccelStepper *st: gameSteppers){
    st->enableOutputs();
    st->setMaxSpeed(3000);
    st->setSpeed(1500*(count+1));

    st->disableOutputs();
    count++ ;
  }
  
  // Serial.println(digitalRead(homeSwitch));
  stepper.connectToPins(SCAN_STEP_PIN,SCAN_DIR_PIN);
  
  
  stepper.setStepsPerMillimeter(5);
  stepper.setSpeedInMillimetersPerSecond(200);
  stepper.setAccelerationInMillimetersPerSecondPerSecond(1500);
  stepper.setCurrentPositionInMillimeters(0);
  stepper.moveToHomeInMillimeters(directionToHome, homingSpeed, maxHomingDistanceInMM, homeSwitch);
  delay(100);
  if(digitalRead(homeSwitch)){
    stepper.moveToHomeInMillimeters(directionToHome,homingSpeed,maxHomingDistanceInMM, homeSwitch); 
  }
  Serial.println("Homed");
  
}

game(){
  for(auto st:gameSteppers){
    st->enableOutputs();
  }
  gameSteppers[1]->moveTo(120);
  while(gameSteppers[1]->distanceToGo != 0){
    gameSteppers[1]->runSpeed();
  }
  bool game{true};
  while(game){
    positions[0] = (int)((gameSteppers[0]->currentPosition())/10);
    positions[1] = (int)((gameSteppers[1]->currentPosition())/20);
    game_msg[1] = (byte)(positions[0] & 0xff);
    game_msg[2] = (byte)((positions[0] >> 8) & 0xff);
    game_msg[3] = (byte)(positions[1] & 0xff);
    game_msg[4] = (byte)((positions[1] >> 8) & 0xff);

    Serial.write(game_msg, 8);

    while(Serial.available() < 10);
    inputLen = Serial.readBytesUntil('\n', input, BUFFER_SIZE);
    if(input[0] == byte('G') && inputLen == 9){
      gameSteppers[0]->setSpeed((float)((input[1] + (input[2] << 8) + (input[3] << 16) + (input[4] << 24)-0x80000000)/100));
      gameSteppers[1]->setSpeed((float)((input[5] + (input[6] << 8) + (input[7] << 16) + (input[8] << 24)-0x80000000)/50));
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() < 8);
  inputLen = Serial.readBytesUntil('\n', input, BUFFER_SIZE);
  if(input[0] == (byte)'Z' && input[1] == (byte)'B' && inputLen == 7){
    delay(1000);
    while(digitalRead(STOP_LIFT));
    while(!digitalRead(STOP_LIFT));
    push();
    while(Serial.available() < 8);
    inputLen = Serial.readBytesUntil('\n', input, BUFFER_SIZE);
    if(input[0] ==(byte)'Z' && input[1] == (byte)'G'){
      Serial.print("ZG10000\n");
      game();
    }else{
      Serial.print("ZG00000\n");
    }
  }else{
    Serial.println(input);
  }
  Serial.flush();
}