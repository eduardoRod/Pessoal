#include "Servo.h"

Servo servo;
int val;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  servo.attach(11);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = map(analogRead(A0), 0, 1023, 0, 180);
  servo.write(val);
}
