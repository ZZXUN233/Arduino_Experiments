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
//int pins[] = {8, 0, 2, 4, 7, 12, 13, 1}; //a~g,dp对应的引脚  8
int numPins[] = {11, 7, 4, 2, 1, 10, 5, 3};//a~g,dp对应的引脚  8
int whichs[] = {6,8,9,12}; //右边开始的第一个位
char whichLights[][4] = {
  "0001", "0010", "0011", "0100",
  "0101", "0110", "0111", "1000",
  "1001", "1010", "1011", "1100",
  "1101", "1110", "1111"
};//15种灯选择


void setup() {
  for (int j = 0; j < 4; j++) {
    pinMode(whichs[j], OUTPUT);
  }
  for (int i = 0; i < 8; i++) {
    //    pinMode(pins[i], OUTPUT);
    pinMode(numPins[i], OUTPUT);
  }
}

// 二进制解析，flag为1时选中写高电平
/*
  bits  二进制码
  num   二进制码位数——对应二进制控制的引脚个数
  flag  1：选中写高 0：选中写低
  pins  传入的引脚数组
*/
void LightWrite(char eight[8]) {
  for (unsigned int i = 0; i < 8; i++) {
    if (eight[i] == '1') {
      digitalWrite(numPins[i], LOW);
    } else {
      digitalWrite(numPins[i], HIGH);
    }
  }
}


//0~9999
void showNumByBit(int num, int Bit) {
          digitalWrite(whichs[0], LOW);
          digitalWrite(whichs[1], LOW);
          digitalWrite(whichs[2], LOW);
          digitalWrite(whichs[3], LOW);
          digitalWrite(whichs[Bit], HIGH);
          LightWrite(lights[num]);
          delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  猜数字
  //  guessNum();
  //0~9999
   for (int bit1 = 0; bit1 < 10; bit1++) {
    for (int bit2 = 0; bit2 < 10; bit2++) {
      for (int bit3 = 0; bit3 < 10; bit3++) {
        for (int bit4 = 0; bit4 < 10; bit4++) {
            showNumByBit(bit4,0);
            showNumByBit(bit3,1);
            showNumByBit(bit2,2);
            showNumByBit(bit1,3);
        }
      }
    }
  }

}
