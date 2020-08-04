/*
    SyringeControl.cpp - Library for control of power and timing of multiple syringe drivers
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "SyringeControl.h"
#include "SyringeDriver.h"

SyringeControl::SyringeControl()
{
    int incubations [4];
    SyringeDriver syringes [4];
}


void SyringeControl::addSyringe(SyringeDriver syringe)
{
    /* 
        write a function to initialise SyringeDrivers 
        function may need extra inputs for the wiring of the motors
        the multiplexer channel may need to be included
    */
}

void SyringeControl::incubate(int minutes)
{
    //write function that pauses the code and the machinery for sample or antibody incubation
}

void SyringeControl::powerOn(SyringeDriver syringe)
{
    //check if any other Syringes are on if so turn off
    //turn the power for this motor on
}

void SyringeControl::powerOff(SyringeDriver syringe)
{
    //turn the power for the associated motor off
}

void SyringeControl::pushVol(SyringeDriver syringe, float vol)
{
    //turn the power for the associated motor on
    //rotate the motor an appropriate number of steps to push out the vol
}

void SyringeControl::pullVol(SyringeDriver syringe, float vol)
{
    //turn the power for the associated motor on
    //rotate the motor an appropriate number of steps to take up the vol
}

void SyringeControl::resetPos(SyringeDriver syringe)
{
    //turn the power for the associated motor on
    //rotate the motor an appropriate number of steps until the stop signal is recieved
}

void SyringeControl::wash(SyringeDriver push, SyringeDriver collect)
{
    //push volume out of push syringe
    //wait 2 minutes
    //pull volume out of pull syringe
    //repeat 3 times
}

void SyringeControl::preTest()
{
    //organise order of syringes and incubations before potentiostat here
}

void SyringeControl::postTest()
{
    //organise order of syringes and incubations after potentiostat here
}