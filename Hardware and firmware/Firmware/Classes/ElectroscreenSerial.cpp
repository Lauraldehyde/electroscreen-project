/*
    ElectroscreenSerial.cpp - Library for control of electroscreen serial communications
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "ElectroscreenSerial.h"
#include "RfidReader.h"
#include "Potentiostat.h"

ElectroscreenSerial::ElectroscreenSerial(int initialisationParas)
{
    //initialisation
}

void ElectroscreenSerial::startComm()
{
    //setup a serial connection with the comoputer
}

bool ElectroscreenSerial::getRfid(RfidReader rfid)
{
    //try to get information from an Rfid
}

void ElectroscreenSerial::sendRfid(Potentiostat potentio)
{
    //send Rfid info to serial and potentiostat
}

void ElectroscreenSerial::stopComm()
{
    //stop the serial connection with the comoputer
}