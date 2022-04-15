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
  TMCdriver.rms_current(600);        // Set motor RMS current
  TMCdriver.microsteps(2);         // Set microsteps to 1/2

  TMCdriver.en_spreadCycle(false);
  TMCdriver.pwm_autoscale(true);     // Needed for stealthChop

  stepper.setStepsPerMillimeter(50);     //2*200/8
  stepper.setCurrentPositionInMillimeters(0);     //Set Zero Position
  stepper.setSpeedInMillimetersPerSecond(5);
  stepper.setAccelerationInMillimetersPerSecondPerSecond(5);

  /*stepper.setCurrentPositionInSteps(0);
  stepper.setSpeedInStepsPerSecond(50);
  stepper.setAccelerationInStepsPerSecondPerSecond(10);*/
}

void Scan(){
  Serial.println("Scanning...");
  
  /*accel = 1000;                                         // Speed increase/decrease amount
  maxSpeed = 50000;                                      // Maximum speed to be reached
  speedChangeDelay = 100;                                // Delay between speed changes

  TMCdriver.shaft(dir); // SET DIRECTION
  
  for (long i = 0; i <= maxSpeed; i = i + accel){             // Speed up to maxSpeed
    TMCdriver.VACTUAL(i);                                     // Set motor speed
    Serial << TMCdriver.VACTUAL() << endl;
    delay(speedChangeDelay);
  }

  TMCdriver.VACTUAL(maxSpeed);
  
  for (long i = maxSpeed; i >=0; i = i - accel){              // Decrease speed to zero
    TMCdriver.VACTUAL(i);
    Serial << TMCdriver.VACTUAL() << endl;
    delay(100);
  }*/
  for(int ii=0;ii<10;ii+=1){
    stepper.moveToPositionInMillimeters(ii*5);
    IR_1_Readings[ii]=readIR();
    Serial.println(IR_1_Readings[ii]);
  }
  

  stepper.moveToPositionInSteps(0);
  delay(1000);
  

}

float readIR(){
  int sum_readings = 0;
  for(int jj=0;jj<avg_size;jj+=1){
    sum_readings+=analogRead(IR_1);
    delay(10);
  }i
  

  return sum_readings/avg_size;
}
