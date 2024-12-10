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
EyeState eyeState = EyeState::Neutral;
enum BodyState { bdyNeutral, bdyFlapWingsHeadDown, bdyLookRightHeadUp, bdyHeadLeft, bdyNeutralFlapWing };

// CONSTANTS
// Outpin pins
byte bodyMotorPins[] = {27, 26, 25}; // White/Grey wires
byte headMotorPins[] = {12, 14, 13}; // Blue/Purple wires
// Sensor pins
const byte numSensors = 4;
const byte tonguePin = 36; // Blue wire to head
const byte eyesClosedPin = 39; // Orange wire
const byte eyesOpenPin = 34; // Purple wire
const byte legPin = 35; // Blue wire
const byte sensorPins[numSensors] = {tonguePin, eyesClosedPin, eyesOpenPin, legPin};
// Input pins
/*
const byte numButtons = 2;
const byte btnPins[] = {A2, A5}; // Top button, Joystick click-in button
const byte joystickPins[] = {A3, A4};
*/
// GLOBALS
//Button2 buttons[numButtons];
Button2 sensors[numSensors];
// Initialize Serial1 for Bluetooth communication on GPIO 16 (Rx) and GPIO 17 (Tx)
//HardwareSerial BTSerial(2);  // Use Serial2 for Bluetooth
BluetoothSerial BTSerial;
// Initialize the U8g2 library with I2C connection on GPIO 32 (SDA) and GPIO 33 (SCL)
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 33, /* data=*/ 32);
int8_t lastBodyMovement = 0;

bool wingsOpen = true;

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__ __DATE__);

  // Initialize the OLED display
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_simple1_tr); // Choose a font size that fits your display
  
  // Initialize Bluetooth Serial
  //BTSerial.begin(115200, SERIAL_8N1, 16, 17); // Baud rate 9600, Rx=16, Tx=17
  BTSerial.begin("SquawkersMcGraw");

  // Display welcome message on the OLED
  u8g2.drawStr(0, 10, "Waiting for BT data...");
  u8g2.sendBuffer();

  // Motors
  for(int i=0; i<3; i++){
    pinMode(headMotorPins[i], OUTPUT);  
    pinMode(bodyMotorPins[i], OUTPUT);
  }
  digitalWrite(headMotorPins[2], HIGH);
  digitalWrite(bodyMotorPins[2], HIGH);
  
  /*
  // Buttons
  for(int i=0; i<numButtons; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    buttons[i].begin(btnPins[i]);
    buttons[i].setPressedHandler(btnPressed);
    buttons[i].setReleasedHandler(btnReleased);
  }
  */
  // Sensors
  for(int i=0; i<numSensors; i++){
    pinMode(sensorPins[i], INPUT_PULLUP);
    sensors[i].begin(sensorPins[i]);
    sensors[i].setDebounceTime(1);
    sensors[i].setChangedHandler(sensorChanged);
  }
  // We don't know what state the eyes are in when we first startup
  calibrateEyes();

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
    if(digitalRead(eyesClosedPin) && !digitalRead(eyesOpenPin)){
      Serial.println("Eyes Open");
      eyeState = EyeState::Open;
    }
    else if(digitalRead(eyesClosedPin) && digitalRead(eyesOpenPin)){
      Serial.println("Eyes Neutral");
      eyeState = EyeState::Neutral;
    }
    else if(!digitalRead(eyesClosedPin) && digitalRead(eyesOpenPin)){
      Serial.println("Eyes Closed");
      eyeState = EyeState::Closed;
    }
    stopEyes();  
  }

  // DEBUG - print current status of all sensors  
  for(int i=0; i<numSensors; i++){
    Serial.print(digitalRead(sensorPins[i]));
    Serial.print(i==numSensors-1 ? "\r\n" : ",");
  }
}

