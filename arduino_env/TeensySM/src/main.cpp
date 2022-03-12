#include <Arduino.h>
#include"StateMachine.h"

StateMachine::RecSM recycade;

const char* debug;

void setup() {
  // put your setup code here, to run once:
  debug = recycade.debugState();
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}