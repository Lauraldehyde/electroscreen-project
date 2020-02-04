int randInt = 0; char junk = ' ';

int high = -1; int low = -1; int scan = -1; int repeat = -1; int go = -1; double res = 2000.0/255.0;

String hString = "The voltage high (mV) is: "; String lString = "The voltage low (mV) is: "; String sString = "The scan rate (mV/s) is: ";
    String rString = "Number of scans is :"; String goString = "Assay initialized. ";
    
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello, please input parameters");
  Serial.flush();

   //get parameters
  high = convertVol(getParameter(hString));  low = convertVol(getParameter(lString));  scan = getParameter(sString);
      repeat = getParameter(rString);  go = getParameter(goString);

   //show parameter in digital selected
  Serial.println("Digital parameters set:");
  Serial.print("High volatage: "); Serial.print(convertDigital(high)); Serial.print(" mV ");  Serial.print("Low voltage: "); 
      Serial.print(convertDigital(low)); Serial.print(" mV ");  Serial.print("Scan rate: "); Serial.print(scan); Serial.print(" mV/s ");
      Serial.print("Repeats: "); Serial.println(repeat);

  if(high != -1 && low != -1 && scan != -1 && repeat != -1 && go != -1){
     
    }else {(Serial.println("Parameters not input"));}
    delay(500);


}

void loop() {
  // put your main code here, to run repeatedly:
    if(high != -1 && low != -1 && scan != -1 && repeat != -1){
        if (repeat > 1){imitateScan(); repeat--;
        }else if (repeat == 1){imitateScan(); repeat = -1;}       
    }else {Serial.println("Communication stopped"); Serial.end();};

  

}


int getParameter(String record){
  int out;
  byte bBuffer[50];
  String full;
  while (Serial.available() == 0);
  {
    full = Serial.readStringUntil("\r");
    out=full.toInt();
    Serial.print(record); Serial.print(full); Serial.print(" , "); Serial.println(out);
    while (Serial.available()> 0)
    { junk = Serial.read(); }
    return(out);
  }  
}

double convertDigital(int convert){
  //code the digital value to be converted back to mV
  double conv1 = convert*res;
  double conv2 = conv1 - 1000;
  return(conv2);
}

int convertVol(int convert){
  // code the mV to be converted to the nearest 0-255 value in -1000 to 1000 range
  int conv2 = (-1 * convert) + 1000; //added after seeing opposite than literature in results of exp-103-023 Pt wire CE trials
  double resolution = 2000.0/255.0;
  double conv3 = (((double)conv2))/resolution;
  int conv4 = round(conv3); 
  return(conv4);
  
}

void imitateScan(){
  //forward scan
  int val;
  for(val=high; val <= low; val++){
    randInt = random(0,1024);
    delay((1000/(scan/res)));
    Serial.print(val); Serial.print(","); Serial.println(randInt);
  }
  for(val = low; val >= high; val--){
        randInt = random(0,1024);
        delay(1000/(scan/res)); //pauses for delay required to fufill scan rate (ms)
        Serial.print(val); Serial.print(",");Serial.println(randInt);
      }
}