// TODO
// This fails if eyes are in closed state when called
void calibrateEyes(){
  Serial.print(F("Calibrating eyes..."));
  unsigned long now = millis();
  digitalWrite(headMotorPins[0], HIGH);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(headMotorPins[2], HIGH);
  // Attempt to calibrate for a maximum of 2 seconds
  while(millis() - now < 2000){
    int closed = !digitalRead(eyesClosedPin);
    int open = !digitalRead(eyesOpenPin);
    if(!closed && !open) {
      stopEyes();
      Serial.print(F("done!"));      
      return;
    }
  }
  stopEyes();
  Serial.print(F("timeout"));
}
void openEyes(){
  Serial.println(F("Open Eyes"));  
  u8g2.drawStr(0, 10, "Opening eyes...");
  u8g2.sendBuffer();
  if(eyeState != EyeState::Open) {
    digitalWrite(headMotorPins[0], LOW);
    // 0-255. Set higher values to increase speed
    digitalWrite(headMotorPins[2], HIGH);
    digitalWrite(headMotorPins[1], HIGH);
    eyeState = EyeState::Opening;
  }
}
void closeEyes(){
  Serial.println(F("Close Eyes"));  
  u8g2.drawStr(0, 10, "Closing Eyes...");
  u8g2.sendBuffer();
  if(eyeState != EyeState::Closed) {
    digitalWrite(headMotorPins[0], HIGH);
    // 0-255 Set lower values to increase speed
    digitalWrite(headMotorPins[1], LOW);
    analogWrite(headMotorPins[2], 64);
    eyeState = EyeState::Closing;
  }
}
void stopEyes(){
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(headMotorPins[2], LOW);
}
void blink(){
  Serial.print(F("Blinking"));  
  u8g2.drawStr(0, 10, "Blinking...");
  u8g2.sendBuffer();
  unsigned long now = millis();
  digitalWrite(headMotorPins[0], HIGH);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(headMotorPins[2], HIGH);
  while(millis() - now < 1000){
    int closed = !digitalRead(eyesClosedPin);
    int open = !digitalRead(eyesOpenPin);
    if(!closed && !open) {
      stopEyes();      
      return;
    }
  }
  stopEyes();
}
/*
void btnPressed(Button2& btn) {
  // Top button
  if(btn == buttons[0]){
    Serial.println("Btn[0] pressed");
    openMouth();
  }
  // Joystick click button
  else if(btn == buttons[1]){    
    Serial.println("Btn[1] pressed");
    blink();
  }
}
void btnReleased(Button2& btn) {
  // Top button
  if(btn == buttons[0]){
    Serial.println("Btn[0] released");
    closeMouth();
  }
  // Joystick click button
  else if(btn == buttons[1]){    
    Serial.println("Btn[1] released");
  }
}
*/
// Spinning head motor in one direction opens mouth
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
void flap(){
  moveBody(1-lastBodyMovement);
}
void shutWings(){
  if(wingsOpen){
    moveBody(0);
  }
}
void openWings(){
  if(!wingsOpen){
    moveBody(1);
  }
}
void calibrateBody(){
  moveBody(0);
}
void loop() {


  // Note that responding to digital button inputs and handling sensor changes
  // are done in callback methods above (btnPressed, btnReleased, and sensorChanged)
  // so are not in loop()

  // Handle Joystick Input
  /*
  if(analogRead(joystickPins[0]) < 100) {
    Serial.println("Down");
    closeEyes();
  }
  else if(analogRead(joystickPins[0]) > 900) {
    Serial.println("Up");    
    openEyes();
  }
  if(analogRead(joystickPins[1]) < 100) {
    //Serial.println("Left");
    moveBody(0);
  }
  else if(analogRead(joystickPins[1]) > 900) {
    //Serial.println("Right");
    moveBody(1);
  }
  */
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
        openMouth();
        delay(100);
        closeMouth();
        break;
      case '0':
        calibrateEyes();
        break;
      case '1':
        openMouth();
        break;
      case '2':
        closeMouth();
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
  //for(int i=0; i<numButtons; i++){ buttons[i].loop(); }
  for(int i=0; i<numSensors; i++){ sensors[i].loop(); }
}
