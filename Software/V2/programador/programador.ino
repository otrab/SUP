void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  Serial.end();
  DDRD&=~0b11;
Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
      Serial.write(Serial.read());
  }
  //Serial.print("hola");
  //delay(500);
}
