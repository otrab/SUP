#include <SPI.h>
#include "MFRC522.h"
#include <SoftwareSerial.h>

SoftwareSerial debugSerial(PD3, PD4);

#define RFID_SS_3V3 PB0
#define RFID_OC_NRST PD7

MFRC522 rfid(RFID_SS_3V3, RFID_OC_NRST); // Instance of the class

// Init array that will store new NUID
byte nuidPICC[4];

void setup() {
  debugSerial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

void loop() {

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    rfid.uid.uidByte[1] != nuidPICC[1] ||
    rfid.uid.uidByte[2] != nuidPICC[2] ||
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    debugSerial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    debugSerial.println(F("The NUID tag is:"));
    debugSerial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    debugSerial.println();
  }
  else {
      debugSerial.println(F("Card read previously."));
      rfid.PCD_Init();
      return;
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    debugSerial.print(buffer[i] < 0x10 ? " 0" : " ");
    debugSerial.print(buffer[i], HEX);
  }
}
