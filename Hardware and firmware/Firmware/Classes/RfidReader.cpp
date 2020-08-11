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
int batchBlock, analysisBlock, cutoffBlock,highBlock, lowBlock, scanBlock, repeatBlock, expiryBlock, bioBlock, fluidBlock;
//String batch = "", analysis = "", cutoff = "", rfidHigh = "", rfidLow = "", rfidScan = "", rfidRepeat = "", expiry = "", bio = "", fluid = "";
String batch, analysis, cutoff, rfidHigh, rfidLow, rfidScan, rfidRepeat, expiry, bio, fluid;
String descriptor[10];
byte readBackBlock[18];
//int blockInt [] = {batchBlock, analysisBlock, cutoffBlock, highBlock, lowBlock, scanBlock, repeatBlock, expiryBlock, bioBlock, fluidBlock};
int blockInt[10];
//String parameterOut [] = {batch,analysis,cutoff,rfidHigh,rfidLow,rfidScan,rfidRepeat,expiry, bio, fluid};
String parameterOut[10];


RfidReader::RfidReader()
{
  //Assigning values of class variables
  batchBlock =4; analysisBlock =5; cutoffBlock =6;highBlock =8; lowBlock =9; scanBlock =10; repeatBlock =12; expiryBlock =13; bioBlock = 14; fluidBlock = 16;
  batch = ""; analysis = ""; cutoff = ""; rfidHigh = ""; rfidLow = ""; rfidScan = ""; rfidRepeat = ""; expiry = ""; bio = ""; fluid = "";
  blockInt[0]=batchBlock; blockInt[1]=analysisBlock; blockInt[2]=cutoffBlock; blockInt[3]=highBlock; blockInt[4]=lowBlock; blockInt[5]=scanBlock; blockInt[6]=repeatBlock;
  blockInt[7]=expiryBlock; blockInt[8]=bioBlock; blockInt[9]=fluidBlock;  
  descriptor[0]= "batch"; descriptor[1] = "analysis"; descriptor[2] = "cutoff"; descriptor[3]="high"; descriptor[4]="low"; descriptor[5] ="scan"; descriptor[6]="repeat";
  descriptor[7]="expiry"; descriptor[8]="biomarker"; descriptor[9]="fluid";
  parameterOut[0]=batch; parameterOut[1]=analysis; parameterOut[2]=cutoff; parameterOut[3]=rfidHigh; parameterOut[4]=rfidLow; parameterOut[5]=rfidScan; parameterOut[6]=rfidRepeat;
  parameterOut[7]=expiry; parameterOut[8]=bio; parameterOut[9]=fluidBlock;

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
        //Serial.write(charArray[u]); 
      }
      parameterOut[i] = joinChar;
      //Serial.flush();
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
  for (byte i =0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF; //populating key with default password
  } 
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
  byte sts;

  //Check authentication status
  sts = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid)); 
  //Serial.print("Status:  "); Serial.println(sts);
  //Serial.print("MFRC522::STATUS_OK:  "); Serial.print(MFRC522::STATUS_OK);
  if(sts != MFRC522::STATUS_OK){
    Serial.print("PCD_Authenticate() failed (read): ");
    Serial.println(mfrc522.GetStatusCodeName(sts));
    return 3;//return "3" as error message
  }

  //Try to read a block
  sts = mfrc522.MIFARE_Read(block, byteArray, &bufferSize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (sts != MFRC522::STATUS_OK){
    Serial.print("MIFARE_read() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(sts));
    return 4; //4 is returned as an error message
  }
}
