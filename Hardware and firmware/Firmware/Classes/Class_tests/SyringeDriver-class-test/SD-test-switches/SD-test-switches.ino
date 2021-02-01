#include <SyringeDriver.h>
SyringeDriver motor(2038,8,10,9,11,7,6); 

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising SyringeDriver"); 
}

void loop() {
  int switchResult = motor.testLimits();
  switch(switchResult)
  {
    case 0:
      Serial.println("Neither switch active");
      break;
    case 1:
      Serial.println("Switch one active");
      break;
    case 2:
      Serial.println("Switch two active");
      break;
    case 3:
      Serial.println("Both switches active");
      Serial.println("Likely error - check switches");
      break;
  }
  delay(500);

}
