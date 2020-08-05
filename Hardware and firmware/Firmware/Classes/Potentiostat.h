/*
    Potentiostat.h - Library for control of potentiostatic circuitry
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/
#ifndef Potentiostat_h
#define Potentiostat_h

#include "Arduino.h"
#include "Potentiostat.h"
#include "SyringeControl.h"
#include "ElectroscreenSerial.h"
#include "Wire.h"
#include "Adafruit_ADS1015.h"

class Potentiostat
{
    public:
        //class functions
        Potentiostat(int pwmPin);
        void scanCV();
        void serialPrint(int digiVal, float voltage, float reference, float current);
        float readAdc(int channel);
        double convertDigital(int convert);
        int convertVol(int convert);
        void setCV(int h, int l, int s, int r);
        void startWash(SyringeControl control);
        void initArrays();

        //class variables
        volatile int gain[4];
        adsGain_t adcSettings[6];
        float oneBitResmV[6];
        int upLimit[6];
        int downLimit[6];
        int high;
        int low;
        int scan;
        int repeat;
        int vol;
        float current;
        float voltage;
        float reference;
        float res;

        //method variables
        int16_t adc;
        float milliVolts;
        double conv1;
        double conv2;
        float conv3;
        int conv4;
        unsigned long wait;

    private:
        int _pwmPin;

};

#endif