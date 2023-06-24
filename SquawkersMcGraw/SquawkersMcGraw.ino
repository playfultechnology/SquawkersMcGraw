/**
 * "Squawkers MGraw" Animatronic Controller
 */

// INCLUDES
#include "Button2.h"

enum EyeState {Open, Closing, Neutral, Closed, Opening};
EyeState eyeState = EyeState::Neutral;

// Movement patterns
// Neutral, (FlapWingsHeadDown), (lookRightHeadUp), (HeadLeft), NeutralFlapWing

// CONSTANTS
// Outpin pins
byte headMotorPins[] = {4, 5}; // Blue/Purple
byte bodyMotorPins[] = {6, 7};
// Sensor pins
const byte numSensors = 4;
const byte tonguePin = 8; // Blue wire to head
const byte eyesClosedPin = 10; // Orange wire
const byte eyesOpenPin = 11; // Purple wire
const byte legPin = 12;
const byte sensorPins[numSensors] = {tonguePin, eyesClosedPin, eyesOpenPin, legPin};
// Input pins
const byte numButtons = 2;
const byte btnPins[] = {A2, A5}; // Top button, Joystick click-in button
const byte joystickPins[] = {A3, A4};

// GLOBALS
Button2 buttons[numButtons];
Button2 sensors[numSensors];

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__ __DATE__);

  // Motors
  for(int i=0; i<2; i++){
    pinMode(headMotorPins[i], OUTPUT);  
    pinMode(bodyMotorPins[i], OUTPUT);
  }
  // Buttons
  for(int i=0; i<numButtons; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    buttons[i].begin(btnPins[i]);
    buttons[i].setPressedHandler(btnPressed);
    buttons[i].setReleasedHandler(btnReleased);
  }
  // Sensors
  for(int i=0; i<numSensors; i++){
    pinMode(sensorPins[i], INPUT_PULLUP);
    sensors[i].begin(sensorPins[i]);
    sensors[i].setDebounceTime(1);
    sensors[i].setChangedHandler(sensorChanged);
  }

  calibrateEyes();
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

void calibrateEyes(){
  Serial.print(F("Calibrating eyes..."));
  unsigned long now = millis();
  digitalWrite(headMotorPins[0], HIGH);
  analogWrite(headMotorPins[1], LOW);
  // Attempt to calibrate for a maximum of 5 seconds
  while(millis() - now < 5000){
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
  if(eyeState != EyeState::Open) {
    digitalWrite(headMotorPins[0], LOW);
    // 0-255. Set higher values to increase speed
    analogWrite(headMotorPins[1], 192);
    eyeState = EyeState::Opening;
  }
}
void closeEyes(){
  Serial.println(F("Close Eyes"));
    if(eyeState != EyeState::Closed) {
    digitalWrite(headMotorPins[0], HIGH);
    // 0-255 Set lower values to increase speed
    analogWrite(headMotorPins[1], 64);
    eyeState = EyeState::Closing;
  }
}
void stopEyes(){
  analogWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
}
void blink(){
  Serial.print(F("Blinking"));
  unsigned long now = millis();
  digitalWrite(headMotorPins[0], HIGH);
  analogWrite(headMotorPins[1], LOW);
  // Attempt to calibrate for a maximum of 5 seconds
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

void openMouth(){
  Serial.println(F("Open Mouth"));
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
}

void closeMouth(){
  Serial.println(F("Close Mouth"));
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

// Ends with head low and to the side
void moveBody(int dir=0){
  Serial.println(F("Move"));
  digitalWrite(bodyMotorPins[0], dir);
  digitalWrite(bodyMotorPins[1], 1-dir);
}

void stopMoving(){
  Serial.println(F("Stop"));
  digitalWrite(bodyMotorPins[0], LOW);
  digitalWrite(bodyMotorPins[1], LOW);
}

void loop() {
  // Read any input
  char command;
  bool cmdReceived = false;
  if(Serial.available() > 0){
    // Read any integer sent
    command = Serial.read();
    cmdReceived = true;
  }

  if(analogRead(joystickPins[0]) < 100) {
    Serial.println("Down");
    closeEyes();
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if(analogRead(joystickPins[0]) > 900) {
    Serial.println("Up");    
    openEyes();
  }
  else {
  }
  
  if(analogRead(joystickPins[1]) < 100) {
    //Serial.println("Left");
    moveBody(0);
  }
  else if(analogRead(joystickPins[1]) > 900) {
    //Serial.println("Right");
    moveBody(1);
  }
  else {
    ;
  }
  
  if(cmdReceived){
    switch(command){
      case '0':
        calibrateEyes();
        break;
      case '3':
        break;
      case '4':
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
  for(int i=0; i<numButtons; i++){ buttons[i].loop(); }
  for(int i=0; i<numSensors; i++){ sensors[i].loop(); }
}