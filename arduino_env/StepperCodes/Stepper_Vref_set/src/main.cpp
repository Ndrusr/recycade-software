#include <Arduino.h>
#include <Defines.h>

int pins[8] = {X_STEP_PIN, X_DIR_PIN, Y_STEP_PIN, Y_DIR_PIN, SCAN_STEP_PIN, SCAN_DIR_PIN, SLIDER_STEP_PIN, SLIDER_DIR_PIN};

int deactivatePins[4] = {X_ENABLE_PIN, Y_ENABLE_PIN, SCAN_ENABLE_PIN, SLIDER_ENABLE_PIN};
void setup() {
  for(size_t i = 0; i < 8; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }

  for (int a: deactivatePins){
    pinMode(a, OUTPUT);
    digitalWrite(a, LOW);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
}