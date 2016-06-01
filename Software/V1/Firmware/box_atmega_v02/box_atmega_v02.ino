//RTC Y RELE
#include "Metro.h" //Include Metro library
//RTC
#include <string.h>
#include <Wire.h>
//EPROM
#include <EEPROM.h>

//SERIAL DEL RFID
#include <SoftwareSerial.h>

//El RX es usado para leer el RFID. El TX es usado para debugear
SoftwareSerial mySerial(2, A2); // RX, TX
//SoftwareSerial mySerial(2, 3); // RX, TX

//SoftwareSerial Serial_debug(A3, A2); // RX, TX

Metro ledMetro = Metro(5000); 

//MUSICA
#include "globales.h"
#include "Music2.h"
#include "Rtc.h"
#include "Rfid.h"

//Utilizado para el control del rele
Metro releMetro = Metro(2000); 
int state = LOW;

//Variables para controlar el tiempo de juego
Metro minutoMetro = Metro(5000); 

void setup() {
  analogWrite(pin_blue, 100);
  //Configurarcion LED RGB
  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);
  
  //SERIAL DEBUG
  Serial.begin(115200);    
  //Serial.println("En Setup");

  //Sonid BUZZER
  pinMode(buz, OUTPUT);
  //MUSICA
  play();
  delay(250);
  digitalWrite(buz, LOW); 
  state=LOW;
  delay(1000);
  
  //Reset necesario para el RFID
  pinMode(vo_rfid, OUTPUT);
  digitalWrite(vo_rfid, LOW);  
   
  //SERIAL RFID
  mySerial.begin(9600);  // connect to the serial port  

  //sw Serial Debug
  mySerial.println("SW debug");
  
  //RELE
  pinMode(RELE,OUTPUT);
  digitalWrite(RELE,!state);  
  // Wire RTC
  Wire.begin();
  // RTC init
  //Para volver a setear la hora y fecha hay que quitar el if y el WriteDs1307
  if(ReadDs1307(0x08) != 0xaa)
  {
    second = 0;
    minute = 42;
    hour = 16;
    dayOfWeek = 1;
    dayOfMonth = 4;
    month = 3;
    year = 13;
    SetDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);   
    WriteDs1307(0x08,0xaa); 
  }
  
  //Si es otro dia lo almaceno en eeprom y reinicio los minutos de juego a cada jugador en variable de programa y en EEPROM
  //delay(10000);
  resetear_tiempo_dia();

  /*
  Serial.println("Esperando byte para reiniciar.");
  delay(2000);
  if (Serial.available() > 0){
    //ASIGNO UN DIA DEL MES QUE NO EXISTE PARA RESETAEAR TIEMPO
    //  Esto ocurre en la funcion void resetear_tiempo_dia(void) del archivo Rtc.h
    EEPROM.write(0, 50); 
    resetear_tiempo_dia();
    Serial.println("Resetear");
  }
  */
  
  minutos_juego1 = EEPROM.read(1);
  minutos_juego2 = EEPROM.read(2);
  usuario1_activar = EEPROM.read(3);
  usuario2_activar = EEPROM.read(4);



  //Con esta opcion siempre reestablecia el tiempo maximo para la luz
  //segundos_restantes = tiempo_juego*60;

  segundos_restantes = minutos_juego1*60;

  

}

/* Funcion para pasar de verde a rojo en funcion del tiempo de juego que quede
 * Debe leer los minutos de juego total  y restante 
 * dividir minutos_juego * 60 / 255 , para obtener cada cuantos segundos cambiar en 1 un color
 * Debe llevar una cuenta de los segundos transcurridos entre cada llamada.
 * Tambien emite sonido el ultimo minuto
*/

