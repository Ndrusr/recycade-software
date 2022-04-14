void StepperConfig(){
  SoftSerial.begin(115200);           // initialize software serial for UART motor control
  TMCdriver.beginSerial(115200);      // Initialize UART
  
  pinMode(EN_PIN, OUTPUT);           // Set pinmodes
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);         // Enable TMC2209 board  

  TMCdriver.begin();                                                                                                                                                                                                                                                                                                                            // UART: Init SW UART (if selected) with default 115200 baudrate
  TMCdriver.toff(5);                 // Enables driver in software
  TMCdriver.rms_current(700);        // Set motor RMS current
  TMCdriver.microsteps(256);         // Set microsteps

  TMCdriver.en_spreadCycle(false);
  TMCdriver.pwm_autoscale(true);     // Needed for stealthChop
}

void Scan(){
  accel = 1000;                                         // Speed increase/decrease amount
  maxSpeed = 50000;                                      // Maximum speed to be reached
  speedChangeDelay = 100;                                // Delay between speed changes
  
  for (long i = 0; i <= maxSpeed; i = i + accel){             // Speed up to maxSpeed
    TMCdriver.VACTUAL(i);                                     // Set motor speed
    Serial << TMCdriver.VACTUAL() << endl;
    delay(100);
  }
  
  for (long i = maxSpeed; i >=0; i = i - accel){              // Decrease speed to zero
    TMCdriver.VACTUAL(i);
    Serial << TMCdriver.VACTUAL() << endl;
    delay(100);
  }  
  
  dir = !dir; // REVERSE DIRECTION
  TMCdriver.shaft(dir); // SET DIRECTION

}
