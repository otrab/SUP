
 
int speaker = buz;    //porta do arduino
long vel = 20000;



void tocar(int tom, long tempo_value) {
     long tempo_gasto = 0;
     while (tempo_gasto < tempo_value) {
        digitalWrite(speaker, HIGH);
        delayMicroseconds(tom / 2);
 
        digitalWrite(speaker, LOW);
        delayMicroseconds(tom/2);
 
        tempo_gasto += tom;
    }
}

void play(void){
  
Serial.println("Empezo??"); 
  long int C =    1911;
long int C1  =  1804;
long int D   = 1703;
long int Eb  =  1607;
long int E   = 1517;
long int F   = 1432;
long int F1  =  1352;
long int G   = 1276;
long int Ab  =  1204;
long int A   = 1136;
long int Bb  =  1073;
long int B   = 1012;
long int c   =   955;
long int c1  =   902;
long int d   =   851;
long int eb  =   803;
long int e  =    758;
long int f   =    716;
long int f1   =   676;
long int g     =  638;
long int ab     = 602;
long int a       =568;
long int bb      =536;
long int b       =506;
 
long int p      = 0;  //pausa
 
int melod[] = {e, e, e, c, e, g, G, c, G, E, A, B, Bb, A, G, e, g, a, f, g, e, c, d, B, c};
int ritmo[] = {6, 12, 12, 6, 12, 24, 24, 18, 18, 18, 12, 12, 6, 12, 8, 8, 8, 12, 6, 12, 12, 6, 6, 6, 12};

    for (int i=0; i<25; i++) {
        int tom = melod[i];
        int tempo = ritmo[i];
 
        long tvalue = tempo * vel;
 
        tocar(tom, tvalue);
 
        delayMicroseconds(900); //pausa entre notas!
    }
    delay(1000);
    Serial.println("Termino??");
}

