/*
    ElectroscreenSerial.cpp - Library for control of electroscreen serial communications
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/


//Note from 21/07/2020 - not sure this is an essential class may be covered by main.py
#include "Arduino.h"
#include "ElectroscreenSerial.h"
#include "RfidReader.h"
#include "Potentiostat.h"

ElectroscreenSerial::ElectroscreenSerial()
{
    //initialisation
    Serial.begin(9600);
    Serial.println("ElectroscreenSerial communication open");
    Serial.flush();
}

void ElectroscreenSerial::startComm()
{
    //setup a serial connection with the comoputer
}

bool ElectroscreenSerial::getRfid(RfidReader rfid)
{
    //try to get information from an Rfid
}

void ElectroscreenSerial::sendRfid()
{
    //send Rfid info to serial and potentiostat
}

void ElectroscreenSerial::stopComm()
{
    //stop the serial connection with the comoputer
}