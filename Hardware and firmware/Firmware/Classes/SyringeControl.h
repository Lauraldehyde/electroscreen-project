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
        long minsTomillis(int mins);
        void setIncubations(int sample, int wash, int antibody, int test);
        void setSampleIncubation(int mins);
        void setWashIncubation(int mins);
        void setAntibodyIncubation(int mins);
        void setTestIncubation(int mins);
        void wait(long millis);
        void wash();
        void startExp();


        //Class variables
        SyringeDriver wellMotor(22,24,23,25,26,27);
        SyringeDriver sampleMotor(28,30,29,31,32,33);
        SyringeDriver collectMotor(34,36,35,37,38,39);
        SyringeDriver washMotor(40,42,41,43,44,45);
        SyringeDriver antibodyMotor(A2,A4,A3,A5,A6,A7);
        SyringeDriver testMotor(A8,A10,A9,A11,A12,A13);
        long sampleIncubation;
        long washIncubation;
        long antibodyIncubation;
        long testIncubation;

    private:
        //add any pin refs for power management
        unsigned long lastTimeCall;

};

#endif