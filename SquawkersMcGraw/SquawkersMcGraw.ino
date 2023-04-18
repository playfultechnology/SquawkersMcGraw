
byte headMotorPins[] = {4, 5};
byte bodyMotorPins[] = {6, 7};

void setup() {
  // put your setup code here, to run once:
  for(int i=0; i<2; i++){
    pinMode(headMotorPins[i], OUTPUT);  
    pinMode(bodyMotorPins[i], OUTPUT);
  }
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
  body();
  delay(2000);
  blink();
  body2();
  body2();
  delay(2000);
  body();
  delay(5000);
  blink();
  talk();
}
