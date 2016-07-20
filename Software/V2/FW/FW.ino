#include "RTClib.h"
#include <SoftwareSerial.h>

#define RFID_SS_3V3 8
#define RFID_OC_NRST 7
#define BUZZER A0
#define RELE A1
#define BOTON A7

SoftwareSerial debugSerial(3, 4);

void setup() {
	debugSerial.begin(115200);
}
enum Estado {bloqueado, desbloqueado, user, master};
Estado estado = bloqueado;

class Usuario {
public:
	Usuario() {
		id = 66;
	}
	byte id;
	byte master;
	byte nuidPICC[4];
	DateTime horario_inicio;
	DateTime horario_fin;
};

boolean intervalo_t(DateTime t, DateTime ti, DateTime tf) {
	return (t.hh > ti.hh || t.mm > ti.mm || t.ss > ti.ss)
	       && (t.hh < tf.hh || t.mm < tf.mm || t.ss < tf.ss);
}
boolean check_nuid(byte * nuid1, byte * nuid2) {
	return nuid1[0] != nuid2[0]
	       || nuid1[1] != nuid2[1]
	       || nuid1[2] != nuid2[2]
	       || nuid1[3] != nuid2[3];
}
boolean leer_tarjeta() {
	if(rfid.PICC_IsNewCardPresent()) {
		if(rfid.PICC_ReadCardSerial()) {
			return 1;
		}
	}
	return 0;
}

byte nusers=4;
Usuario users[4];
DateTime tiempo;

void loop() {
    //Lecturas de tarjetas, tiempo del RTC, etc...
	tarjeta = leer_tarjeta();
    //Transiciones de estados, no leer nada aqui, usar solo variables fijas.
	if(estado == bloqueado) {
        //Para cada usuario...
		for(int i = 0; i < nusers; i++) {
            //Ignorar usuarios no creados.
			if(users[i].id != 0) {
                //Transicion a horario permitido
				if(intervalo_t(tiempo, users[i].horario_inicio, users[i].horario_fin)) {
					estado = desbloqueado;
				}
                //Si hay tarjeta puesta...
				if(tarjeta) {
                    //Revisar de quien es la tarjeta
					if(check_nuid(rfid.uid.uidByte, users[i].nuidPICC)) {
                        //Transicion a modo master o revisar el tiempo restante del ninho
						if(users[i].master) {
							estado = master;
						} else {
                            //Revisar el tiempo restante
						}
					}
				}
			}
		}
	} else if(estado == desbloqueado) {
	} else if(estado == user) {
	} else if(estado == master) {
	}
}
