/*
    RfidReader.cpp - Library for RFID reading using the MFRC-RC522 module
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "RfidReader.h"
#include "SPI.h"
#include "MFRC522.h"


//THese are the pins used for the Mega - to be re-instated once new board arrives
//#define RST_PIN 53
//#define SS_PIN 5

//Nano pins for development
#define SS_PIN 10;
#define RST_PIN 9;

MFRC522::StatusCode status;
MFRC522::MIFARE_Key key;
int dataBlocks [6];
String results [6];

RfidReader::RfidReader()
{
    //initialisation of pins to connection
    MFRC522 mfrc522(SS_PIN, RST_PIN);
    SPI.begin();
    for (byte i=0; i < 6; i++) key.keyByte[i] = 0xFF;
    boolean a = 0;

}

String RfidReader::readRfid(int block)
{
    //write function
}

int RfidReader::readBlock(int block){
  byte len = 18;
  byte buffer1[18];
  char strOut[18];
  int val;
  int largestModulo4Number = block/4*4;
  byte trailerBlock = largestModulo4Number+3;
  
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    Serial.println("error in PCD_Authenticate code block");
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    Serial.println("Error in MIFARE_Read code block");
    return;
  }

  for (int i = 0; i < 10; i++)
  {
      if (buffer1[i] > 44 && buffer1[i] < 58)
        {
          Serial.write(buffer1[i]);
          strOut[i] = buffer1[i];
          val = atoi(strOut);
        } else if(buffer1[i] == NULL){
          return val;
        }
  }
  Serial.println("Never made NULL distinction");
  Serial.println(strOut);
  Serial.println(val);
  return val; 
  
}