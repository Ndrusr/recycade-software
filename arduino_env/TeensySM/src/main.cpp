#define USE_TEENSY_HW_SERIAL

#include <Arduino.h>
#include"Defines.h"

#include<AccelStepper.h>
#include<MultiStepper.h>

#include<ros.h>
#include<std_msgs/Bool.h>

AccelStepper xStepper(1, STEPPER_STEP_X, STEPPER_DIR_X);
AccelStepper yStepper(1, STEPPER_STEP_Y, STEPPER_DIR_Y);
MultiStepper coreSteppers;
bool debug{false};

ros::NodeHandle nh_;



bool whichMotor;

void scanning(){
  Serial1.begin(9600);
  
}

void game(){
  
}

void pub_response(const std_msgs::Bool &debugMsg){
  debug = true;
  whichMotor = debugMsg.data;
}

ros::Subscriber<std_msgs::Bool> boolSub("teensy/chatter", &pub_response);

void debugMotors(){
  
  if(debug){
    if(whichMotor){
      digitalWrite(STEPPER_ACTIVATION_X, HIGH);
      digitalWrite(STEPPER_ACTIVATION_Y, LOW);
      xStepper.runSpeed();

    }else{
      digitalWrite(STEPPER_ACTIVATION_Y, HIGH);
      digitalWrite(STEPPER_ACTIVATION_X, LOW);
      yStepper.runSpeed();
    }
    
    
    // Serial.print("Running motors");
    // delay(1000);
  }
}



void setup() {
  
  nh_.initNode();

  pinMode(STEPPER_ACTIVATION_X, OUTPUT);
  pinMode(STEPPER_ACTIVATION_Y, OUTPUT);

  xStepper.setMaxSpeed(400);
  xStepper.setSpeed(400);
  
  yStepper.setMaxSpeed(400);
  yStepper.setSpeed(400);
  
  coreSteppers.addStepper(xStepper);
  coreSteppers.addStepper(yStepper);

  nh_.subscribe(boolSub);
}

void loop() {
  // put your main code here, to run repeatedly:
  debugMotors();
  nh_.spinOnce();
  
}