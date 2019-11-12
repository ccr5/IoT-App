#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int input = 2;
const int lamp = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(input, INPUT);
  pinMode(lamp, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int result = digitalRead(input);
  if (result == HIGH) {
    digitalWrite(lamp, HIGH);
  } else if (result == LOW) {
    digitalWrite(lamp, LOW);
  }
}
