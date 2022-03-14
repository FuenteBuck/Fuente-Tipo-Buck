 /*
  Programa creado para la medicion del voltaje de salida de una fuente de voltaje DC-DC
  conectada a una ESP8266 para el control de voltaje 
*/

#include "Wire.h"
#include "Adafruit_INA219.h"
Adafruit_INA219 ina219;//Se crea el objeto INA219

unsigned int Ts=10000;//[us]
unsigned int t, t0;
float u_past;
float e_past;
float r=5;
float e=0;
float y=0;
float kp=10;
int u;

void setup() 
{
  Serial.begin(115200);//Se inicia el monitor serie 
  uint32_t currentFrequency;
  ina219.begin();
  t0=micros();
  ledcSetup(0,5000,16);
  ledcAttachPin(4,0);
  ledcWrite(0,0);
  u=65150;
  u_past=u;
}

void loop() 
{

   t=micros();// El valor del registro actual en el tiempo 
    if(t-t0>=Ts){// Se debe mantener la diferencia
      Referencia();
      PWM_C(r);
      t0=t;
   }
   if(t0>t){
     Referencia();
     PWM_C(r);
     t0=t;
   }
}

void Referencia(){
      if (Serial.available()) { 
         String command = Serial.readStringUntil('\n');
         r = command.toFloat();
            if(r<2.7){
                r=2.7;
            }
            if(r>11.5){
                r=11.5;
            }
      }
}

void PWM_C(float r){
  
  float shuntvoltage=0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
 
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();

loadvoltage = busvoltage + (shuntvoltage / 1000);
Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
Serial.print(" \n");
y=loadvoltage;
e=r-y;
ledcWrite(0,u);
u=u_past-250*e;
ledcWrite(0,u);
e_past=e;
u_past=u;
}  


  


 
