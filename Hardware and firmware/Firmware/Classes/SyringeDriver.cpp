/*
    SyringeDriver.cpp - Library for control of the motor of a syringe driver
    Based on code Stepper.h which can be found at https://www.arduino.cc/en/Reference/Stepper
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "SyringeDriver.h"

SyringeDriver::SyringeDriver(int numberOfSteps, int motorPinOne, int motorPinTwo, int motorPinThree, int motorPinFour, int limitOne, int limitTwo)
{
    //initialises variables and sets up pins
    this->stepNumber = 0;
    this->direction = 0;
    this->lastStepTime = 0;
    this->numberOfSteps = numberOfSteps;

    this->motorPinOne = motorPinOne;
    this->motorPinTwo = motorPinTwo;
    this->motorPinThree = motorPinThree;
    this->motorPinFour = motorPinFour;
    this->limitOne = limitOne;
    this->limitTwo = limitTwo;

    pinMode(this->motorPinOne, OUTPUT);
    pinMode(this->motorPinTwo, OUTPUT);
    pinMode(this->motorPinThree, OUTPUT);
    pinMode(this->motorPinFour, OUTPUT);
    pinMode(this->limitOne, INPUT);
    pinMode(this->limitTwo, INPUT);


}

void SyringeDriver::setSpeed(long newSpeed)
{
    //sets the rpm of the motor
    this->stepDelay = 60L*1000L*1000L / this->numberOfSteps / newSpeed;
}

void SyringeDriver::moveSteps(int steps)
{
    //moves the motor a number of steps
    int stepsLeft = abs(steps);
    if (steps > 0){this -> direction = 1;}
    if (steps < 0){this -> direction = 0;}

    while(stepsLeft>0)
    {
        unsigned long now = micros();
        if(now - this -> lastStepTime >= this -> stepDelay)
        {
            this -> lastStepTime = now;
            if(this -> direction == 1)
            {
                this -> stepNumber++;
                if(this -> stepNumber == this -> numberOfSteps){this -> stepNumber = 0;}
            }
            else
            {
                if(this -> stepNumber == 0){this -> stepNumber = this -> numberOfSteps;}
                this -> stepNumber--;
            }
            stepsLeft--;
            step(this -> stepNumber % 4);
        }
    }
}

void SyringeDriver::step(int thisStep)
{
    //moves the motor one step
    switch(thisStep)
    {
        case 0: //1010
            digitalWrite(motorPinOne, HIGH);
            digitalWrite(motorPinTwo, LOW);
            digitalWrite(motorPinThree, HIGH);
            digitalWrite(motorPinFour, LOW);
        break;
        case 1: //0110
            digitalWrite(motorPinOne, LOW);
            digitalWrite(motorPinTwo, HIGH);
            digitalWrite(motorPinThree, HIGH);
            digitalWrite(motorPinFour, LOW);
        break;
        case 2: //0101
            digitalWrite(motorPinOne, LOW);
            digitalWrite(motorPinTwo, HIGH);
            digitalWrite(motorPinThree, LOW);
            digitalWrite(motorPinFour, HIGH);
        break;
        case 3: //1001
            digitalWrite(motorPinOne, HIGH);
            digitalWrite(motorPinTwo, LOW);
            digitalWrite(motorPinThree, LOW);
            digitalWrite(motorPinFour, HIGH);
        break;
    }
}

int SyringeDriver::testLimits()
{
    /* function tests whether max push or max pull has been reached
    Low = switch activated (0), High = not at limit(1)
    No limits reached: return 0
    Max pull reached: return 1
    Max push reached: return 2
    Both limits triggered (an error) return 3*/
    int maxPull = digitalRead((int)limitOne);
    int maxPush = digitalRead((int)limitTwo);
    if(maxPull==1 && maxPush==1)
    {
        return 0;
    } else if(maxPull == 0 && maxPush == 1)
    {
        return 1;
    } else if (maxPull == 1 && maxPush == 0)
    {
        return 2;
    } else 
    {
        return 3;
    }
}

void SyringeDriver::resetToFull()
{
    //rotate motor anticlockwise until switchTwo is triggered and syringe is full
    int limitResult = testLimits();
    while(limitResult != 1)
    {
        unsigned long now = micros();
        if(now - this->lastStepTime >= this->stepDelay)
        {
            this->lastStepTime = now;
            if(this->stepNumber == this->numberOfSteps){this->stepNumber = 0;}
            this->stepNumber++;
            step(this->stepNumber%4);
        }
        limitResult = testLimits();
    }
}

void SyringeDriver::resetToEmpty()
{
    //rotate motor clockwise until switchOne is triggered and syringe is emptied
    int limitResult = testLimits();
    while(limitResult != 2)
    {
        unsigned long now = micros();
        if(now - this->lastStepTime >= this->stepDelay)
        {
            this->lastStepTime = now;
            if(this->stepNumber == 0){this->stepNumber = this->numberOfSteps;}
            this->stepNumber--;
            step(this->stepNumber%4);
        }
        limitResult = testLimits();
    }
}