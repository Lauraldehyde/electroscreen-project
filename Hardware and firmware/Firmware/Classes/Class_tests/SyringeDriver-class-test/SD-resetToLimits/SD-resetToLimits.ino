#include <SyringeDriver.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising SyringeDriver");
  SyringeDriver motor(2038, 8, 10, 9, 11, 6, 7);
  Serial.println("Setting speed");
  motor.setSpeed(5);
  Serial.println("Testing resetToEmpty()");
  Serial.println("Motor will rotate clockwise until switch 2 triggered");
  motor.resetToEmpty();
  delay(1000);
  Serial.println("Testing resetToFull()");
  Serial.println("Motor will rotate anticlockwise until switch 1 triggered");
  motor.resetToFull();
}

void loop() {
  // put your main code here, to run repeatedly:

}
