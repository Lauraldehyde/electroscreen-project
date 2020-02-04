#include "math.h"               // include the Math Library
//variables for parameter setting
int high = -1;
int low = -1;
int scan = -1;
int repeat = -1;
int go = -1;
int resCount = 0;
long resistor = 10000000;
double res = 2000.0/255.0;

char junk = ' ';

//variables for potentiostat running
int a = 10;  int en = 4;  int s0 = 5;  int s1 = 6; int s2 = 7; int s3 = 8;
int val = 0;
int ct = A0;
int c = 0;
int n = 0;

//recording strings
String hString = "The voltage high (mV) is: ";
String lString = "The voltage low (mV) is: ";
String sString = "The scan rate (mV/s) is: ";
String rString = "Number of scans is :";
String goString = "Assay initialized. ";

void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);           // set up Serial library at 9600 bps

  Serial.println("Lets input our parameters.");
  Serial.flush();
  
  //get parameters
  high = convertVol(getParameter(hString));
  low = convertVol(getParameter(lString));
  scan = getParameter(sString);
  repeat = getParameter(rString);
  go = getParameter(goString);
  //show parameter in digital selected
  Serial.println("Digital parameters set:");
  Serial.print("High volatage: "); Serial.print(convertDigital(high)); Serial.print(" mV ");
  Serial.print("Low voltage: "); Serial.print(convertDigital(low)); Serial.print(" mV ");
  Serial.print("Scan rate: "); Serial.print(scan); Serial.print(" mV/s ");
  Serial.print("Repeats: "); Serial.println(repeat);
  if(high != -1 && low != -1 && scan != -1 && repeat != -1 && go != -1){
     
    }else {(Serial.println("Parameters not input"));}
    delay(500);

  //set up pins for cyclic voltammetry
  TCCR1B = TCCR1B & B11111000 | B00000001;
  pinMode(a,OUTPUT); //Sets Pin 10 (a declared to Pin 10 above) as an output - variable 'a' the PWM output to the potentiostat
  pinMode(en, OUTPUT);  pinMode(s0, OUTPUT);  pinMode(s1, OUTPUT);  pinMode(s2, OUTPUT);  pinMode(s3, OUTPUT); //for automated resitstor selection
  pinMode(ct,INPUT); //Sets pin A0 (ct declared to A0 above) as an inout- variable 'c' is the voltage proportional to current measurement
  digitalWrite(en,0); digitalWrite(s0,0); digitalWrite(s1,0); digitalWrite(s2,0); digitalWrite(s3,0);
  
}

void loop()
{
    if(high != -1 && low != -1 && scan != -1 && repeat != -1){
        if (repeat > 1){scanCV(); repeat--;
        }else if (repeat == 1){scanCV(); repeat = -1;}       
    }else {Serial.println("Communication stopped"); Serial.end();};
    
}

int getParameter(String record){
  int out;
  while (Serial.available() == 0);
  {
    out = Serial.parseInt();
    Serial.print(record); Serial.println(out);
    while (Serial.available()> 0)
    { junk = Serial.read(); }
    return(out);
  }  
}

int convertVol(int convert){
  // code the mV to be converted to the nearest 0-255 value in -1000 to 1000 range
  int conv2 = convert + 1000;
  double resolution = 2000.0/255.0;
  double conv3 = (((double)conv2))/resolution;
  int conv4 = round(conv3); 
  return(conv4);
  
}

double convertDigital(int convert){
  //code the digital value to be converted back to mV
  double conv1 = convert*res;
  double conv2 = conv1 - 1000;
  return(conv2);
}

void scanCV(){
  
  //forward scan start
      for(val = low; val <= high; val++){
        analogWrite(a,val); //writes analog PWM value
        delay((1000/(scan/res))); //pauses for delay (ms)
        c = analogRead(ct); //current measurement
        Serial.print(val); Serial.print(",");Serial.print(c); Serial.print(","); Serial.println(resistor);
        while (((c == 1023) && (resCount<13)) || ((c == 0) && (resCount<13))){
          resCount = changeRes(resCount);
          c = analogRead(ct);
          Serial.print(val); Serial.print(",");Serial.print(c); Serial.print(","); Serial.println(resistor);
        }
      }
  //reverse scan start
      for(val = high; val >= low; val--){
        analogWrite(a,val); //writes analog PWM value
        delay(1000/(scan/res)); //pauses for delay required to fufill scan rate (ms)
        c = analogRead(ct); //current measurement
        Serial.print(val); Serial.print(",");Serial.println(c); Serial.print(","); Serial.println(resistor);
        while (((c == 1023) && (resCount<13)) || ((c == 0) && (resCount<13))){
          resistor = changeRes(resCount);
          c = analogRead(ct);
          Serial.print(val); Serial.print(",");Serial.print(c); Serial.print(","); Serial.println(resistor);
        }
      }
}

void truthSelect(int s0T, int s1T, int s2T, int s3T, int enT){
  truthWrite(en, enT);
  truthWrite(s0, s0T);
  truthWrite(s1, s1T);
  truthWrite(s2, s2T);
  truthWrite(s3, s3T);
}

void truthWrite(int pin, int truth){
  if (truth == 1){
    digitalWrite(pin, HIGH);
  } else{
    digitalWrite(pin, LOW);
  }
}


int changeRes(int count){
  int counter = count + 1;
  if (counter == 1){
    truthSelect(1,0,0,0,0);
    resistor = 4700000;  
  } else if (counter == 2){
    truthSelect(0,1,0,0,0);
    resistor = 2000000;
  } else if (counter == 3){
    truthSelect(1,1,0,0,0);
    resistor = 1000000;
  } else if (counter == 4){
    truthSelect(0,0,1,0,0);
    resistor = 470000;
  } else if (counter == 5){
    truthSelect(1,0,1,0,0);
    resistor = 220000;
  } else if (counter == 6){
    truthSelect(0,1,1,0,0);
    resistor = 100000;
  } else if (counter == 7){
    truthSelect(1,1,1,0,0);
    resistor = 47000;
  } else if (counter == 8){
    truthSelect(0,0,0,1,0);
    resistor = 20000;
  } else if (counter == 9){
    truthSelect(1,0,0,1,0);
    resistor = 10000;
  } else if (counter == 10){
    truthSelect(0,1,0,1,0);
    resistor = 5100;
  } else if (counter == 11){
    truthSelect(1,1,0,1,0);
    resistor = 2200;
  } else if (counter == 12){
    truthSelect(0,0,1,1,0);
    resistor = 1000;
  } else{
    truthSelect(1,0,1,1,0);
    resistor = 390;
  }
  return counter;
}
