/*
    Potentiostat.cpp - Library for control of potentiostatic circuitry
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "Potentiostat.h"
#include "SyringeControl.h"
#include "ElectroscreenSerial.h"

Potentiostat::Potentiostat(int a, int d)
{
    int _analogPin = a;  //Change according to wiring
    int _digitalPin = d; //Change according to wiring
    int high = 1;
    int low =-1;
    int scan = 100;
    int repeat = 2;
}

void Potentistat::scanCV(ElectroscreenSerial ser)
{
    //write function that scans up and down and sends data to serial
}

void Potentistat::convertVol(int convert)
{
    //write function that converts digital values to voltages
}

void Potentistat::convertDigital(int convert)
{
    //write function that converts voltages to digital values
}

void Potentistat::startWash(SyringeControl control)
{
    //write function that sends a washing signal to SyringeControl
}