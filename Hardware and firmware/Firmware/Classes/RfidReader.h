/*
    RfidReader.h - Library for RFID reading using the MFRC-RC522 module
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/
#ifndef RfidReader_h
#define RfidReader_h
//#define RST_PIN 9
//#define SS_PIN 10

#include "Arduino.h"
#include "RfidReader.h"
#include "MFRC522.h"
#include "SPI.h"



class RfidReader
{
    public:
        //methods
        RfidReader(); //update initiation to include all pins needed for control
        String readRfid();
        bool getMode();
        void getRfidData();
        boolean findCard();
        int byteReadBlock(int block, byte byteArray[]);
        
        //variables
        const int SS_PIN = 10;
        const int RST_PIN = 9;
        MFRC522 mfrc522;
        MFRC522::StatusCode status;
        MFRC522::MIFARE_Key key;

        int batchBlock;
        int analysisBlock;
        int cutoffBlock;
        int highBlock;
        int lowBlock;
        int scanBlock;
        int repeatBlock;
        int expiryBlock;
        int bioBlock;
        int fluidBlock;
        String batch;
        String analysis;
        String cutoff;
        String high;
        String low;
        String scan;
        String repeat;
        String expiry;
        String bio;
        String fluid;
        String descriptor[10];
        byte readBackBlock[18];
        int blockInt [10];
        String parameterOut [10];

        bool mode;
        char charArray[16];
        String joinChar = "";
        String full;
        int val;
        int fullInt;
        String junk;
        int index;
        boolean a;

        byte bufferSize;
        int largestModulo4Number;
        byte trailerBlock;
        byte sts;
  

    private:
        //wiring of pins to be put in here

};

#endif