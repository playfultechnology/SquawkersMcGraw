/**
 * "Squawkers MGraw" Animatronic Controller
 */

// INCLUDES
#include "Button2.h"

// Movement patterns
// Neutral, (FlapWingsHeadDown), (lookRightHeadUp), (HeadLeft), NeutralFlapWing

// CONSTANTS
byte headMotorPins[] = {4, 5}; // Blue/Purple
byte bodyMotorPins[] = {6, 7};
byte numInputs = 5;
byte inputPins[] = {A1, A2, A3, A4, A5};
byte numSensors = 5;
const byte tonguePin = 8; // Blue wire to head
const byte eyesClosedPin = 10; // Orange wire
const byte eyesOpenPin = 11; // Purple wire
const byte legPin = 12;
const byte sensorPins[] = {tonguePin, 9, eyesClosedPin, eyesOpenPin, legPin};

Button2 buttons[5];
Button2 sensors[5];

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__ __DATE__);
  // put your setup code here, to run once:
  for(int i=0; i<2; i++){
    pinMode(headMotorPins[i], OUTPUT);  
    pinMode(bodyMotorPins[i], OUTPUT);
  }
  /*
  for(int i=0; i< numInputs; i++){
    pinMode(inputPins[i], INPUT_PULLUP);
    buttons[i].begin(inputPins[i]);
    buttons[i].setPressedHandler(pressed);
    buttons[i].setReleasedHandler(released);
  }
  */
  for(int i=0; i< numSensors; i++){
    pinMode(sensorPins[i], INPUT_PULLUP);
    sensors[i].begin(sensorPins[i]);
    sensors[i].setDebounceTime(1);
    //sensors[i].setChangedHandler(changed);
  }
}

void changed(Button2& btn) {
  Serial.print("Sensor changed ");
  Serial.print(btn.getPin());
  /*
  for(int i=0; i<numSensors; i++){
    Serial.print(digitalRead(sensorPins[i]));
    Serial.print(i==numSensors-1 ? "\r\n" : ",");
  }
*/
  if(btn == sensors[0]){
    Serial.println(btn.isPressed() ? "Tongue pressed" : "Tongue released");
  }

  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(bodyMotorPins[0], LOW);
  digitalWrite(bodyMotorPins[1], LOW);
}

void openEyes(){
  digitalWrite(headMotorPins[0], HIGH);
  analogWrite(headMotorPins[1], 128);
}
void closeEyes(){
  analogWrite(headMotorPins[0], HIGH);
  digitalWrite(headMotorPins[1], 128);
}

void pressed(Button2& btn) {
  if(btn == buttons[0]){
    digitalWrite(bodyMotorPins[0], HIGH);
    digitalWrite(bodyMotorPins[1], LOW);
    //openEyes();
  }
  else if(btn == buttons[1]){
    digitalWrite(bodyMotorPins[0], LOW);
    digitalWrite(bodyMotorPins[1], HIGH);
    //closeEyes();
  }
  else if(btn == buttons[2]){   
    digitalWrite(headMotorPins[0], HIGH);
    digitalWrite(headMotorPins[1], LOW);
  }
  else if(btn == buttons[3]){   
    digitalWrite(headMotorPins[0], LOW);
    digitalWrite(headMotorPins[1], HIGH);
  }
}
void released(Button2& btn) {
    Serial.print("released: ");
    digitalWrite(headMotorPins[0], LOW);
    digitalWrite(headMotorPins[1], LOW);
    digitalWrite(bodyMotorPins[0], LOW);
    digitalWrite(bodyMotorPins[1], LOW);
    digitalWrite(LED_BUILTIN, LOW);
}

void blink(){
  digitalWrite(headMotorPins[0], HIGH);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, LOW);   
}

void talk(){
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

// Ends with head low and to the side
void moveBody(int dir=0){
  digitalWrite(bodyMotorPins[0], dir);
  digitalWrite(bodyMotorPins[1], 1-dir);
}

void loop() {
  // Read any input
  int command = -1;
  if(Serial.available() > 0){
    // Read any integer sent
    command = Serial.parseInt();
  }

  // Test Limit switches
  // eye pins
  if(sensors[2].wasPressed()){
    digitalWrite(headMotorPins[0], LOW);
    digitalWrite(headMotorPins[1], LOW);
  }
  if(sensors[3].wasPressed()){
    digitalWrite(headMotorPins[0], LOW);
    digitalWrite(headMotorPins[1], LOW);
  }
  // legPin
  if(sensors[4].wasPressed()){
    digitalWrite(bodyMotorPins[0], LOW);
    digitalWrite(bodyMotorPins[1], LOW);
  }

  switch(command){
    case 3:
      blink();
      break;
    case 4:
      talk();
      break;
    case 5:
      openEyes();
      break;
    case 6:
      closeEyes();
      break;
    case 7:
      moveBody(0);
      break;
    case 8:
      moveBody(1);
      break;
  }

  for(int i=0; i<numInputs; i++){ buttons[i].loop(); }
  for(int i=0; i<numSensors; i++){ sensors[i].loop(); }
}
