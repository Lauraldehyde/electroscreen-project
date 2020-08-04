/*
    SyringeDriver.cpp - Library for control of a singular electroscreen syringe driver
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "SyringeDriver.h"

SyringeDriver::SyringeDriver()
{


}

int SyringeDriver::convertToStep(float vol)
{
    //write function to convert mL or uL to steps of motor needed
}

void SyringeDriver::pushStep(int steps)
{
    //write function to rotate motor the steps specified in the push direction
}

void SyringeDriver::pullStep(int steps)
{
    //write function to rotate motor the steps specified in the pull direction
}