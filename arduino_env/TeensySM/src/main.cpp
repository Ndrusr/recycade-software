#include <Arduino.h>
#include"StateMachine.h"

void setup() {
  // put your setup code here, to run once:
  StateMachine::RecSM recycade;
  auto debug = recycade.debugState();
  Serial.begin(9600);
  if(debug != nullptr)
    Serial.print(*debug);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}