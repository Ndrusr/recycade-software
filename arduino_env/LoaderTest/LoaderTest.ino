#include <SpeedyStepper.h>
#define SCAN_STEP_PIN      26
#define SCAN_DIR_PIN       28
#define SCAN_ENABLE_PIN    24
#define SCAN_CS_PIN        42
#define homeSwitch         3


const float homingSpeed = 400;
const float maxHomingDistanceInMM = 600;
const int directionToHome = 1;

SpeedyStepper stepper;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SCAN_ENABLE_PIN, OUTPUT);
  pinMode(SCAN_STEP_PIN, OUTPUT);
  pinMode(SCAN_DIR_PIN,OUTPUT);
  pinMode(homeSwitch,INPUT);
  digitalWrite(SCAN_ENABLE_PIN,LOW);

  Serial.println(digitalRead(homeSwitch));
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

void loop() {
  // put your main code here, to run repeatedly:
  stepper.moveToPositionInMillimeters(-500);
}