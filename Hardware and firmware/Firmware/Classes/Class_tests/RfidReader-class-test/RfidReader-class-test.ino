#include <ElectroscreenSerial.h>
#include <Potentiostat.h>
#include <RfidReader.h>
#include <SyringeControl.h>
#include <SyringeDriver.h>

void setup() {
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Inside loop");
  RfidReader rfid;
  Serial.println("rfid initilised");
  rfid.readRfid();
  Serial.println("read method complete");

  Serial.println("Testing access of variables in the array");
  //Testing access of variables
  for (int i=0; i<10; i++)
  {
    Serial.print(rfid.descriptor[i]); Serial.print(": "); Serial.println(*rfid.parameterOut[i]);
  }

  //Testing access of other variables not in the array but referenced by it
  Serial.println("Testing direct access to variables (not through array)");
  Serial.print("batch: "); Serial.println(rfid.batch);
  Serial.print("analysis: "); Serial.println(rfid.analysis);
  Serial.print("cutoff: "); Serial.println(rfid.cutoff);
  Serial.print("rfidHigh: "); Serial.println(rfid.rfidHigh);
  Serial.print("rfidLow: "); Serial.println(rfid.rfidLow);
  Serial.print("rfidScan: "); Serial.println(rfid.rfidScan);
  Serial.print("rfidRepeat: "); Serial.println(rfid.rfidRepeat);
  Serial.print("expiry: "); Serial.println(rfid.expiry);
  Serial.print("bio: "); Serial.println(rfid.bio);
  Serial.print("fluidBlock: "); Serial.println(rfid.fluidBlock);
  
  
  
}
