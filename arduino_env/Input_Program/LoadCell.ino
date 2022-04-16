long LoadCellTare(){
  // tare procedure

  long x0 = 0L;
  
  Serial.println("Taring...");
  for (int ii=0;ii<int(avg_size);ii++){
    delay(10);
    x0+=hx711.read();
  }
  x0/=long(avg_size);

  return x0;
}

bool mass_reading(long x0){
  // averaging reading
  long reading = 0;
  for (int jj=0;jj<int(avg_size);jj++){
    reading+=hx711.read();
  }
  reading/=long(avg_size);
  reading -= x0;
  Serial.println(reading);
  if (reading < -20000 && reading > -50000){
    return true;
  }else{
    return false;
  }
}
