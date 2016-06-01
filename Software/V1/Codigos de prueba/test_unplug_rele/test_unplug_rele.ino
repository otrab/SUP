#define RELE1 A1
void setup() {
  // put your setup code here, to run once:
  pinMode(RELE1,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly
  digitalWrite(RELE1,LOW);
  delay(500);
  digitalWrite(RELE1,HIGH);
  delay(500);
}
