/*
    SyringeDriver.h - Library for control of a singular electroscreen syringe driver
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/
#ifndef SyringeDriver_h
#define SyringeDriver_h

#include "Arduino.h"
#include "Stepper.h"

class SyringeDriver
{
    public:
        //Class methods
        SyringeDriver();
        int convertToStep(float vol);
        void pushStep(int steps);
        void pullStep(int steps);
        int testLimits();
        
        //Class variables
        float totalVol;
        float currentVol;
        int stepPerTurn;
        int thread;
        float volPerStep;
        int speed;
        bool power;
        const int maxPullPin;
        const int maxPushPin;
        int maxPull;
        int maxPush;
        String limitOutputs[4];
    private:
        //put pins and multiplexer channels here
};

#endif