void cambiar_colores(){
  GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
  tiempo_color = second;
  //tiempo_color=millis();
  mySerial.println();
  mySerial.print("tiempo_pre_color: ");
  mySerial.println(tiempo_pre_color);
  mySerial.print("tiempo_color: ");
  mySerial.println(tiempo_color);

  int segundos_transcurridos=0;
  if(tiempo_color>tiempo_pre_color)segundos_transcurridos = tiempo_color-tiempo_pre_color;
  //Cuando es otro minuto. Ej: 15:31:59 y 15:32:02 -> dif = 3 
  else if(tiempo_color<tiempo_pre_color)segundos_transcurridos = (60-tiempo_pre_color)+tiempo_color;
  //int segundos_transcurridos = (tiempo_color-tiempo_pre_color)/1000;
  //analogWrite(pin_red, 255);  //digitalWrite(pin_red, HIGH);
  //analogWrite(pin_green, 255);  //digitalWrite(pin_green, LOW);
  analogWrite(pin_blue, 0);  //digitalWrite(pin_blue, LOW);
  
  //Trabajo en segundos
  if (segundos_transcurridos>0)
  {
    mySerial.print("segundos_transcurridos: ");
    mySerial.println(segundos_transcurridos);
    if(segundos_restantes>0)
    {
      segundos_restantes=segundos_restantes - segundos_transcurridos;
      if(segundos_restantes<0)segundos_restantes=0;
    }
    mySerial.print("segundos_restantes2: ");
    mySerial.println(segundos_restantes);

    int verde=map(segundos_restantes, 0, tiempo_juego*60, 0, 255);
    int rojo=255-verde;
    
    /*
    Serial.print("rojo: ");
    Serial.println(rojo);
    Serial.print("verde: ");
    Serial.println(verde);
    */

    analogWrite(pin_red, rojo);  //digitalWrite(pin_red, HIGH);
    
    //El verde es muy fuerte en comparacion al rojo
    verde=verde-25;
    if(verde<0)verde=0;
    analogWrite(pin_green, verde);  //digitalWrite(pin_green, LOW);

    //Sonido 
    if (segundos_restantes<60)
    {
      analogWrite(buz,20); //emite sonido
      delay(50);
      digitalWrite(buz, LOW);
      delay(50);
      analogWrite(buz,20); //emite sonido
      delay(50);
      digitalWrite(buz, LOW);
    }
    tiempo_pre_color=tiempo_color;
  }
}

/* Funcion para debuguear por consola
*   Es llamada al poner una tarjeta en la caja
*   Imprime cuantos minutos tiene el jugador
*/
void imprimir_date(void){
  Date2Str(date);
  Time2Str(time);
  //IMPRIMO FECHA
  for(int i = 0; i<10; i++)mySerial.print(date[i]);
  mySerial.print(" | ");
  //IMPRIMO HORA
  for(int i = 0; i<8; i++)mySerial.print(time[i]);
  mySerial.println("");
  //IMPRIMO MINUTOS DE JUGADOR 1        
  if(usuario==1){
    mySerial.print("A enzo le quedan: ");
    mySerial.print(minutos_juego1);
    mySerial.println(" minutos");
  }
  //IMPRIMO MINUTOS DE JUGADOR 2  
  if(usuario==2){
    mySerial.print("A vixo le quedan: ");
    mySerial.print(minutos_juego2);
    mySerial.println(" minutos");
  }  
}

/*  Funcion para descontar 1 minuto de juego
*     Es llamada cada un minuto siempre que el jugador este jugando
*     Refresca la variable global minuto_ant
*     Resta un minuto de la variable minuto_juego y guarda en eprom
*/
void quitar_minuto(void){
  mySerial.println("Descontar minuto de juego?");
  //Actualizo el minuto
  GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
  minuto_ant=minute;     
  //Resto minutos y modifico valor en EEPROM cada 5 minutos
  if(usuario==1)mySerial.print("Enzo ");
  if(usuario==2)mySerial.print("Vixo ");
  mySerial.print("Menos un minuto: ");
  if(usuario==1){
    minutos_juego1=minutos_juego1-1;
    mySerial.println(minutos_juego1);
    //la operacion modulo (%) esta en caso de que decida no escribir cada 1 minuto en la eprom
    if(minutos_juego1!=0&&minutos_juego1%1==0){
      mySerial.println("A EEPROM J1");
      EEPROM.write(1, minutos_juego1);
    }
  }
  if(usuario==2){
    minutos_juego2=minutos_juego2-1;
    mySerial.println(minutos_juego2);
    if(minutos_juego2!=0&&minutos_juego2%1==0){
      mySerial.println("A EEPROM J2");
      EEPROM.write(2, minutos_juego2);
    }
  }
}

