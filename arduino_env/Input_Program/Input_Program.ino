#include <Q2HX711.h>
#include <TMCStepper.h>         // TMCstepper - https://github.com/teemuatlut/TMCStepper
#include <SpeedyStepper.h>
#include <SoftwareSerial.h>     // Software serial for the UART to TMC2209 - https://www.arduino.cc/en/Reference/softwareSerial
#include <Streaming.h>          // For serial debugging output - https://www.arduino.cc/reference/en/libraries/streaming/
#include "Arduino.h"

// Stepper Scanner Pins

#define EN_PIN           12      // Enable - PURPLE
#define DIR_PIN          10      // Direction - WHITE
#define STEP_PIN         11      // Step - ORANGE
#define SW_SCK           13      // Software Slave Clock (SCK) - BLUE
#define SW_TX            4      // SoftwareSerial receive pin - BROWN
#define SW_RX            5      // SoftwareSerial transmit pin - YELLOW
#define DRIVER_ADDRESS   0b00   // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f           // SilentStepStick series use 0.11 ...and so does my fysetc TMC2209 (?)

SoftwareSerial SoftSerial(SW_RX, SW_TX);                          // Be sure to connect RX to TX and TX to RX between both devices

TMC2209Stepper TMCdriver(&SoftSerial, R_SENSE, DRIVER_ADDRESS);   // Create TMC driver

SpeedyStepper stepper;

int accel;
long maxSpeed;
int speedChangeDelay;
bool dir = false;

//Load Cell Pins

const byte hx711_data_pin = 3;
const byte hx711_clock_pin = 4;

long tare = 0L;
float avg_size = 20.0; // amount of averages for each mass measurement

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711

//Motor Connections

int enA = 9;
int in1 = 8;
int in2 = 7;
int door = 6;

//Serial Parameters

int incomingByte = 0;

//IR Ring

int IR_1 = A0;
float IR_1_Readings[10];

//Main Program

void setup() {

  Serial.begin(9600); // prepare serial port
  delay(1000); // allow load cell and hx711 to settle

  tare = LoadCellTare();
  StepperConfig();
  DoorConfig();

  //Input_State_Machine machine();
}

void loop() {
  if(read_pedal()){
    openDoor(50);  
  }
  Scan();
}
