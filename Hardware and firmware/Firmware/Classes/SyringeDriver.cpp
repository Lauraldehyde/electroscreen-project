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
int maxPullPin;
int maxPushPin;
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
int maxPull;
int maxPush;
int syringeNumber;

SyringeDriver::SyringeDriver(int in1, int in2, int in3, int in4, int limit1, int limit2, SyringeControl controller)
{
    maxPullPin = limit1;
    maxPushPin = limit2;
    limitOutputs[0] = "No limits reached"; limitOutputs[1] = "maxPull limit reached"; limitOutputs[2] = "maxPush limit reached"; limitOutputs[3] = "Error in limit sensors";
    pinMode(maxPullPin, INPUT);
    pinMode(maxPushPin, INPUT);
    Serial.println("Pins set.");
    Serial.println(limitOutputs[testLimits()]);
    Stepper myStepper(STEPS, in1, in2, in3, in4);
    motorReference = &myStepper;
    speed =5; //The speed is in rotations per minute this is set as an arbitrary until more testing is done max speed advised is
    myStepper.setSpeed(speed);
    screwPitch = 2;
    reference = passStepper(myStepper, controller);
}

void SyringeDriver::passStepper(Stepper stepper, SyringeControl controlller)
{
    if (controller.syringeCount == 0)
    {
        syringeNumber = controller.syringeCount;
        controller.steppers[controller.syringeCount] = stepper;
        controller.syringeCount = syringeNumber + 1;
    }else
    {
        syringeNumber = controller.syringeCount +1;
        controller.syringeCount = syringeNumber;
        controller.stepper[syringeNumber] = stepper;
    }
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
    Serial.print("Inside rotateStep(), stepInt set to: "); Serial.println(stepInt);
    int interval =  10;
    Serial.print("Interval set to: "); Serial.println(interval);
    if (stepInt > 0)
    {
        Serial.println("stepInt is greater than 0");
        for (int s = 0; s < stepInt; s + interval)
        {
            int tL = testLimits();
            Serial.print("TestLimits returned: "); Serial.println(tL);
            if(tL == 0)
            {
                Serial.print("testLimits returned 0.");
                reference.step(interval);
                Serial.print("Motor move attempted.");
            }else{
                break;
            }            
        }
    }
    if (stepInt < 0)
    {
        for (int s =0; s > stepInt; s-interval)
        {
            int tL = testLimits();
            Serial.print("TestLimits returned: "); Serial.println(tL);
            if(tL == 0)
            {
                Serial.print("testLimitsnreturned 0.");
                reference.step(-1 *interval);
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
   int maxPull = digitalRead((int)maxPullPin);
   int maxPush = digitalRead((int)maxPushPin);
   if (maxPull==1 && maxPush == 1)
   {
       return 0;
   } else if (maxPull == 0 && maxPush == 1)
   {
       Serial.println("Push limit reached.");
       return 1;
       
   } else if (maxPull == 1 && maxPush == 0)
   {
       Serial.println("Pull limit reached.");
       return 2;       
   } else
   {
       Serial.println("Error");
       return 3;
   }
}