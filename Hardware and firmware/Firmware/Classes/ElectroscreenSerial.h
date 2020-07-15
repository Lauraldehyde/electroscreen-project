/*
    ElectroscreenSerial.h - Library for control of electroscreen serial communications
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#ifndef ElectroscreenSerial_h
#define ElectroscreenSerial_h

#include "Arduino.h"
#include "RfidReader.h"
#include "Potentiostat.h"

class ElectroscreenSerial
{
    public:
        //Class methods
        ElectroscreenSerial();
        void startComm();
        bool getRfid(RfidReader rfid);
        void sendRfid(Potentiostat potentio);
        void stopComm();
    private:
        int _baud;
}