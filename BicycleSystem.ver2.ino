/*
 * digital =1 ------analog 548 미만 (평소 510)
 * digital =0 ------analog 548 이상 
 * DTIME 20, analog 값 520 고정
 * 자전거 반지름 : 33CM
 * 자석까지 반지름 : 25CM
*/

//v[km/h] = l[km] / t[h] = 3600 * l[mm] / 1,000,000 * t[s]
//cadence[회/분] = 1 / t[분] = 60 / t[s] = 1,000,000 * v[km/h] / (60 * l[mm])

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2,3); //Tx,Rx

#define AOUT A0
#define DOUT 4
#define BAUD 9600 
#define DTIME 100
#define WAITING 3000

void setup() {
  pinMode(DOUT,INPUT);
  pinMode(AOUT,INPUT);
  Serial.begin(BAUD);
  BTSerial.begin(BAUD); //블루투스
  Serial.println("Hall_Sensor Start");
}

int Avalue;
int Dvalue;
double Cycle_Time=0;
bool Zero_Flag=0;
unsigned long Current_time=0;
unsigned long Prev_time=0;
double Round=2073;//(MM)
unsigned long Result;

void loop() {
 
 Dvalue = digitalRead(DOUT);
 Avalue = analogRead(AOUT);
 Current_time = millis();
 Cycle_Time=Current_time - Prev_time;

 if(Avalue>=548) {
    Zero_Flag=0;
    if(Cycle_Time>=DTIME){   
        BTSerial.print((3600*Round)/(1000000*Cycle_Time/1000));
        BTSerial.print('\n');
      
        Serial.print(Cycle_Time/1000);
        Serial.print("   ");
        Serial.print((3600*Round)/(1000000*Cycle_Time/1000));
        Serial.println("KM/h");
    }
   Prev_time=Current_time;
 }
 
 else{
      if(Cycle_Time>=WAITING && Zero_Flag==0){    
         BTSerial.print(0);
         BTSerial.print('\n'); 
         Serial.print(Cycle_Time/1000);
         Serial.print("   ");
         Serial.print(0);
         Serial.println("KM/h");
         Prev_time=0;
         Zero_Flag=1;
    }
 }
}
