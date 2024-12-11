/**
 * "Squawkers McGraw" Animatronic Controller
 * Copyright (c) 2023 Playful Technology 
 */

// INCLUDES
// For debouncing button input. See https://github.com/LennartHennigs/Button2
#include "Button2.h"
#include <U8g2lib.h>
// using the ESP32 built-in bluetooth, so no need for serial Bluetooth connection to, e.g. HC-05
#include "BluetoothSerial.h"

// ENUMS
// Define the states through which the parrot's eyes and body moves
enum EyeState { Open, Closing, Neutral, Closed, Opening };
enum BodyState { bdyNeutral, bdyFlapWingsHeadDown, bdyLookRightHeadUp, bdyHeadLeft, bdyNeutralFlapWing };

// CONSTANTS
// Outpin pins
// TP6612 motor driver has 3 pins per channel. IN1/IN2/PWM
byte bodyMotorPins[] = {27, 26, 25}; // White/Grey wires
byte headMotorPins[] = {12, 14, 13}; // Blue/Purple wires
// Sensor pins
const byte numSensors = 4;
const byte tonguePin = 36; // Blue wire to head
const byte eyesClosedPin = 39; // Orange wire
const byte eyesOpenPin = 34; // Purple wire
const byte legPin = 35; // Blue wire
const byte sensorPins[numSensors] = {tonguePin, eyesClosedPin, eyesOpenPin, legPin};

// GLOBALS
Button2 sensors[numSensors];
#ifdef ESP32
  BluetoothSerial BTSerial;
#else
// Initialize Serial2 for Bluetooth communication on GPIO 16 (Rx) and GPIO 17 (Tx)
// HardwareSerial BTSerial(2);  // Use Serial2 for Bluetooth
#endif
// Initialize the U8g2 library with I2C connection on GPIO 32 (SDA) and GPIO 33 (SCL)
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 33, /* data=*/ 32);
// Keep track of current position
int8_t lastBodyMovement = 0;
bool wingsOpen = true;
EyeState eyeState = EyeState::Neutral;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(__FILE__ __DATE__);

  // Initialize the OLED display
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_simple1_tr); // Choose a font size that fits your display
  
  // Initialize Bluetooth Serial
  #ifdef ESP32
    // Built-in Bluetooth interface needs only advertised name
    BTSerial.begin("SquawkersMcGraw");
  #else
    // If using an external Bluetooth module (e.g. HC-05), specify the parameters
    // of the serial interface to it - baud rate, Rx, Tx pin
    BTSerial.begin(9600, SERIAL_8N1, 8, 9);
  #endif

  // Display welcome message on the OLED
  u8g2.drawStr(0, 10, "Waiting for BT data...");
  u8g2.sendBuffer();

  // Motors
  for(int i=0; i<3; i++){
    pinMode(headMotorPins[i], OUTPUT);  
    pinMode(bodyMotorPins[i], OUTPUT);
  }
  // Set PWM speed for motor movements
  digitalWrite(headMotorPins[2], HIGH);
  digitalWrite(bodyMotorPins[2], HIGH);
  
  // Sensors
  for(int i=0; i<numSensors; i++){
    pinMode(sensorPins[i], INPUT_PULLUP);
    sensors[i].begin(sensorPins[i]);
    sensors[i].setDebounceTime(1);
    sensors[i].setChangedHandler(sensorChanged);
  }

  openEyes();
  delay(500);
  calibrateBody();
}

void sensorChanged(Button2& sensor) {
  Serial.print("Sensor changed: ");
  if(sensor.getPin() == tonguePin) {
    Serial.println(sensor.isPressed() ? "Tongue pressed" : "Tongue released");    
  }
  else if(sensor.getPin() == legPin) {
    Serial.println(sensor.isPressed() ? "Leg pin pressed" : "Leg pin released");
    stopMoving();
  }
  else if(sensor.getPin() == eyesOpenPin || sensor.getPin() == eyesClosedPin) {
    eyeState = getEyeState();
  }

  // DEBUG - print current status of all sensors  
  for(int i=0; i<numSensors; i++){
    Serial.print(digitalRead(sensorPins[i]));
    Serial.print(i==numSensors-1 ? "\r\n" : ",");
  }
  // TESTING!
  // For stepping through head motor movement
  //stopEyes();
}

