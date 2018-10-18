char lights[][8] = {
  "11111100",//0
  "01100000",//1
  "11011010",//2
  "11110010",//3
  "01100110",//4
  "10110110",//5
  "10111110",//6
  "11100000",//7
  "11111110",//8
  "11110110"//9
  //0xfc,0x60,0xda,0xf2,0x63,0xb6,0xbe,0xe0,0xfe,0xf6
};  //10
int pins[] = {8, 0, 2, 4, 7, 12, 13, 1}; //a~g,dp对应的引脚  8
int flag = 0;
void setup() {
  int i = 0;
  for (i; i < 10; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void LightWrite(char eight[8]) {
  for (unsigned int i = 0; i < 8; i++) {
    if (eight[i] == '1') {
      digitalWrite(pins[i], LOW);
    } else {
      digitalWrite(pins[i], HIGH);
    }
  }
}

void guessNum() {
  int j = 0;
  while (j < 10) {
    LightWrite(lights[j]);
    delay(50);
    while (analogRead(0) > 1000) {
      LightWrite(lights[j]);
    }
    j++;
  }
}

//0~9999
void scanNum(){
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //  猜数字
  guessNum();
}
