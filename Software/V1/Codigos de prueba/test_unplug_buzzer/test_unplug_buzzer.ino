#define BUZZER A0
void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 1500; i > 10; i = int(i / 1.2)) {
    tone(BUZZER, 4000);
    if (i > 13) {
      delay(30);
      noTone(BUZZER);
    }
    else
      delay(1500);
    if (i > 1000)
      delay(1000);
    else
      delay(i);
  }
  noTone(BUZZER);
  delay(10000);
}
