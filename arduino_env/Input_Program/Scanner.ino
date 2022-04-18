void StepperConfig(){
  SoftSerial.begin(115200);           // initialize software serial for UART motor control
  TMCdriver.beginSerial(115200);      // Initialize UART
  
  pinMode(EN_PIN, OUTPUT);           // Set pinmodes
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);         // Enable TMC2209 board  
  stepper.connectToPins(STEP_PIN, DIR_PIN); // Initialise SpeedyStepper

  
  TMCdriver.begin();                                                                                                                                                                                                                                                                                                                            // UART: Init SW UART (if selected) with default 115200 baudrate
  TMCdriver.toff(5);                 // Enables driver in software
  TMCdriver.rms_current(900);        // Set motor RMS current
  TMCdriver.microsteps(2);         // Set microsteps to 1/2

  TMCdriver.en_spreadCycle(false);
  TMCdriver.pwm_autoscale(true);     // Needed for stealthChop

  stepper.setStepsPerMillimeter(50);     //2*200/8
  stepper.setCurrentPositionInMillimeters(0);     //Set Zero Position
  stepper.setSpeedInMillimetersPerSecond(maxSpeed);
  stepper.setAccelerationInMillimetersPerSecondPerSecond(accel);

  /*stepper.setCurrentPositionInSteps(0);
  stepper.setSpeedInStepsPerSecond(50);
  stepper.setAccelerationInStepsPerSecondPerSecond(10);*/

  stepper.moveToHomeInMillimeters(directionTowardHome, 10, maxDistanceToMoveInMillimeters, homeLimitSwitchPin);
}

float readIR(int pin){
  int sum_readings = 0;
  for(int jj=0;jj<avg_size;jj+=1){
    sum_readings+=analogRead(pin);
    delay(10);
  }

  return sum_readings/avg_size;
}

void sum_of_errors_bottle(float IR_1_readings[], float IR_2_readings[]){

  bottle_sum_e_1 = 0;
  bottle_sum_e_2 = 0;

  for(int kk = 0;kk<samples;kk++){
    bottle_sum_e_1 += 0.01*(IR_1_readings[kk]-bottleModel[0][kk])*(IR_1_readings[kk]-bottleModel[0][kk]);
    bottle_sum_e_2 += 0.01*(IR_2_readings[kk]-bottleModel[1][kk])*(IR_2_readings[kk]-bottleModel[1][kk]);
    /*Serial.print("Errors: ");
    Serial.print(bottle_sum_e_1);
    Serial.print("\t");
    Serial.println(bottle_sum_e_2);*/
  }
}

void sum_of_errors_fcan(float IR_1_readings[], float IR_2_readings[]){

  fcan_sum_e_1 = 0;
  fcan_sum_e_2 = 0;

  for(int kk = 0;kk<samples;kk++){
    fcan_sum_e_1 += 0.01*(IR_1_readings[kk]-fatCanModel[0][kk])*(IR_1_readings[kk]-fatCanModel[0][kk]);
    fcan_sum_e_2 += 0.01*(IR_2_readings[kk]-fatCanModel[1][kk])*(IR_2_readings[kk]-fatCanModel[1][kk]);
  }
}

void sum_of_errors_tcan(float IR_1_readings[], float IR_2_readings[]){
  
  tcan_sum_e_1 = 0;
  tcan_sum_e_2 = 0;
  
  for(int kk = 0;kk<samples;kk++){
    tcan_sum_e_1 += 0.01*(IR_1_readings[kk]-tallCanModel[0][kk])*(IR_1_readings[kk]-tallCanModel[0][kk]);
    tcan_sum_e_2 += 0.01*(IR_2_readings[kk]-tallCanModel[1][kk])*(IR_2_readings[kk]-tallCanModel[1][kk]);
  }
}

bool metal_check(){
  Serial.println("Metal Checking");
  bool can = false;
  lc_servo.write(70);
  delay(500);
  can = not digitalRead(inductive_sensor);
  delay(100);
  lc_servo.write(10);
  delay(500);
  return can;
}

String Scan(){
  Serial.println("Scanning...");

  for(int ii=0;ii<samples;ii++){
    stepper.moveToPositionInMillimeters(ii*150/samples);
    IR_1_Readings[ii]=min(readIR(IR_1),300);
    IR_2_Readings[ii]=min(readIR(IR_2),300);
    Serial.print(IR_1_Readings[ii]);
    Serial.print("\t");
    Serial.println(IR_2_Readings[ii]);
  }

  stepper.moveToHomeInMillimeters(directionTowardHome, 10, maxDistanceToMoveInMillimeters, homeLimitSwitchPin);
  
  sum_of_errors_bottle(IR_1_Readings,IR_2_Readings);
  sum_of_errors_fcan(IR_1_Readings, IR_2_Readings);
  sum_of_errors_tcan(IR_1_Readings, IR_2_Readings);
  float r_error=min(max(bottle_sum_e_1,bottle_sum_e_2), min(max(fcan_sum_e_1,fcan_sum_e_2), max(tcan_sum_e_1, tcan_sum_e_2)));
  Serial.print("Tall Can Error 1: ");
  Serial.print(tcan_sum_e_1);
  Serial.print("\tFat Can Error 1: ");
  Serial.print(fcan_sum_e_1);
  Serial.print("\tBottle Error 1: ");
  Serial.println(bottle_sum_e_1);
  Serial.print("Tall Can Error 2: ");
  Serial.print(tcan_sum_e_2);
  Serial.print("\tFat Can Error 2: ");
  Serial.print(fcan_sum_e_2);
  Serial.print("\tBottle Error 2: ");
  Serial.println(bottle_sum_e_2);
  Serial.println(r_error);
  if(r_error<750){
    if(metal_check()){
      return "can";
    }else{
      return "bottle";
    }
  }else{
    return "invalid";
  }
}
