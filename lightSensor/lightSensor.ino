void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  while (1) {
    char i, j;
    while (1) {
      for (i = 0; i < 80; i++) {
        digitalWrite(6, HIGH);
        delay(1);
        digitalWrite(6, LOW);
        delay(1);
      }
      for (i = 0; i < 130; i++) {
        digitalWrite(6, HIGH);
        delay(3);
        digitalWrite(6, LOW);
        delay(3);
      }
    }
  }

}
