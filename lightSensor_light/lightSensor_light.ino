void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (1) {
    int range_light = analogRead(3);
    Serial.println(range_light);
    if(range_light<660&&range_light>665){
      digitalWrite(6,HIGH);
      delay(3);
    }
    else if ()
    delay(300);
  }


}
