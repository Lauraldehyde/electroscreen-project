#include <SyringeDriver.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising SyringeDriver");
  SyringeDriver motor(2038, 8, 10, 9, 11, 6, 7);
  Serial.println("Setting speed");
  motor.setSpeed(5);
  Serial.println("Moving anticlockwise, pulling back, to fill 1000");
  motor.moveSteps(1000);
  Serial.println("Moving clockwise, pushing forward to  empty 1000");
  motor.moveSteps(-4000);  

}

void loop() {
  // put your main code here, to run repeatedly:

}
