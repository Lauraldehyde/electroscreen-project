/*
    SyringeControl.cpp - Library for control of power and timing of multiple syringe drivers
    Created by Laura Moody, 14th July 2020
    University of Portsmouth
*/

#include "Arduino.h"
#include "SyringeControl.h"
#include "SyringeDriver.h"

SyringeControl::SyringeControl()
{
    float oneMlSyringe = 0.069228; //uL in each step
    float tenMlSyringe = 0.69266; //uL in each step
    this->sampleMotor.setConversion(oneMlSyringe);
    this->collectMotor.setConversion(tenMlSyringe);
    this->washMotor.setConversion(oneMlSyringe);
    this->antibodyMotor.setConversion(oneMlSyringe);
    this->testMotor.setConversion(oneMlSyringe);
    setIncubations(30,2,30,15);

}

void SyringeControl::setIncubations(int sample, int wash, int antibody, int test)
{
    this->sampleIncubation = long(sample)*60L*1000L;
    this->washIncubation = long(wash)*60L*1000L;
    this->antibodyIncubation = long(antibody)*60L*1000L;
    this->testIncubation = long(test)*60L*1000L;
}

void SyringeControl::setSampleIncubation(int mins)
{
    this->sampleIncubation = long(mins)*60L*1000L;
}

void SyringeControl::setWashIncubation(int mins)
{
    this->washIncubation = long(mins)*60L*1000L;
}

void SyringeControl::setAntibodyIncubation(int mins)
{
    this->antibodyIncubation = long(mins)*60L*1000L;
}
void SyringeControl::setTestIncubation(int mins)
{
    this->testIncubation = long(mins)*60L*1000L;
}

void SyringeControl::wait(long millis)
{
    unsigned long now = millis();
    this->lastTimeCall = now;
    if(now - lastTimeCall < millis)
    {
        delay(1000);
        now = millis();
    }
}

void SyringeControl::wash()
{
    this->collectMotor.moveVol(100);
    for(int n = 0; n<4; n++)
    {
        this->washMotor.moveVol(-100);
        wait(this->washIncubation);
        this->collectMotor.moveVol(100);
    }   
    
}

void SyringeControl::startExp()
{
    this->sampleMotor.moveVol(-50);
    wait(this->sampleIncubation);
    wash();
    this->antibodyMotor.moveVol(-50);
    wait(this->antibodyIncubation);
    wash();
    this->testMotor.moveVol(-50);
    wait(this->testIncubation);
}




