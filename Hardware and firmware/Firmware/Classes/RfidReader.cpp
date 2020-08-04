/*
    RfidReader.cpp - Library for RFID reading using the MFRC-RC522 module
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "RfidReader.h"

RfidReader::RfidReader(int connection)
{
    //initialisation of pins to connection
    int dataBlocks [6];
    String results [6];
}

String RfidReader::readRfid(int block)
{
    //write function
}