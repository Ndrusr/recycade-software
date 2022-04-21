#include <Servo.h>

Servo sorter_release;
Servo sorter_tilt;
Servo can_dispense;
Servo bottle_dispense;

#define ST_PIN 4
#define SR_PIN 11
#define BOTTLE_PIN 5
#define CAN_PIN 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sorter_release.attach(SR_PIN);
  sorter_tilt.attach(ST_PIN);
  can_dispense.attach(CAN_PIN);
  bottle_dispense.attach(BOTTLE_PIN);
  
}

void load_can(){
  //can_dispense.write(120);
  //delay(500);
  can_dispense.write(25);//down
  delay(2000);
  can_dispense.write(120);//up
  delay(500);
}

void load_bottle(){
  //bottle_dispense.write(60);
  //delay(500);
  bottle_dispense.write(150);//down
  delay(2000);
  bottle_dispense.write(60);//up
  delay(500);
}

void sort_bottle(){
  sorter_tilt.write(15);
  delay(500);
  sorter_release.write(20);//down
  delay(1000);
  sorter_release.write(110);//up
  delay(500);
}

void sort_can(){
  sorter_tilt.write(40);
  delay(1000);
  sorter_tilt.write(15);
  delay(500);
  sorter_release.write(20);//down
  delay(2000);
  sorter_release.write(110);//up
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Looping");

  sorter_tilt.write(0);
  delay(1000);
  /*sorter_release.write(20);
  delay(1000);
  sorter_release.write(110);
  delay(1000);*/
  sort_bottle();
  delay(2000);
  
}
