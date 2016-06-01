/*
 * Este programa es para probar que el RTC está funcionando correctamente
 * Abrir el Serial a 9600. Comprobar que la hora tiene un formato adecuado y que el tiempo avanza.
 */

#include <Wire.h>
#include "DS1307.h"

DS1307 clock;

void setup() {
  Serial.begin(9600);
  //Pines por defecto al hacer Wire.begin() son GPIO4/D2 (SDA) and GPIO5/D1 (SCL). Wire.begin() se hace na la linea 42 del DS1307.cpp
  clock.begin();
  clock.fillByYMD(2013,1,19);//Jan 19,2013
  clock.fillByHMS(15,28,30);//15:28 30"
  clock.fillDayOfWeek(SAT);//Saturday
  clock.setTime();//write time to the RTC chip
}

void loop() {
  Serial.println("Hola mundo");
  printTime();
  delay(1000);
}

/*Function: Display time on the serial monitor*/
void printTime()
{
  clock.getTime();
  Serial.print(clock.hour, DEC);
  Serial.print(":");
  Serial.print(clock.minute, DEC);
  Serial.print(":");
  Serial.print(clock.second, DEC);
  Serial.print("  ");
  Serial.print(clock.month, DEC);
  Serial.print("/");
  Serial.print(clock.dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(clock.year+2000, DEC);
  Serial.print(" ");
  Serial.print(clock.dayOfMonth);
  Serial.print("*");
  switch (clock.dayOfWeek)// Friendly printout the weekday
  {
    case MON:
      Serial.print("MON");
      break;
    case TUE:
      Serial.print("TUE");
      break;
    case WED:
      Serial.print("WED");
      break;
    case THU:
      Serial.print("THU");
      break;
    case FRI:
      Serial.print("FRI");
      break;
    case SAT:
      Serial.print("SAT");
      break;
    case SUN:
      Serial.print("SUN");
      break;
  }
  Serial.println(" ");
}

