#include <SyringeDriver.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising SyringeDriver");
  SyringeDriver motor(2038, 8, 10, 9, 11);
  Serial.println("Setting speed");
  motor.setSpeed(1);
  Serial.println("Moving forward 1000");
  motor.moveSteps(1000);
  Serial.println("Moving backward 1000");
  motor.moveSteps(-1000);  

}

void loop() {
  // put your main code here, to run repeatedly:

}
