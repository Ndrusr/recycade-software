#include <Q2HX711.h>
#include <TMCStepper.h>         // TMCstepper - https://github.com/teemuatlut/TMCStepper
#include <SpeedyStepper.h>
#include <SoftwareSerial.h>     // Software serial for the UART to TMC2209 - https://www.arduino.cc/en/Reference/softwareSerial
#include <Streaming.h>          // For serial debugging output - https://www.arduino.cc/reference/en/libraries/streaming/
#include "Arduino.h"

// Stepper Scanner Pins

#define EN_PIN           11      // Enable - PURPLE
#define DIR_PIN          8      // Direction - WHITE
#define STEP_PIN         9      // Step - ORANGE
#define SW_SCK           10      // Software Slave Clock (SCK) - BLUE
#define SW_TX            3      // SoftwareSerial receive pin - BROWN
#define SW_RX            2      // SoftwareSerial transmit pin - YELLOW
#define DRIVER_ADDRESS   0b00   // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f           // SilentStepStick series use 0.11 ...and so does my fysetc TMC2209 (?)

SoftwareSerial SoftSerial(SW_RX, SW_TX);                          // Be sure to connect RX to TX and TX to RX between both devices

TMC2209Stepper TMCdriver(&SoftSerial, R_SENSE, DRIVER_ADDRESS);   // Create TMC driver

SpeedyStepper stepper;

int accel = 5;
long maxSpeed = 5;
int speedChangeDelay;
bool dir = false;

int homeLimitSwitchPin = A5;
long directionTowardHome = -1;
long maxDistanceToMoveInMillimeters = 200;

//Load Cell Pins

const byte hx711_data_pin = 13;
const byte hx711_clock_pin = 12;
#define dispense_pin 5

long tare = 0L;
float avg_size = 20.0; // amount of averages for each mass measurement

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711

//DC Motor Connections

#define enA 6
#define in1 4
#define in2 7
#define door_switch A4

//Serial Parameters

int incomingByte = 0;

//IR Ring

#define IR_1 A0
const int samples = 10;
float IR_1_Readings[samples];
float IR_2_Readings[samples];
float IR_3_Readings[samples];
float bottleModel[3][samples];
float canModel[3][samples];

//Main Program

void setup() {

  Serial.begin(9600); // prepare serial port
  delay(1000); // allow load cell and hx711 to settle

  tare = LoadCellTare();

  StepperConfig();
  DoorConfig();

  //Input_State_Machine machine();*/
}

void loop() {
  if(read_pedal()){
    openDoor(50);  
  }
  Serial.println(analogRead(A3));
  //Scan();

  //Serial.println(mass_reading());
  //Serial.println(mass_reading(tare));
}
