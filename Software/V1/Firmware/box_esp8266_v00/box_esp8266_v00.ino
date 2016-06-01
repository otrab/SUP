/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

/*
 * Codigo basado en el ejemplo AdvancesWebServer
 * 
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "UnPlugBox";
const char *password = "1234567890";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */

 

void handleRoot() {
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  char pagina[600];
  snprintf(pagina, 600,

"<html>\
  <body>\
  <table border='0'>\
    <form action='set'>\
    <tr>\
      <td>Hora actual</td>\
      <td> %02d:%02d:%02d PM</td>\
    </tr>\
    <tr>\
      <td>Horario diario &nbsp</td>\
      <td><input type='time' min='0' max='24' step='3600' name='c' value='09:00:00'></td>\
      <td>- <input type='time' min='0' max='24' step='3600' name='d' value='19:00:00'></td>\
    </tr>\
    <tr>\
      <td>Tiempo diario</td>\
      <td align='right'><input type='number' min='0' max='24' name='b' value='2'>&nbsp&nbsp&nbsp horas </td>\
      <td align='center'><input type='submit' value='Guardar'></td>\
    </tr>\
    </table>\
    </form>\
  </body>\
</html>",
hr, min % 60, sec % 60);

  
	server.send(200, "text/html", pagina);
}

void set_horario_tiempo(){
  Serial.println("recibido");
  server.send(200, "text/html", "Listo");
  
  //Para no desperdiciar memoria, solo estoy usando el orden de los argumentos. 
  //Podria recuperar los nombres con server.argName
    
  String ini=server.arg(0);
  String fin=server.arg(1);
  String horas=server.arg(2);

  Serial.println(ini);
  Serial.println(fin);
  Serial.println(horas);
}
void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on ( "/set", set_horario_tiempo);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
