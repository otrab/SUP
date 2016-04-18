#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//LED RGB
int pin_red = 9; 
int pin_green = 10; 
int pin_blue = 6; 
long segundos_restantes; 
unsigned long tiempo_color;
unsigned long tiempo_pre_color=0;

//BUZZER
int buz=A0;
//RELE
int RELE=A1; //8 o 12 es el otro

//CHECKSUM DE LAS TARJETAS
int chk1=216;
int chk2=240;
int master_id=169;

// RTC variables
byte second;        // 0-59
byte minute;       // 0-59
byte hour;          // 1-23
byte dayOfWeek;     // 1-7
byte dayOfMonth;    // 1-28/29/30/31
byte month;         // 1-12
byte year;
char date[11];
char time[9];

//RFID Variables
int vo_rfid=5; 
char buffer[64];
//Variable global para identificar el codigo
int user_chk=0;
//Variable global para identificar usuario
int usuario = 0;
//Es necesario reiniciar el rfid para detecta que se mantiene el tag
int reset = 8;
//Variable global para activar o desactivar usuario. Tambien indica las tandas
int usuario1_activar=1; //EEPROM 3
int usuario2_activar=1; //EEPROM 4
//Variables para llevar el tiempo restante
int tiempo_juego=5;
int minutos_juego1=0; //EEPROM 1
int minutos_juego2=0; //EEPROM 2
int pausa=0;
int minutos_stop1=0; 
int minutos_stop2=0; 
//Variable para comparar minuto actual con el anterior
int minuto_ant=0;

//EEPROM VARIABLES
int dia_mes; //EEPRTOM 0

//RELE
int forzar_apagado=0;
