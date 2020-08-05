/*
    RfidReader.h - Library for RFID reading using the MFRC-RC522 module
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/
#ifndef RfidReader_h
#define RfidReader_h

#include "Arduino.h"

class RfidReader
{
    public:
        RfidReader(int connection); //update initiation to include all pins needed for control
        String readRfid(int block);
        int readBlock(int block);
        int dataBlocks[6];
        String results[6];
    private:
        //wiring of pins to be put in here
        int _pin;

};

#endif