/*  Funcion que bloquea la tarjeta.
*     Resta una tanda de usuario1_activar y guarda en eprom. Usuario1_activar en 0 es la condicion que bloquea el rele
*     Ademas de restar una tanda, asigna pausa y restituye tiempo para la tanda siguiente
*     Es llamada cuando se acaban los minutos de juego
*/
void desactivar_tarjeta(void){
  mySerial.println("Fin de tu tiempo");
  if(usuario==1){
    if(usuario1_activar>0){
      usuario1_activar=usuario1_activar-1;
      EEPROM.write(3, usuario1_activar);
      EEPROM.write(1, tiempo_juego);
      minutos_juego1=tiempo_juego;
      if(usuario1_activar!=0){
        mySerial.println("Otra tanda a ENZO");
        minutos_stop1=pausa;
      }
      else mySerial.println("Fin tandas a ENZO");
      
    }
  }
  if(usuario==2){
    if(usuario2_activar>0){
      usuario2_activar=usuario2_activar-1;
      EEPROM.write(4, usuario2_activar);
      EEPROM.write(2, tiempo_juego);
      minutos_juego2=tiempo_juego;
      if(usuario2_activar!=0){
        mySerial.println("Otra tanda a VIXO");
        minutos_stop2=pausa;
      }
      else mySerial.println("Fin tandas a VIXO");
    }
  }
}

/* Esta funcion se ocupa en caso de que se deseen que se tomen pausas al jugar. 
*   Por ejemplo: Podria setearle a un niño 2 tandas de 1 hora, con pausa obligada de al menos 15 minutos.
*   Si bien esto está codificado, no se ofrecerá en el primer MVP.
*/
int tiempo_pausa=0; //Variable temporal para descontar de 1 minuto y no menos. Solo se ocupa en esta funcion
void descuento_pausa(void){
  //DESCUENTO TIEMPO STOP JUGADOR 1
  if(minutos_stop1>0){
    GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
    //Solo cada 1 minuto descuento
    if(tiempo_pausa!=minute){
      mySerial.println("PAUSA DE ENZO");
      minutos_stop1=minutos_stop1-1;
      tiempo_pausa=minute; 
    }
  }
  //DESCUENTO TIEMPO STOP JUGADOR 2
  if(minutos_stop2>0){
    GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
    //Solo cada 1 minuto descuento
    if(tiempo_pausa!=minute){
      mySerial.println("PAUSA DE VIXO");
      minutos_stop2=minutos_stop2-1;
      tiempo_pausa=minute; 
    }
  }
}

int target=1;

int segundo_inicio=0;