EyeState getEyeState(){
  if(digitalRead(eyesClosedPin) && !digitalRead(eyesOpenPin)){
      Serial.println("Eyes Open");
      return EyeState::Open;
    }
    else if(digitalRead(eyesClosedPin) && digitalRead(eyesOpenPin)){
      Serial.println("Eyes Neutral");
      return EyeState::Neutral;
    }
    else if(!digitalRead(eyesClosedPin) && digitalRead(eyesOpenPin)){
      Serial.println("Eyes Closed");
      return EyeState::Closed;
    }
    // Default
    Serial.print("ERROR: Could not determine eye state");
    return EyeState::Neutral;
}

void openEyes(){
  Serial.println(F("Open Eyes"));  
  u8g2.drawStr(0, 10, "Opening eyes...");
  u8g2.sendBuffer();
  unsigned long now = millis();
  // Open eyes one step
  while((millis() - now < 250) && (eyeState != EyeState::Open)){
    digitalWrite(headMotorPins[0], LOW);
    // 0-255. Set higher values to increase speed
    digitalWrite(headMotorPins[2], HIGH);
    digitalWrite(headMotorPins[1], HIGH);
   // eyeState = EyeState::Opening;
  }
  stopEyes();
}
void closeEyes(){
  Serial.println(F("Close Eyes"));  
  u8g2.drawStr(0, 10, "Closing Eyes...");
  u8g2.sendBuffer();
  unsigned long now = millis();
  while((millis() - now < 100) && (eyeState != EyeState::Closed)){
    digitalWrite(headMotorPins[0], HIGH);
    // 0-255 Set lower values to increase speed
    digitalWrite(headMotorPins[1], LOW);
    analogWrite(headMotorPins[2], 64);
    //eyeState = EyeState::Closing;
  }
  stopEyes();
}
void stopEyes(){
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(headMotorPins[2], LOW);
}
/**
/* Note: BLINKING is the opposite of TALKING
 */
void blink(){
  Serial.print(F("Blinking"));  
  u8g2.drawStr(0, 10, "Blinking...");
  u8g2.sendBuffer();
  unsigned long now = millis();
  // Close eyes all the way
  while((millis() - now < 200) && (eyeState != EyeState::Closed)){
    digitalWrite(headMotorPins[0], HIGH);
    digitalWrite(headMotorPins[1], LOW);
    digitalWrite(headMotorPins[2], 64);
  }
  // Open eyes all the way
  while((millis() - now < 400) && (eyeState != EyeState::Open)){
    digitalWrite(headMotorPins[0], LOW);
    digitalWrite(headMotorPins[1], HIGH);
    digitalWrite(headMotorPins[2], 64);
  }
  stopEyes();
}

// Spinning head motor in one direction opens mouth
// (will also open eyes)
void openMouth(){
  Serial.println(F("Open Mouth"));
  u8g2.drawStr(0, 10, "Opening Mouth");
  u8g2.sendBuffer();  
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], HIGH);
  digitalWrite(headMotorPins[2], HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}
