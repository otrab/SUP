//TODO: ESP

#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>

#include "WS2812.h"
#include "MFRC522.h"
#include "RTClib.h"

#define RFID_SS_3V3 8
#define RFID_OC_NRST 7
#define BUZZER A0
#define RELE A1
#define BOTON A7

RTC_DS1307 rtc;
MFRC522 rfid(RFID_SS_3V3, RFID_OC_NRST);
WS2812 LED(7);
SoftwareSerial debugSerial(3, 4);

cRGB value;
int f = 4000;

void setup() {
	pinMode(9, OUTPUT);
	LED.setOutput(9);     // Digital Pin 9
	LEDoff();
	Serial.begin(57600);
	debugSerial.begin(115200);
	debugSerial.println(F("\n\nTest FW setup()"));
	Wire.begin();
	SPI.begin(); // Init SPI bus
	pinMode(BUZZER, OUTPUT);
	pinMode(RELE, OUTPUT);
	digitalWrite(RELE, HIGH);
}
//boton -> 700 a 0 (presion)
void loop() {
	debugSerial.print(F("[Buzzer]"));
	tone(BUZZER, f);
	delay(50);
	noTone(BUZZER);
	debugSerial.println(F(" BIP!"));
	delay(500);
	debugSerial.print("[LED]");
	debugSerial.println(" Flashing");
	for(int k = 0; k < 1; k++) {
		for(int i = 0; i < 360; i++) {
			for(int j = 0; j < 7; j++) {
				value.SetHSV(i, 100, 100);
				LED.set_crgb_at(j, value);
				LED.sync();
				delayMicroseconds(500);
				if(i == 0) {
					delay(100);
				}
			}
		}
	}
	for(int j = 6; j >= 0; j--) {
		value.SetHSV(0, 0, 0);
		LED.set_crgb_at(j, value);
		LED.sync();
		delay(101);
	}
	byte error, address;
	int nDevices;
	debugSerial.print(F("[I2C] Escaneando"));
	delay(100);
	bool rtcd = 0;
	nDevices = 0;
	for(address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if(error == 0) {
			debugSerial.print(F(" -> Address 0x"));
			if(address < 16) {
				debugSerial.print(F("0"));
			}
			debugSerial.print(address, HEX);
			if(address == 0x68) {
				debugSerial.print(F(" -> RTC"));
				rtcd = 1;
			} else {
				debugSerial.println();
			}
			nDevices++;
		} else if(error == 4) {
			debugSerial.print(F("Error desconocido en 0x"));
			if(address < 16) {
				debugSerial.print(F("0"));
			}
			debugSerial.println(address, HEX);
		}
	}
	if(nDevices == 0) {
		debugSerial.println(F(" -> No hay dispositivos"));
	}
	if(rtcd) {
		rtc.begin();
		if(!rtc.isrunning()) {
			debugSerial.print(" (Ajustando hora)");
			// following line sets the RTC to the date & time this sketch was compiled
			rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
			// This line sets the RTC with an explicit date & time, for example to set
			// January 21, 2014 at 3am you would call:
			// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
		}
		DateTime now = rtc.now();
		debugSerial.print(" -> t = ");
		debugSerial.print(now.hour(), DEC);
		debugSerial.print(':');
		debugSerial.print(now.minute(), DEC);
		debugSerial.print(':');
		debugSerial.print(now.second(), DEC);
		debugSerial.println();
	}
	debugSerial.print(F("[Rele]"));
	int t = 300;
	digitalWrite(RELE, LOW);
	delay(t);
	debugSerial.print(F(" CLICK!"));
	digitalWrite(RELE, HIGH);
	delay(t);
	debugSerial.print(F(" CLICK!"));
	digitalWrite(RELE, LOW);
	delay(t);
	debugSerial.print(F(" CLICK!"));
	digitalWrite(RELE, HIGH);
	delay(t);
	debugSerial.println();
	delay(500);
	byte nuidPICC[4];
	rfid.PCD_Init();
	debugSerial.print(F("[RFID] Activado"));
	rfid.PCD_SetAntennaGain(0x70);
	while(1) {
		// Look for new cards
		if(!rfid.PICC_IsNewCardPresent()) {
			continue;
		}

		// Verify if the NUID has been readed
		if(!rfid.PICC_ReadCardSerial()) {
			continue;
		}
		if(rfid.uid.uidByte[0] != nuidPICC[0] ||
		   rfid.uid.uidByte[1] != nuidPICC[1] ||
		   rfid.uid.uidByte[2] != nuidPICC[2] ||
		   rfid.uid.uidByte[3] != nuidPICC[3]) {
			debugSerial.print(F(" -> Tarjeta detectada"));
			tone(BUZZER, f);
			delay(50);
			noTone(BUZZER);
			// Store NUID into nuidPICC array
			for(byte i = 0; i < 4; i++) {
				nuidPICC[i] = rfid.uid.uidByte[i];
			}

			debugSerial.print(F(" (NUID ="));
			printHex(rfid.uid.uidByte, rfid.uid.size);
			debugSerial.print(F(")"));
		} else {
			debugSerial.println(F(" confirmada"));
			tone(BUZZER, f);
			delay(50);
			noTone(BUZZER);
			rfid.PCD_Init();
			break;
		}
		// Halt PICC
		rfid.PICC_HaltA();
		// Stop encryption on PCD
		rfid.PCD_StopCrypto1();
	}
	rfid.PCD_AntennaOff();
	debugSerial.print("[Boton]");
	while(analogRead(BOTON) > 350) {
	}
	debugSerial.println(" -> Presionado");
	debugSerial.println();
}
void printHex(byte *buffer, byte bufferSize) {
	for(byte i = 0; i < bufferSize; i++) {
		debugSerial.print(buffer[i] < 0x10 ? " 0" : " ");
		debugSerial.print(buffer[i], HEX);
	}
}
void LEDoff() {
	value.b = 0;
	value.g = 0;
	value.r = 0;
	for(int i = 0; i < 7; i++) {
		LED.set_crgb_at(i, value);
	}
	LED.sync();
}
