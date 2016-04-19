#include <SoftwareSerial.h>
#include <Wire.h>

#define RELE2 A2

SoftwareSerial debugSerial(A3, RELE2);

void setup()
{
  Wire.begin();
  debugSerial.begin(115200);
  debugSerial.println("\nI2C Scanner");
}
 
 
void loop()
{
  byte error, address;
  int nDevices;
 
  debugSerial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      debugSerial.print("I2C device found at address 0x");
      if (address<16)
        debugSerial.print("0");
      debugSerial.print(address,HEX);
      debugSerial.println("  !");
       if(address==0x68){
        debugSerial.println("RTC detected");
      }
      nDevices++;
    }
    else if (error==4)
    {
      debugSerial.print("Unknow error at address 0x");
      if (address<16)
        debugSerial.print("0");
      debugSerial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    debugSerial.println("No I2C devices found\n");
  else
    debugSerial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}
