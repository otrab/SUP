/*
 * light_ws2812 example
 *
 * Created: 07.03.2014 12:49:30
 *  Author: Matthias Riegler
 */

#include "WS2812.h"

WS2812 LED(7); // 1 LED

cRGB value;

byte anim[12] = {0b01001001,
	             0b01001000,
	             0b00100101,
                 0b00100100,
	             0b01001001,
                 0b01001000,
	             0b00100101,
                 0b00100100,
	             0b01001001,
                 0b01001000,
	             0b00100101,
                 0b00100100};

void setup() {
	Serial.begin(115200);
	LED.setOutput(9);     // Digital Pin 9
	value.b = 0;
	value.g = 0;
	value.r = 0;
	for(int i = 0; i < 7; i++) {
		LED.set_crgb_at(i, value);
	}
	LED.sync();
}
int n = 12;
unsigned long dt = 1000;
unsigned long t = 0;
unsigned long pt = 0;
unsigned long offset = 0;
int kk=0;
//Fin de millis -> t = 0 + el resto de la division del maximo millis con dt ¿?¿?¿?¿?¿?¿?¿?¿?
void loop() {
	t = millis();
	int tf = t % dt;
	int nf = ((t / dt) % n);
	byte pframe = anim[nf == 0 ? n - 1 : nf - 1];
	byte frame = anim[nf];
	for(int i = 0; i < 7; i++) {
		boolean pbit = bitRead(pframe, i);
		boolean bit = bitRead(frame, i);
		double ti = constrain(3*double(tf) / double(dt),0.0,1.0);
		byte pix = byte((pbit * 255.0 * (1 - ti)) + (bit * 255.0 * ti));
		value.SetHSV(i == 0 ? 0 : 30, 255, i == 0 ? pix : map(pix,0,255,0,200));
		LED.set_crgb_at(i, value);
	}
	LED.sync();
    pt=t;
}
