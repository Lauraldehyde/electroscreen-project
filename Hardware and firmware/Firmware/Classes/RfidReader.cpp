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
//#define SS_PIN 10
//#define RST_PIN 9

//#define RST_PIN 9
//#define SS_PIN 10

const int SS_PIN = 10;
const int RST_PIN = 9;
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::StatusCode status;
MFRC522::MIFARE_Key key;


RfidReader::RfidReader()
{
  int batchBlock =4, analysisBlock =5, cutoffBlock =6,highBlock =8, lowBlock =9, scanBlock =10, repeatBlock =12, expiryBlock =13, bioBlock = 14, fluidBlock = 16;
  String batch = "", analysis = "", cutoff = "", high = "", low = "", scan = "", repeat = "", expiry = "", bio = "", fluid = "";
  String descriptor[10] = {"batch", "analysis", "cutoff", "high", "low", "scan", "repeat", "expiry", "bio", "fluid"};
  byte readBackBlock[18];
  int blockInt [10] = {batchBlock, analysisBlock, cutoffBlock, highBlock, lowBlock, scanBlock, repeatBlock, expiryBlock, bioBlock, fluidBlock};
  String parameterOut [10] = {batch,analysis,cutoff,high,low,scan,repeat,expiry, bio, fluid};

}

String RfidReader::readRfid()
{
    bool mode = getMode();
    getRfidData();
    if(mode)
    {
      for (int i =0; i <10; i++){
      char charArray[16];
      String joinChar = "";
      byteReadBlock(blockInt[i], readBackBlock);
      for (int j=0; j<16; j++){
        charArray[j] = char(readBackBlock[j]);
      }
      for (int u=0; u<16; u++){
        joinChar = joinChar + charArray[u];
        Serial.write(charArray[u]);
      }
      parameterOut[i] = joinChar;
      Serial.flush();
    }
    }
}

//METHODS for Serial communication
bool RfidReader::getMode(){
  String full;
  int val = 0;
  int fullInt = 0;
  String junk = "";
  int index;
  while (Serial.available() == 0);
  {
    full = Serial.readStringUntil('\n');
    while (Serial.available()> 0)
    { junk = Serial.read(); }
    index = full.indexOf("True");
    if (index == -1){
      return false;
    }else {
      return true;
    }
  }  
}

//METHODS FOR RFID READING
//initialising RFID reader and reading
void RfidReader::getRfidData(){
  SPI.begin(); //SPI bus for RFID reader communication
  mfrc522.PCD_Init(); //RFID reader initialised
  for (byte i =0; i < 6; i++) key.keyByte[i] = 0xFF; //populating key with default password
  boolean a = 0; 
  while (a == 0){
    boolean x = findCard();
    a = x;
  }
}

//Return True when a RFID is present
boolean RfidReader::findCard(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  Serial.println(F("Card Detected"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump details about the card
  return 1;
  
}

//Block reading method
int RfidReader::byteReadBlock(int block, byte byteArray[]){
  byte bufferSize = 18;
  int largestModulo4Number = block/4*4;
  byte trailerBlock = largestModulo4Number+3;

  //Check authentication status
  byte sts = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if(sts != MFRC522::STATUS_OK){
    Serial.print("PCD_Authenticate() failed (read): ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return 3;//return "3" as error message
  }

  //Try to read a block
  sts = mfrc522.MIFARE_Read(block, byteArray, &bufferSize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (sts != MFRC522::STATUS_OK){
    Serial.print("MIFARE_read() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return 4; //4 is returned as an error message
  }
}
