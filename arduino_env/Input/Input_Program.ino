#include <Q2HX711.h>

const byte hx711_data_pin = 3;
const byte hx711_clock_pin = 4;

float y1 = 20.0; // calibrated mass to be added
long x1 = 0L;
long x0 = 0L;
float avg_size = 20.0; // amount of averages for each mass measurement

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711

void setup() {
  Serial.begin(9600); // prepare serial port
  delay(1000); // allow load cell and hx711 to settle
  // tare procedure
  Serial.println("Taring...");
  for (int ii=0;ii<int(avg_size);ii++){
    delay(10);
    x0+=hx711.read();
  }
  x0/=long(avg_size);
  int ii = 1; 
}

bool mass_reading(){
  // averaging reading
  long reading = 0;
  for (int jj=0;jj<int(avg_size);jj++){
    reading+=hx711.read();
  }
  reading/=long(avg_size);
  reading -= x0;
  Serial.println(reading);
  if (reading < -20000 && reading > -45000){
    return true;
  }else{
    return false;
  }
}

void loop() {
  bool valid_input = mass_reading();
  Serial.println("Valid Input: "+String(valid_input));
}
