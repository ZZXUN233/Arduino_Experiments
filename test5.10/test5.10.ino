#include <IRremote.h>
int RECV_PIN = 13; // 红外一体化接收头连接到Arduino 13号引脚
IRrecv irrecv(RECV_PIN);
decode_results results; // 用于存储编码结果的对象
long int tool;
long int clockTimeNum = 0;
int tag = -1;
int tagNum[] = {0, 0, 0, 0}; //数字显示时用到
long int button[] = {0xFF6897,  0xFF30CF,   0xFF18E7,   0xFF7A85,   0xFF10EF,
                     //按键数字0   按键数字1   按键数字2   按键数字3   按键数字4
                     0xFF38C7,   0xFF5AA5,   0xFF42BD,   0xFF4AB5,   0xFF52AD,
                     //按键数字5   按键数字6   按键数字7   按键数字8   按键数字9
                     0xFFA25D,    16736925,   16769565,   16720605,   16754775,    16748655,    16712445,     16761405
                     //电源键      模式        静音        暂停        -            +           向左          向右
                    };

int dig[] = {6, 8, 9, 12};
int a = 11;
int b = 7;
int c = 4;
int d = 2;
int e = 1;
int f = 10;
int g = 5;
int dp = 3;
void digdp() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp, LOW);
}
void digital_0(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_1(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_2(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(g, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_3(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(g, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);

  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_4(void) {
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(a, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_5(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_6(void) {
  digitalWrite(a, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_7(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_8(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}
void digital_9(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(g, LOW);
  digitalWrite(f, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(dp, HIGH); //熄灭小数点DP段
}

void setup() {
  for (int i = 1; i < 13; i++) {
    pinMode(i, OUTPUT);
  }
  //Serial.begin(9600);
  irrecv.enableIRIn(); // 初始化红外解码
}
//控制i号灯关
void off(int i) {
  digitalWrite(dig[i], LOW);
}

//控制i号灯显示j
void on(int i, int j) {
  digitalWrite(dig[0], LOW);
  digitalWrite(dig[1], LOW);
  digitalWrite(dig[2], LOW);
  digitalWrite(dig[3], LOW);
  digitalWrite(dig[i], HIGH);
  switch (j) {
    case 0:
      digital_0();  break;
    case 1:
      digital_1();  break;
    case 2:
      digital_2(); break;
    case 3:
      digital_3();  break;
    case 4:
      digital_4();  break;
    case 5:
      digital_5(); break;
    case 6:
      digital_6();  break;
    case 7:
      digital_7(); break;
    case 8:
      digital_8(); break;
    case 9:
      digital_9();  break;
    case 10:
      digdp(); break;
  }
  if (j != 10)
    delay(5);
}
//按键功能:i按键显示数字i
void actionNum(int i) {
  int flag=3;
  tag++;
  tag = tag % 4;
  if (tag == 0)
    for (int i = 1; i < 4; i++)
      tagNum[i] = 0;
  tagNum[tag] = i;
  while (1)
  {
    on(tag, tagNum[0]); on((tag + 3) % 4, tagNum[1]); on((tag + 2) % 4, tagNum[2]); on((tag + 1) % 4, tagNum[3]);
    if (irrecv.decode(&results)) {
      if (results.value == 4294967295)           //处理长按按钮操作
        irrecv.resume();
        else if (results.value == 16748655){       //"+"1
        tagNum[flag]=(tagNum[flag]+1)%10;
        irrecv.resume();
        }
        else if (results.value == 16754775){        //"-1"
        tagNum[flag]=(tagNum[flag]+9)%10;
        irrecv.resume();
        }
        else if (results.value == 16712445){       //向左以一位
        flag=(flag+3)%4;
        irrecv.resume();
        }
        else if (results.value == 16761405){       //向右以一位
        flag=(flag+1)%4;
        irrecv.resume();
        }
      else {
        irrecv.resume();                           //其他按钮操作
        break;
      }
    }
  }
}

//电源键功能:关闭电源，初始化一切参数
void actionPower() {
  tag = -1;
  for (int i = 0; i < 4; i++)
    tagNum[i] = 0;
  off(0), off(1), off(2), off(3);
  while (1) {
    if (irrecv.decode(&results)) {
      if (results.value == 0xFFA25D ) {
        on(0, 0); on(1, 0); on(2, 0); on(3, 0);
      }
      irrecv.resume();
      break;
    }
  }
}
//模式按钮：计时
void actionModel() {
  while (1)
  {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        for (int k = 0; k < 10; k++) {
          for (int m = 0; m < 10; m++) {
            for (int n = 0; n < 50; n++) {
              on(0, m);
              on(1, k);
              on(2, j);
              on(3, i);
              if (irrecv.decode(&results)) {
                if (results.value == 0xFFA25D ) {             //处理电源按钮操作
                  results.value = 0xFFA25D;
                  irrecv.resume();
                  break;
                }//if (results.value == 0xFFA25D )
                else if (results.value == 16720605 || results.value == 16736925  ) {         //处理暂停键和模式键
                  irrecv.resume();
                  if (results.value == 16736925) {             //再次按下模式键，归零暂停
                    m = 0; k = 0; j = 0; i = 0;
                  }
                  while (1) {                                 //暂停键
                    on(0, m); on(1, k); on(2, j); on(3, i);
                    if (irrecv.decode(&results)) {
                      if (results.value == 16720605 || results.value == 0xFFA25D ) {
                        if (results.value == 0xFFA25D)          //暂停时能关闭电源
                          results.value = 0xFFA25D;
                        irrecv.resume();
                        break;
                      }
                      irrecv.resume();
                    }
                  }
                }
                else if (results.value == 16748655) {          //处理"+"按钮操作
                  irrecv.resume();
                  m++;
                }
                else if (results.value == 16754775) {          //处理"-"按钮操作
                  irrecv.resume();
                  m--;
                }
                else
                  irrecv.resume();                           //其他按钮操作

              }//if (irrecv.decode(&results))
            }//for (int n = 0; n <50; n++)
            if (results.value == 0xFFA25D)
              break;
          }
          if (results.value == 0xFFA25D)
            break;
        }
        if (results.value == 0xFFA25D)
          break;
      }
      if (results.value == 0xFFA25D)
        break;
    }
    if (results.value == 0xFFA25D)
      break;
  }
}

//处理未定义功能功能按键：无操作反应
void noAction() {
  while (1) {
    if (irrecv.decode(&results)) {
      irrecv.resume();
      break;
    }
  }
}

//钟表功能
void clockTime() {
  clockTimeNum = (tagNum[0] * 36000 + tagNum[1] * 3600 + tagNum[2] * 600 + tagNum[3] * 60) % 86400;
  Serial.println( clockTimeNum);
  while (1) {
    int i = clockTimeNum / 36000;
    int j = (clockTimeNum - i * 36000) / 3600;
    int k = (clockTimeNum - i * 36000 - j * 3600) / 600;
    int l = (clockTimeNum - i * 36000 - j * 3600 - k * 600) / 60;
    for (int p = 0; p < 60; p++) {
      for (int o = 0; o < 40; o++) {
        on(0, l);
        on(1, k);
        on(2, 10);
        on(2, j);
        on(3, i);
        if (irrecv.decode(&results)) {
          if (results.value == 0xFFA25D ) {             //处理电源按钮操作
            results.value = 0xFFA25D;
            irrecv.resume();
            break;
          }//if (results.value == 0xFFA25D )
          else
            irrecv.resume();                           //其他按钮操作
        }
      }
      if (results.value == 0xFFA25D)
        break;
      off(3);
      delay(200);
      clockTimeNum++;
    }
    if (results.value == 0xFFA25D)
      break;
  }
}

void loop() {
  tool = results.value;
  irrecv.resume();
  switch (tool) {
    case 0xFF6897:
      actionNum(0);      break;
    case 0xFF30CF:
      actionNum(1);      break;
    case 0xFF18E7:
      actionNum(2);      break;
    case  0xFF7A85:
      actionNum(3);      break;
    case 0xFF10EF:
      actionNum(4);      break;
    case 0xFF38C7:
      actionNum(5);      break;
    case  0xFF5AA5:
      actionNum(6);      break;
    case 0xFF42BD:
      actionNum(7);      break;
    case 0xFF4AB5:
      actionNum(8);      break;
    case 0xFF52AD:
      actionNum(9);     break;
    case 16736925:
      actionModel();      break;
    case 0xFFA25D:
      actionPower();      break;
    case 16769565:
      clockTime();        break;
    default:
      noAction();
  }
}
//void loop() {
//    if (irrecv.decode(&results))
//      {
//          Serial.println( results.value);
//          irrecv.resume(); // 接收下一个编码
//      }
//}
