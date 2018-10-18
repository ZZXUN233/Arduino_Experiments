#define IN1 3
#define IN2 4
#define IN3 6
#define IN4 7
#define PWMA 10
#define PWMB 11
void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop()
{
  int i;
  for(i=0;i<=255;i++)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(PWMA, i);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(PWMB, i);
    delay(40);
  }
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  delay(2000);
  for(i=0;i<=255;i++)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(PWMA, i);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(PWMB, i);
    delay(40);
  }
}
