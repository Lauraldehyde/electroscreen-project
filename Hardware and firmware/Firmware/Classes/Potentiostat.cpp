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
double res = 2000.0/255.0;
int high = -1;
int low =-1;
int scan = -1;
int repeat = -1;
float current;
float voltage;
float reference;
int _pwmPin;
int gain[4] = {0,0,0,0};
adsGain_t adcSettings[6] = {GAIN_TWOTHIRDS, GAIN_ONE, GAIN_TWO, GAIN_FOUR, GAIN_EIGHT, GAIN_SIXTEEN};
float oneBitResmV[6] = {0.1875, 0.125, 0.0625, 0.03125,0.015625, 0.0078125};
long upLimit[6] ={40000, 30400, 28800,27200,25600,25600};
long downLimit[6]= {18667, 13600, 12800, 12480, 11520, -40000};

Potentiostat::Potentiostat(int pwmPin)
{
    //Set up 16 bit ADC 
    //channel 2 = output connection, channel 0= input connection, channel 1 = reference electrode conncection
    ads.setGain(GAIN_TWOTHIRDS); // 2/3x gain +/- 6.144V  1 bit = 0.1875mV
    
    //Set up pins and variables for cyclic voltammetry    
    _pwmPin = pwmPin;  //Change according to wiring
    TCCR1B = TCCR1B & B11111000 | B00000001;
    pinMode(_pwmPin,OUTPUT);

}

void Potentiostat::scanCV()
{
    //set up eror checking to check high low etc have been set correctly
    Serial.println("Digital voltage in, Theorectical voltage in (mV), Measured voltage in (mV), Reference voltage (mV), Current (mA)");
    int val = 0;
    if(repeat >= 1)
    {
        for(val=convertVol(low); val <= convertVol(high); val++)
        {
            Serial.print("LOW IS: "); Serial.print(convertVol(low)); Serial.print(", HIGH IS: "); Serial.println(convertVol(high));
            analogWrite(_pwmPin, val);
            delay(int(1000.00/(scan/res)));
            voltage = readAdc(0);
            reference = readAdc(1);
            current = readAdc(2);
            serialPrint(val, voltage, reference, current);

        }
        for(val = convertVol(high); convertVol(val) >= low; val--)
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
    int16_t adc;
    adc = ads.readADC_SingleEnded(channel);
    while (adc < (downLimit[(gain[channel])]))
    {
        gain[channel] += 1;
        ads.setGain(adcSettings[channel]);
        adc = ads.readADC_SingleEnded(channel);
    }
    while (adc > (upLimit[(gain[channel])]))
    {
        gain[channel] -= 1;
        ads.setGain(adcSettings[channel]);
        adc = ads.readADC_SingleEnded(channel);
    }
    float milliVolts = adc * oneBitResmV[(gain[channel])];
    return milliVolts;
}

int Potentiostat::convertVol(int convert)
{
    //write function that converts voltage values to digital values
    int conv3 = convert + 1000;
    double conv4 = (((double)conv3))/res;
    int conv5 = round(conv4); 
    return(conv5);
}

double Potentiostat::convertDigital(int convert)
{
    //code the digital value to be converted back to mV
    double conv1 = convert*(2000.0/255.0);
    double conv2 = conv1 - 1000;
    return(conv2);
}

void setCV(int h, int l, int s, int r)
{
    high = h;
    low = l;
    scan = s;
    repeat = r;
}

void Potentiostat::startWash(SyringeControl control)
{
    //write function that sends a washing signal to SyringeControl
}