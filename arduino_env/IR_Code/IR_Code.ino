#include <SharpIR.h>

SharpIR sensor(SharpIR::GP2Y0A02YK0F, A0);
const int packets = 20;
const int samples = 10;
float reading;
bool hit;
int count;
bool y_home;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  y_home = false;
  reading = 0.0;
  hit = true;
  count=0;
  for(int j=0;j<samples;j++){
    for(int i =0;i<packets;i++){
      reading+=sensor.getDistance();  
    }
    reading = reading/packets;
    //Serial.println(reading);
    if(reading>30){
      count++;
    }

    if(reading>60){
      y_home = true;
      break;
    }
    if(count>3){
      hit = false;
      break;
    }
  }
  if(y_home){
    Serial.println("HOME");
  }else if(hit){
    Serial.println("HIT!!");
  }else{
    Serial.println("CLEAR");
  }
}
