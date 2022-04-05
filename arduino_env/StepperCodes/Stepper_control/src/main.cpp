#include <Arduino.h>
#include <Defines.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

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
AccelStepper gameSteppers[2] = {AccelStepper(1, X_STEP_PIN, X_DIR_PIN), AccelStepper(1, Y_STEP_PIN, Y_DIR_PIN)};
AccelStepper scanSteppers[2] = {AccelStepper(1, SLIDER_STEP_PIN, SLIDER_DIR_PIN), AccelStepper(1, SCAN_STEP_PIN, SCAN_DIR_PIN)};

MultiStepper coreSteppers;
AccelStepper *allSteppers[4];

void calibMotors(){
  Serial.print("beginning calibration step\n");
  long target[2]{-100000, -100000};
  coreSteppers.moveTo(target);
  
  for(auto st : gameSteppers){
    st.enableOutputs();
    st.setSpeed(-100);
  }
  bool xStop = false, yStop = false;

  Serial.print("Beginning move\n");

  while (!(xStop && yStop)){
    coreSteppers.run();
    if(!digitalRead(X_STOP)){
      xStop = !xStop;
      Serial.print(0);
      gameSteppers[0].stop();
      gameSteppers[0].disableOutputs();
    }
    if(!digitalRead(Y_STOP)){
      Serial.print(1);
      yStop = !yStop;
      gameSteppers[0].stop();
      gameSteppers[1].disableOutputs();
    }
  }
  for(auto st: gameSteppers){
    st.setCurrentPosition(0);
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);

  while(!Serial.available());

  for(int i = 0; i < 2; i++){
    allSteppers[i] = &gameSteppers[i];
    allSteppers[i+2] = &scanSteppers[i];
  }

  Serial.print("Steppers Init");

  int activationPins[4] = {X_ENABLE_PIN, Y_ENABLE_PIN, SLIDER_ENABLE_PIN, SCAN_ENABLE_PIN};

  int count = 0;

  for(AccelStepper *stp: allSteppers){
    stp->setEnablePin(activationPins[count]);
    stp->setPinsInverted(false, false, false);
    stp->disableOutputs();
    count++;
  }

  Serial.print("Enable_set");

  count = 0;
  
  for(AccelStepper st: gameSteppers){
    st.setMaxSpeed(1000);
    st.setSpeed(400*(1*(!count)+(count)*tan(0.1309)));

    coreSteppers.addStepper(st);
    count++ ;
  }
  Serial.print(gameSteppers[0].speed());
  calibMotors();

}

void loop() {
  // put your main code here, to run repeatedly:
}