// Mouth is sprung so will automatically close when head motor is unpowered
void closeMouth(){
  Serial.println(F("Close Mouth"));
  u8g2.drawStr(0, 10, "Closing Mouth...");
  u8g2.sendBuffer();
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(headMotorPins[2], LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void talk(){
  Serial.print(F("Talking"));  
  unsigned long now = millis();
  EyeState previousEyeState = eyeState;
  // Open mouth (and eyes)
  while((millis() - now < 100)){
    digitalWrite(headMotorPins[0], LOW);
    digitalWrite(headMotorPins[1], HIGH);
    digitalWrite(headMotorPins[2], HIGH);
  }
  // Close mouth again until returned to previous eye state
  while((millis() - now < 200 && eyeState != previousEyeState)){
    digitalWrite(headMotorPins[0], HIGH);
    digitalWrite(headMotorPins[1], LOW);
    digitalWrite(headMotorPins[2], HIGH);
  }
  // Stop
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(headMotorPins[2], LOW);

}

// Cycle through movement patterns in specified direction
//<0   1>
//Mouth Open (no sensor) <> Eyes Open <> Neutral Eyes <> Eyes Closed <> Eyes straining to be _really_ closed (from this state, 0 goes back to OPEN though, not neutral....)

void moveHead(int dir=0){
  Serial.println(F("Move head"));
  u8g2.drawStr(0, 10, "Moving...");
  u8g2.sendBuffer();
  digitalWrite(headMotorPins[0], dir);
  digitalWrite(headMotorPins[1], 1-dir);
  digitalWrite(headMotorPins[2], 128);
}

/*
A <- MOVEMENT A <-Slight Wiggle -> MOVEMENT B -> B -> B
|         ^
\----------
*/
// Cycle through movement patterns in specified direction
void moveBody(int dir=0){
  Serial.println(F("Move"));
  u8g2.drawStr(0, 10, "Moving...");
  u8g2.sendBuffer();
  digitalWrite(bodyMotorPins[0], dir);
  digitalWrite(bodyMotorPins[1], 1-dir);
  digitalWrite(bodyMotorPins[2], HIGH);
  lastBodyMovement = dir;
  wingsOpen = dir;
}
// Stop all body movement
void stopMoving(){
  Serial.println(F("Stop"));  
  u8g2.drawStr(0, 10, "Stopping...");
  u8g2.sendBuffer();
  digitalWrite(bodyMotorPins[0], LOW);
  digitalWrite(bodyMotorPins[1], LOW);
  digitalWrite(bodyMotorPins[2], LOW);
}
// To flap wings, we move body in the opposite direction to last time it was moved
void flap(){
  moveBody(1-lastBodyMovement);
}
// These are more limited versions of flap()
void shutWings(){
  if(wingsOpen){
    moveBody(0);
  }
}
// These are more limited versions of flap()
void openWings(){
  if(!wingsOpen){
    moveBody(1);
  }
}
// Move the body so we know the position it ends in
void calibrateBody(){
  moveBody(0);
}
void loop() {
  // Note that responding to digital button inputs and handling sensor changes
  // are done in callback methods above (btnPressed, btnReleased, and sensorChanged)
  // so are not in loop()
  // For debugging, we will also accept commands sent over the Serial connection
  // Read any input
  char command;
  bool cmdReceived = false;
  if(Serial.available() > 0){
    // Read any single character sent
    command = Serial.read();
    cmdReceived = true;
  }
  // Check if data is available on Bluetooth
  else if(BTSerial.available()) {
    command = BTSerial.read();
    cmdReceived = true;
  }
  if(cmdReceived){
    switch(command){
      case 'f':
        flap();
        break;
      case 'b':
        blink();
        break;
      case 'z':
        moveBody(0);
        break;
      case 'x':
        moveBody(1);
        break;
      case 'o':
        openWings();
        break;
      case 's':
        shutWings();
        break;
      case 't':
        talk();
        break;
      case '1':
        moveHead(0);
        break;
      case '2':
        moveHead(1);
        break;  
      case '3':
        openMouth();
        delay(100);
        closeMouth();
        break;
      case '4':
        blink();
        break;
      case '5':
        openEyes();
        break;
      case '6':
        closeEyes();
        break;
      case '7':
        moveBody(0);
        break;
      case '8':
        moveBody(1);
        break;
    }
  }
  for(int i=0; i<numSensors; i++){ sensors[i].loop(); }
}
