/*
    SyringeControl.h - Library for control of power and timing of multiple syringe drivers
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#ifndef SyringeControl_h
#define SyringeControl_h

#include "Arduino.h"
#include "SyringeDriver.h"

class SyringeControl
{
    public:
        //Class methods
        SyringeControl();
        void addSyringe(SyringeDriver syringe);
        void incubate(int minutes);
        void powerOn(SyringeDriver syringe);
        void powerOff(SyringeDriver syringe);
        bool pushVol(SyringeDriver syringe, float vol);
        bool pullVol(SyringeDriver syringe, float vol);
        bool resetPos(SyringeDriver syringe);
        bool wash(SyringeDriver push, SyringeDriver collect);
        void preTest();
        void postTest();

        //Class variables
        SyringeDriver[] syringes;
        static int[] incubations;
    private:
        //add any pin refs for power management

}