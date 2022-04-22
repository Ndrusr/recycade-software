#include <SpeedyStepper.h>
#include <Servo.h>
#include<SharpIR.h>
#include<AccelStepper.h>
#include<ArduinoQueue.h>

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
ArduinoQueue<char> bottleCanQueue(10);

Servo tiltServo;
Servo releaseServo;
Servo bottleServo;
Servo canServo;

const float homingSpeed = 400;
const float maxHomingDistanceInMM = 600;
const int directionToHome = 1;

uint16_t positions[2]{0,0};

SpeedyStepper stepper;
AccelStepper* gameSteppers[2] {new AccelStepper(1, X_STEP_PIN, X_DIR_PIN), new AccelStepper(1, Y_STEP_PIN, Y_DIR_PIN)};

//IR Sensor
SharpIR ir_sensor(SharpIR::GP2Y0A02YK0F, A0);
const int packets = 20;
const int samples = 10;
float reading;
bool hit;
int ir_count;

void push(){
  delay(1000);
  while(digitalRead(STOP_LIFT));
  while(!digitalRead(STOP_LIFT));
  stepper.moveToPositionInMillimeters(-540);
  stepper.moveToHomeInMillimeters(directionToHome,homingSpeed,maxHomingDistanceInMM, homeSwitch);
}

float getIR(){
  reading = 0;
  for(int i=0;i<packets;i++){
    reading+=ir_sensor.getDistance();
  }

  return reading/packets;
}

bool check_hit(){
  for(int i = 0;i<samples;i++){
    if(getIR()>30){
      ir_count++;
    }

    if(ir_count>3){
      return false;
    }
  }

  return true;
}

void push_out(){
  char bOrC = bottleCanQueue.dequeue();
  if(bOrC == 'b'){
    tiltServo.write(TWIST_BTL);
    delay(1000);
    tiltServo.write(TWIST_NEU);
    releaseServo.write(20);
    delay(2000);
    releaseServo.write(110);
    bottleCanQueue.enqueue('c');
    canServo.write(25)
    delay(2000);
    canServo.write(120);

  }else{
    tiltServo.write(TWIST_CAN);
    delay(1000);
    tiltServo.write(TWIST_NEU);
    releaseServo.write(20);
    delay(2000);
    releaseServo.write(110);
    bottleCanQueue.enqueue('b');
    bottleServo.write(150)
    delay(2000);
    bottleServo.write(60);

  }
  tiltServo.write(TWIST_BTL);
  push();
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
    if(getIR()>60){
      yStop=true;
    }
  }
  gameSteppers[1]->stop();
  gameSteppers[1]->disableOutputs();

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
  pinMode(SORT_IR, INPUT);

  attachInterrupt(digitalPinToInterrupt(SORT_IR), push_out, RISING);

  digitalWrite(SCAN_ENABLE_PIN,LOW);

  //pinMode(PEDAL_PIN, INPUT);
  pinMode(STOP_LIFT, INPUT);
  tiltServo.attach(TWIST_SERVO);
  tiltServo.write(TWIST_BTL);
  releaseServo.attach(RLS_SRV);
  releaseServo.write(110);
  bottleServo.attach(BOTTLE_DISP);
  canServo.attach(CAN_DISP);


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
      gameSteppers[0]->setSpeed((float)((long(input[1]) + long(input[2] << 8) + long(input[3] << 16) + long(input[4] << 24))/100));
      gameSteppers[1]->setSpeed((float)((long(input[5]) + long(input[6] << 8) + long(input[7] << 16) + long(input[8] << 24))/50));
    }
    gameSteppers[0]->runSpeed();
    gameSteppers[1]->runSpeed();
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() < 8);
  inputLen = Serial.readBytesUntil('\n', input, BUFFER_SIZE);
  if(input[0] == (byte)'Z' && input[1] == (byte)'B' && inputLen == 7){
    
    push();
    Serial.print("ZB10000\n");
    while(Serial.available() < 8);
    inputLen = Serial.readBytesUntil('\n', input, BUFFER_SIZE);
    if(input[0] ==(byte)'Z' && input[1] == (byte)'G'){
      Serial.print("ZG10000\n");
      game();
    }else{
      Serial.print("ZG00000\n");
    }
  }else if(input[0] == (byte)'Z' && (input[1] == (byte)'b' || input[1] == (byte)'c')){
    if(input[1] == (byte)'b'){
      bottleCanQueue.enqueue('b')
    }else{
      bottleCanQueue.enqueue('c')
    }
  }else{
    Serial.println(input);
  }
  Serial.flush();
}
