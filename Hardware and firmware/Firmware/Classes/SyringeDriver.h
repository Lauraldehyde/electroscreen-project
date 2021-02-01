/*
    SyringeDriver.h - Library for control of the motor of a syringe driver
    Based on code Stepper.h which can be found at https://www.arduino.cc/en/Reference/Stepper
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
        SyringeDriver(int numberOfSteps, int motorPinOne, int motorPinTwo, int motorPinThree, int motorPinFour, int limitOne, int limitTwo);
        void setSpeed(long newSpeed); //set speed
        void moveSteps(int numberOfSteps); //move a number of steps
        void resetToFull(); //rotate the motor until switchOne is triggered
        void resetToEmpty(); //rotate the motor until switchTwo is triggered
        int testLimits(); //Find the state of the switches and return 
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
        int limitOne;
        int limitTwo;

        unsigned long lastStepTime; //timestamp of last step taken, us
};

#endif