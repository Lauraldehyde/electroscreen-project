/*
    Potentiostat.cpp - Library for control of potentiostatic circuitry
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "Potentiostat.h"
#include "SyringeControl.h"
#include "ElectroscreenSerial.h"
#include "Wire.h"
#include "Adafruit_ADS1015.h"

Adafruit_ADS1115 ads;
float res = 2000.0/255.0;
int high = -1;
int low =-1;
int scan = -1;
int repeat = -1;
float current;
float voltage;
float reference;
int _pwmPin;
volatile int gain[4] = {0,0,0,0};
adsGain_t adcSettings[6] = {GAIN_TWOTHIRDS, GAIN_ONE, GAIN_TWO, GAIN_FOUR, GAIN_EIGHT, GAIN_SIXTEEN};
float oneBitResmV[6] = {0.1875, 0.125, 0.0625, 0.03125,0.015625, 0.0078125};
long upLimit[6] ={40000, 30000, 30000,30000,30000,30000};
long downLimit[6]= {3000, 3000, 3000, 3000, 3000, -40000};
int16_t adc;

Potentiostat::Potentiostat(int pwmPin)
{
    //Set up 16 bit ADC 
    //channel 2 = output connection, channel 0= input connection, channel 1 = reference electrode conncection
    ads.setGain(GAIN_TWOTHIRDS); // 2/3x gain +/- 6.144V  1 bit = 0.1875mV
    
    //Set up pins and variables for cyclic voltammetry    
    _pwmPin = pwmPin;  //Change according to wiring
    TCCR1B = TCCR1B & B11111000 | B00000001;
    pinMode(_pwmPin,OUTPUT);
    Serial.print("Potentiostat initialised");

}

void Potentiostat::scanCV()
{
    //set up eror checking to check high low etc have been set correctly
    Serial.println("Digital voltage in, Theorectical voltage in (mV), Measured voltage in (mV), Reference voltage (mV), Current (mA)");
    int val = 0;
    ads.begin();
    if(repeat >= 1)
    {
        Serial.println("scanCV starting");
        Serial.print("Unconverted low:"); Serial.print(low); Serial.print(", unconverted high is:"); Serial.println(high);
        Serial.print("LOW IS: "); Serial.print(convertVol(low)); Serial.print(", HIGH IS: "); Serial.println(convertVol(high));
        for(val=convertVol(low); val <= convertVol(high); ++val)
        {
            analogWrite(_pwmPin, val);
            delay(int(1000.00/(scan/res)));
            voltage = readAdc(0);
            reference = readAdc(1);
            current = readAdc(2);
            serialPrint(val, voltage, reference, current);

        }
        for(val = convertVol(high); convertVol(val) >= low; --val)
        {
            analogWrite(_pwmPin,val); //writes analog PWM value
            delay(1000/(scan/res)); //pauses for delay required to fufill scan rate (ms)
            voltage = readAdc(0);
            reference = readAdc(1);
            current = readAdc(2);
            serialPrint(val, voltage, reference, current);
        }
        repeat = repeat -1;
    }
}

void Potentiostat::serialPrint(int digitalVal, float voltage, float reference, float current)
{
    Serial.print(digitalVal); Serial.print(",");
    Serial.print(convertDigital(digitalVal)); Serial.print(",");
    Serial.print(voltage); Serial.print(",");
    Serial.print(reference); Serial.print(",");
    Serial.println(current);
}

float Potentiostat::readAdc(int channel)
{
    adc = ads.readADC_SingleEnded(channel);
    Serial.print("Down limit: "); Serial.println(downLimit[(gain[channel])]);
    Serial.print("Up limit: "); Serial.println(upLimit[(gain[channel])]);
    Serial.print("Channel: "); Serial.print(channel); Serial.print("Current gain: "); Serial.print(gain[channel]);
    if(gain[channel] < 5)
    {
        while (adc < (downLimit[(gain[channel])]))
        {
            gain[channel] += 1;
            ads.setGain(adcSettings[(gain[channel])]);
            Serial.println(adcSettings[(gain[channel])]);
            adc = ads.readADC_SingleEnded(channel);
            Serial.print("Gain: "); Serial.print(gain[channel]); Serial.print(", ADC: "); Serial.print(adc); Serial.print(", Resolution: "); Serial.println(oneBitResmV[(gain[channel])]);
        }
    }
    if(gain[channel] >= 0)
    {
    while (adc > (upLimit[(gain[channel])]))
        {
            gain[channel] -= 1;
            ads.setGain(adcSettings[(gain[channel])]);
            adc = ads.readADC_SingleEnded(channel);
        }
    }
    float milliVolts = adc * oneBitResmV[(gain[channel])];
    
    Serial.print(channel); Serial.print(" ADC: "); Serial.println(adc);

    return milliVolts;
}

int Potentiostat::convertVol(int convert)
{
    //write function that converts voltage values to digital values
    float conv3 = (convert + 1000.00)/(2000.00/255.00);
    int conv4 = round(conv3); 
    return(conv4);
}

double Potentiostat::convertDigital(int convert)
{
    //code the digital value to be converted back to mV
    double conv1 = convert*(2000.0/255.0);
    double conv2 = conv1 - 1000;
    return(conv2);
}

void Potentiostat::setCV(int h, int l, int s, int r)
{
    high = h;
    low = l;
    scan = s;
    repeat = r;
    Serial.print("High: "); Serial.print(high); Serial.print( ", Low: "); Serial.print(low); Serial.print(", Scan: "); Serial.print(scan); Serial.print(", Repeats: "); Serial.println(repeat);
}

void Potentiostat::startWash(SyringeControl control)
{
    //write function that sends a washing signal to SyringeControl
}