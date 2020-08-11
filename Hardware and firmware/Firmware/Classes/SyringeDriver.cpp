/*
    SyringeDriver.cpp - Library for control of a singular electroscreen syringe driver
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "Stepper.h"
#include "SyringeDriver.h"

#define STEPS 2038

//Collision sensors - used as limit sensors here
const int maxPullPin = 6;
const int maxPushPin = 7;
int maxPull;
int maxPush;
String limitOutputs[4];

SyringeDriver::SyringeDriver(int in1, int in2, int in3, int in4)
{
    limitOutputs[0] = "No limits reached"; limitOutputs[1] = "maxPull limit reached"; limitOutputs[2] = "maxPush limit reached"; limitOutputs[3] = "Error in limit sensors";
    pinMode(maxPullPin, INPUT);
    pinMode(maxPushPin, INPUT);
    maxPull = digitalRead(maxPullPin);
    maxPush = digitalRead(maxPushPin);
    Serial.print("maxPull is: "); Serial.println(maxPull);
    Serial.print("maxPush is: "); Serial.println(maxPush);
    Serial.println(limitOutputs[testLimits()]);
    Stepper stepper(STEPS, in1, in2, in3, in4);


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

int SyringeDriver::testLimits()
{
    /*function tests whether max push or max pull has been reached
    Low = switch activated (0), High = not at limit(1)
    No limits reached: return 0
    Max pull reached: return 1
    Max push reached: return 2
    Both limits triggered (an error) return 3
    */
   maxPull = digitalRead(maxPullPin);
   maxPush = digitalRead(maxPushPin);
   if (maxPull==1 && maxPush == 1)
   {
       return 0;
   } else if (maxPull == 0 && maxPush == 1)
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