void LoadCellConfig(){
  // tare procedure
  Serial.println("Taring...");
  for (int ii=0;ii<int(avg_size);ii++){
    delay(10);
    x0+=hx711.read();
  }
  x0/=long(avg_size);
  int ii = 1; 

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);

  openDoor(50);
  delay(1000);
  closeDoor(40);
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
