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
std_msgs::Bool* callbackMsg;
ros::Publisher callbackPub("teensy/callback", callbackMsg);

void pub_response(const std_msgs::Bool &debugMsg){
  debug = true;
  whichMotor = debugMsg.data;
  callbackMsg->data = debugMsg.data;
  callbackPub.publish(callbackMsg);
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
  nh_.getHardware()->setBaud(115200);

  nh_.advertise(callbackPub);
  nh_.subscribe(boolSub);
  

  pinMode(STEPPER_ACTIVATION_X, OUTPUT);
  pinMode(STEPPER_ACTIVATION_Y, OUTPUT);

  digitalWrite(STEPPER_ACTIVATION_X, LOW);
  digitalWrite(STEPPER_ACTIVATION_Y, LOW);

  xStepper.setMaxSpeed(400);
  xStepper.setSpeed(400);
  
  yStepper.setMaxSpeed(400);
  yStepper.setSpeed(400);
  
  coreSteppers.addStepper(xStepper);
  coreSteppers.addStepper(yStepper);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  debugMotors();
  nh_.spinOnce();
  
}