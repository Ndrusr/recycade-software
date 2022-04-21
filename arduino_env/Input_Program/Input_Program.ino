#include <Q2HX711.h>
#include <TMCStepper.h>         // TMCstepper - https://github.com/teemuatlut/TMCStepper
#include <SpeedyStepper.h>
#include <SoftwareSerial.h>     // Software serial for the UART to TMC2209 - https://www.arduino.cc/en/Reference/softwareSerial
#include <Streaming.h>          // For serial debugging output - https://www.arduino.cc/reference/en/libraries/streaming/
#include "Arduino.h"
#include <Servo.h>

// Stepper Scanner Pins

#define EN_PIN           12      // Enable - PURPLE
#define DIR_PIN          9      // Direction - WHITE
#define STEP_PIN         10      // Step - ORANGE
#define SW_SCK           11      // Software Slave Clock (SCK) - BLUE
#define SW_TX            4      // SoftwareSerial receive pin - BROWN
#define SW_RX            3      // SoftwareSerial transmit pin - YELLOW
#define DRIVER_ADDRESS   0b00   // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f           // SilentStepStick series use 0.11 ...and so does my fysetc TMC2209 (?)

SoftwareSerial SoftSerial(SW_RX, SW_TX);                          // Be sure to connect RX to TX and TX to RX between both devices

TMC2209Stepper TMCdriver(&SoftSerial, R_SENSE, DRIVER_ADDRESS);   // Create TMC driver

SpeedyStepper stepper;

int accel = 50;
long maxSpeed = 40;
int speedChangeDelay;
bool dir = false;

int homeLimitSwitchPin = A5;
long directionTowardHome = -1;
long maxDistanceToMoveInMillimeters = 200;

//Load Cell Pins

const byte hx711_data_pin = A0;
const byte hx711_clock_pin = 13;
#define dispense_pin 5

long tare = 0L;
float avg_size = 20.0; // amount of averages for each mass measurement

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711

//DC Motor Connections

#define enA 6
#define in1 7
#define in2 8
#define door_switch A4

//Serial Parameters

int incomingByte = 0;

//IR Ring

#define IR_1 A1
#define IR_2 A2
const int samples = 10;
float IR_1_Readings[samples];
float IR_2_Readings[samples];

float tallCanModel[2][samples]={{26.30, 28.55, 28.70, 29.75, 30.75, 33.90, 32.90, 33.80, 58.40, 297.15},
{29.20, 29.35, 29.40, 29.40, 29.15, 31.55, 30.35, 33.05, 38.95, 300.0}};
float bottleModel[2][samples]={{29.35, 29.45, 32.30, 32.40, 30.65, 31.15, 32.70, 31.00, 24.80, 44.05},
{39.05, 34.50, 32.30, 32.40, 32.35, 31.60, 30.00, 28.55, 28.65, 34.90}};
float fatCanModel[2][samples] = {{23.90, 25.25, 27.00, 27.30, 26.55, 38.40, 63.30, 300.0, 300.0, 300.0},
{24.55, 25.85, 28.50, 25.15, 24.65, 31.30, 119.75, 300.0, 300.0, 300.0}};

//Load Cell Servo

Servo lc_servo;

//Inductive_Sensor

#define inductive_sensor A3

//Bottle Cap detection

volatile byte cap_detected = false;
const byte cap_ir = 2;

String result;

//Errors

float bottle_sum_e_1 = 0;
float bottle_sum_e_2 = 0;
float fcan_sum_e_1 = 0;
float fcan_sum_e_2 = 0;
float tcan_sum_e_1 = 0;
float tcan_sum_e_2 = 0;

//Main Program

void cap_detect(){
  Serial.println("Cap Detected");
  cap_detected = true;
}

void setup() {

  Serial.begin(9600); // prepare serial port
  delay(1000); // allow load cell and hx711 to settle
  pinMode(cap_ir,INPUT);
  pinMode(inductive_sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(cap_ir), cap_detect, FALLING);

  tare = LoadCellTare();

  StepperConfig();
  DoorConfig();
  //Input_State_Machine machine();*/
}

void loop() {
  if(read_pedal()){
    openDoor(100);  

    while(!mass_reading(tare)){}

    closeDoor(80);

    result = Scan();
  
    if(result=="bottle" && cap_detected){
      //Serial.println("Input Verified: Bottle");
      serial_comm("accept");
      dispense();
    }else if(result=="can"){
      //Serial.println("Input Verified: Can");
      serial_comm("accept");
      dispense();
    }else{
      //Serial.println("Input Invalid");
      serial_comm("reject");
      openDoor(100);
      
      //Wait for user to remove invalid object
      while(mass_reading(tare)){}
      delay(1000);
      closeDoor(80);
    }

    //reset load cell reading and bottle_cap input
    tare = LoadCellTare();
    cap_detected = false;
  }
  //Serial.println(digitalRead(homeLimitSwitchPin));
  //Serial.println(Scan());
}
