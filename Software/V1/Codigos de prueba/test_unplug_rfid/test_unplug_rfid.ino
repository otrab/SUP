#include <SoftwareSerial.h>
#define RFID_TX 2
#define RFID_PW 5
#define RELE2 A2

SoftwareSerial debugSerial(A3, RELE2);
SoftwareSerial rfidSerial(RFID_TX, A6);

void setup() {
  // put your setup code here, to run once:
  debugSerial.begin(115200);
  rfidSerial.begin(9600);
  pinMode(RFID_PW,OUTPUT);
}

void loop() {
  debugSerial.println("rfid on");
  // put your main code here, to run repeatedly:
  digitalWrite(RFID_PW,LOW);
  unsigned long t1=millis();
  while(millis()-t1<5000){
    if(rfidSerial.available()>0){
      debugSerial.write(rfidSerial.read());
    }
  }
  debugSerial.println("rfid off");
  digitalWrite(RFID_PW,HIGH);
  delay(5000);
}
