/*
    Potentiostat.h - Library for control of potentiostatic circuitry
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/
#ifndef Potentiostat_h
#define Potentiostat_h

#include "Arduino.h"
#include "SyringeControl.h"

class Potentiostat
{
    public:
        //class functions
        Potentiostat(int high, int low, int scan, int repeat);
        void scanCV();
        int convertVol(int convert);
        double convertDigital(int convert);
        void startWash(SyringeControl control);

        //class variables
        int high;
        int low;
        int scan;
        int repeat;

    private:
        int _analogPin;
        int _ouputPin;
        int _resolution;


};