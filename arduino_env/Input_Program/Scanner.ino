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
  stepper.setSpeedInMillimetersPerSecond(maxSpeed);
  stepper.setAccelerationInMillimetersPerSecondPerSecond(accel);

  /*stepper.setCurrentPositionInSteps(0);
  stepper.setSpeedInStepsPerSecond(50);
  stepper.setAccelerationInStepsPerSecondPerSecond(10);*/

  stepper.moveToHomeInMillimeters(directionTowardHome, maxSpeed, maxDistanceToMoveInMillimeters, homeLimitSwitchPin);
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
  for(int ii=0;ii<150;ii+=150/samples){
    stepper.moveToPositionInMillimeters(ii);
    IR_1_Readings[ii]=readIR();
    Serial.println(IR_1_Readings[ii]);
  }
  

  stepper.moveToHomeInMillimeters(directionTowardHome, maxSpeed, maxDistanceToMoveInMillimeters, homeLimitSwitchPin);
  

}

float readIR(){
  int sum_readings = 0;
  for(int jj=0;jj<avg_size;jj+=1){
    sum_readings+=analogRead(IR_1);
    delay(10);
  }

  return sum_readings/avg_size;
}

float sum_of_errors(float IR_readings[]){

  static float r[10];
  float sum_of_e = 0;

  for(int kk = 0;kk<10;kk++){
    sum_of_e += (IR_readings[kk]-bottleModel[0][kk])*(IR_readings[kk]-bottleModel[0][kk]);
  }

  return sum_of_e;
}