void loop () { 
  //Este while esta para evitar la interrupcion que hace el main para leer el serial
  // Puede que haya que cambiarlo, dado que hay que estar atento al serial (HW o SW) conectado al ESP
  while(true){
    int target=0;
    
    //SI HAY TARGETA RETORNA 0 Y ALCMACENA EL NUMERO EN user_chk
    target=leer_rfid();
    if(!target)user_chk=chk_rfid();

    
    
    //SI HAY TARJETA
    if (target==0)
    {
      digitalWrite(buz, LOW);
      mySerial.print("Tarjeta: ");
      mySerial.println(user_chk);
      
      if(user_chk==master_id)
      {
        resetear_tiempo_dia();
        EEPROM.write(0, 50); 
        usuario1_activar=1;
        resetear_tiempo_dia();
        mySerial.println("Resetear");

        minutos_juego1 = EEPROM.read(1);
        minutos_juego2 = EEPROM.read(2);
        usuario1_activar = EEPROM.read(3);
        usuario2_activar = EEPROM.read(4);

        //Para la luz
        segundos_restantes = minutos_juego1*60;
        /*
        Serial.print("usuario1_activar0: ");
        Serial.println(usuario1_activar);
        Serial.print("segundos_restantes0: ");
        Serial.println(segundos_restantes);
        Serial.println();
        */

        //Hago parpadear la luz azul
        analogWrite(pin_blue, 255);
        delay(50);
        analogWrite(pin_blue, 0);
        delay(50);
        analogWrite(pin_blue, 255);
        delay(50);
        analogWrite(pin_blue, 0);
        delay(50);
        analogWrite(pin_blue, 100);
        
      }
      
     //Ve si la tarjeta corresponde a uno de los 2 usuarios, si éste está activo y no tiene restricción por pausa entre tandas (minutos_stop)
      if((user_chk==chk1&&usuario1_activar&&minutos_stop1==0)||(user_chk==chk2&&usuario2_activar&&minutos_stop2==0)){
        //Cuando recien se detecta la tarjeta del jugador
        if(segundo_inicio==-1){
          GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
          segundo_inicio=second; // -1 indica que no esta seteado
          tiempo_pre_color=second;
          //Para que coincida la luz con el apagado efectivo
          segundos_restantes = minutos_juego1*60;
          //Señal luminica al detectar tarjeta
          analogWrite(pin_green, 255);
          
          
        }
        cambiar_colores();
        //Prendo Rele
        state=HIGH;
        mySerial.println("Rele HIGH");
        //Si la wii estaba apagada, señalo usuario
        if(usuario==0)
        {    
          //SEÑALO USUARIO
          if(user_chk==chk1){
            usuario=1;
          }
          //OBTENGO TIEMPO
          GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
          //GUARDO EL MINUTO PREVIO
          minuto_ant=minute;
          imprimir_date();
        }      
        //Reviso el tiempo
        GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
        //imprimir_date();
        //Descuento un minuto de juego
        if (minuto_ant!=minute)
        {
          mySerial.print("minute: ");
          mySerial.println(minute);
          mySerial.print("second: ");
          mySerial.println(second);
          mySerial.print("segundo_inicio: ");
          mySerial.println(segundo_inicio);
          if (second>segundo_inicio)
          {
            quitar_minuto();
            //Bip al pasar cada minuto
            analogWrite(buz,20); //emite sonido
            delay(50);
            digitalWrite(buz, LOW);
            delay(50);
            analogWrite(buz,20); //emite sonido
            delay(100);
            digitalWrite(buz, LOW);
          }
        }
        
        //Desactivo tarjeta si se acabó el tiempo
        if((minutos_juego1<=0&&usuario==1)||(minutos_juego2<=0&&usuario==2)){
          desactivar_tarjeta();   
          forzar_apagado=1;
          delay(2000);
          /*
          analogWrite(buz,20); //emite sonido
          delay(50);
          digitalWrite(buz, LOW);
          delay(50);
          */
          analogWrite(buz,20); //emite sonido
          delay(1000);
          digitalWrite(buz, LOW);
        }
        //Actualizo la cuenta regresiva para que no se apague
        releMetro.reset();
      }    
      //Reseteo el rfid para poder volver a leer la tarjeta
      reset_rfid();
      //Actualizo la cuenta regresiva para que no se apague
      releMetro.reset();
    }
    
    //Revisa si hay que apagar, ya sea porque se retiró por algunos segundos la tarjeta o porque se acabo el tiempo (flag forzar_apagado)
    if (releMetro.check() == 1 || forzar_apagado){
      segundo_inicio=-1; //Para contar los minutos solo al pasar por el segundo de inicio
      forzar_apagado = 0;
      usuario=0;
      mySerial.print(usuario);
      mySerial.println("Apagar");
      analogWrite(pin_red, 0);
      analogWrite(pin_green, 0);
      analogWrite(pin_blue, 100);
      state=LOW;
      //releMetro.reset();
    }
    
    //DESCUENTO EL TIEMPO ENTRE TANDAS
    descuento_pausa();
     
    //CONTROL SOBRE EL RELE
    digitalWrite(RELE,!state);
    //Pausa
    delay(100);

  }
  
}
