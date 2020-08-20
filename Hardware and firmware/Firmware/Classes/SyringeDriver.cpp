/*
    SyringeDriver.cpp - Library for control of a singular electroscreen syringe driver
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "Stepper.h"
#include "SyringeDriver.h"

#define STEPS 2048

//Collision sensors - used as limit sensors here
const int maxPullPin = 6;
const int maxPushPin = 7;
int maxPull;
int maxPush;
String limitOutputs[4];

/* Initial motor selected is the 28BYJ-48 with the ULN2003 Motor Driver Board - may be channged
Initial lead screw used was an 8 mm Diameter 2mm Lead Tr8x2 with a flanged lead nut
Below are the variable associated with the motor
*/
int speed;
int steps;
long totalSteps;
int screwPitch;
int syringeVol;
int syringeExtension;
float volPerMilimetre;
float distancePerStep;
float volPerStep;
Stepper *motorReference;

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
    Stepper myStepper(STEPS, in1, in2, in3, in4);
    motorReference = &myStepper;
    speed =10; //The speed is in rotations per minute this is set as an arbitrary until more testing is done max speed advised is
    screwPitch = 2;
}

void SyringeDriver::setSyringe(int vol, int extension)
{
    syringeVol = vol;
    syringeExtension = extension;
    volPerMilimetre = vol/extension;
    distancePerStep = screwPitch/STEPS;
    volPerStep = distancePerStep * volPerMilimetre;
}

int SyringeDriver::convertToStep(float vol)
{
    int stepsOut = round(vol*volPerStep);
    return stepsOut;
}


void SyringeDriver::rotateStep(int stepInt)
{
    //write function to rotate motor the steps specified in the pull direction
    int interval =  1;
    if (stepInt > 0)
    {
        for (int s = 0; s < stepInt; s ++)
        {
            if(testLimits() == 1)
            {
                (*motorReference).step(interval);
            }else{
                break;
            }            
        }
    }
    if (stepInt < 0)
    {
        for (int s =0; s > stepInt; s--)
        {
            if(testLimits() == 1)
            {
                (*motorReference).step(-1 *interval);
            }else{
                break;
            }
        }
    }
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
       Serial.println("Push limit reached.");
       return 2;       
   } else
   {
       Serial.println("Pull limit reached");
       return 3;
   }
}