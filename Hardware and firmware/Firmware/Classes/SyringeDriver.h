/*
    SyringeDriver.h - Library for control of a singular electroscreen syringe driver
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/
#ifndef SyringeDriver_h
#define SyringeDriver_h

#include "Arduino.h"

class SyringeDriver
{
    public:
        //Class methods
        SyringeDriver(int numberOfSteps, int motorPinOne, int motorPinTwo, int motorPinThree, int motorPinFour);
        void setSpeed(long newSpeed); //set speed
        void moveSteps(int numberOfSteps); //move a number of steps
    private:
        void step(int thisStep); //step the motor one step

        int direction; //direction of rotation
        unsigned long stepDelay; //delay between steps, ms
        int numberOfSteps; //total number of steps the motor can take
        int stepNumber; //which step the motor is on

        int motorPinOne;
        int motorPinTwo;
        int motorPinThree;
        int motorPinFour;

        unsigned long lastStepTime; //timestamp of last step taken, us
};

#endif