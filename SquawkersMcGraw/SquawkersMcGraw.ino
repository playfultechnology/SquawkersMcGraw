

// INCLUDES
#include "Button2.h"

// CONSTANTS
byte headMotorPins[] = {4, 5};
byte bodyMotorPins[] = {6, 7};
byte numInputs = 5;
byte inputPins[] = {A1, A2, A3, A4, A5};
byte numSensors = 5;
byte sensorPins[] = {8, 9, 10, 11, 12};

Button2 buttons[5];
Button2 sensors[5];



void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  for(int i=0; i<2; i++){
    pinMode(headMotorPins[i], OUTPUT);  
    pinMode(bodyMotorPins[i], OUTPUT);
  }
  for(int i=0; i< numInputs; i++){
    pinMode(inputPins[i], INPUT_PULLUP);
    buttons[i].begin(inputPins[i]);
    buttons[i].setPressedHandler(pressed);
    buttons[i].setReleasedHandler(released);
  }
  for(int i=0; i< numSensors; i++){
    pinMode(sensorPins[i], INPUT_PULLUP);
    sensors[i].begin(sensorPins[i]);
    sensors[i].setChangedHandler(changed);
  }
}

void changed(Button2& btn) {
  Serial.print("Sensor changed");
  for(int i=0; i<numSensors; i++){
    Serial.print(digitalRead(sensorPins[i]));
    Serial.print(i==numSensors-1 ? "\r\n" : ",");
  }

  if(btn == sensors[0]){
    Serial.println(btn.isPressed() ? "Tongue pressed" : "Tongue released");
  }

  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(bodyMotorPins[0], LOW);
  digitalWrite(bodyMotorPins[1], LOW);
}

void openEyes(){
  while(digitalRead(sensorPins[3]) == HIGH) {
    digitalWrite(headMotorPins[0], HIGH);
    digitalWrite(headMotorPins[1], LOW);
  }
}
void closeEyes(){
  while(digitalRead(sensorPins[1]) == HIGH) {
    digitalWrite(headMotorPins[0], HIGH);
    digitalWrite(headMotorPins[1], LOW);
  }
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
  delay(500);
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, LOW);   
}

void talk(){
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(headMotorPins[0], LOW);
  digitalWrite(headMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void body(){
  digitalWrite(bodyMotorPins[0], LOW);
  digitalWrite(bodyMotorPins[1], HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(bodyMotorPins[0], LOW);
  digitalWrite(bodyMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void body2(){
  digitalWrite(bodyMotorPins[0], HIGH);
  digitalWrite(bodyMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(bodyMotorPins[0], LOW);
  digitalWrite(bodyMotorPins[1], LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  for(int i=0; i<numInputs; i++){ buttons[i].loop(); }
  for(int i=0; i<numSensors; i++){ sensors[i].loop(); }
}
