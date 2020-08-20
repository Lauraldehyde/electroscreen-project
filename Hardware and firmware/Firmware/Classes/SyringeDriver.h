/*
    SyringeDriver.h - Library for control of a singular electroscreen syringe driver
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/
#ifndef SyringeDriver_h
#define SyringeDriver_h

#include "Arduino.h"
#include "Stepper.h"

#define STEPS 2038


class SyringeDriver
{
    public:
        //Class methods
        SyringeDriver(int in1, int in2, int in3, int in4);
        void setSyringe(int vol, int extension);
        int convertToStep(float vol);
        void rotateStep(int step);
        int testLimits();
        
        //Class variables
        float totalVol;
        float currentVol;
        int stepPerTurn;
        int thread;
        float volPerStep;
        int speed;
        bool power;
        Stepper *motorReference;

        //Variables associated with limit detection
        const int maxPullPin;
        const int maxPushPin;
        int maxPull;
        int maxPush;
        String limitOutputs[4];
    private:
        //put pins and multiplexer channels here
};

#endif