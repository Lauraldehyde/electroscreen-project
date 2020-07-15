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
        RfidReader(int connection);
        String readRfid(int block);
        int dataBlocks[];
        String results[];
    private:
        //wiring of pins to be put in
        int _pin;

}