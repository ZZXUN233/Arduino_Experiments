#include<IRremote.h>
#include<MsTimer2.h>
#include<Thread.h>

Thread myThread = Thread();
Thread blueThread = Thread();

//小车运动控制
#define EN1  4
#define EN2  7
#define PWM1  5
#define PWM2  6
#define LEFT true
#define RIGHT false
#define AHEAD true
#define BACK false

//寻线
#define lineL 11  //左线
#define lineM 2  //中线
#define lineR 3  //右线
//避障
#define barrL 10
#define barrM 9
#define barrR 8
//遥控
int RECV_PIN = 12; // 红外一体化接收头连接到Arduino 11号引脚

IRrecv irrecv(RECV_PIN);
decode_results results; // 用于存储编码结果的对象

bool goOnline = false;
bool goAvoid = false;
int speB = 80;
int speS = 60;

enum btn {
  btn0 = 16738455, //0
  btn1 = 16724175, //1
  btn2 = 16718055, //2
  btn3 = 16743045, //3
  btn4 = 16716015, //4
  btn5 = 16726215, //5
  btn6 = 16734885, //6
  btn7 = 16728765, //7
  btn8 = 16730805, //8
  btn9 = 16732845, //9


  btnOn = 16753245, //开关
  btnMode = 16736925, //mode
  btnQuiet = 16769565, //静音
  btnPlay  = 16720605, //播放
  btnBack  = 16712445, //退
  btnAhead = 16761405, //进
  btnEq    = 16769055, //EQ

  btnSub   = 16750695, //-
  btnAdd   = 16712445 //+
};

boolean keepMode = true;
boolean rem_status;

void setup()
{
  //运动控制
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  //寻线
  pinMode(lineL, INPUT);
  pinMode(lineM, INPUT);
  pinMode(lineR, INPUT);
  //避障
  pinMode(barrL, INPUT);
  pinMode(barrM, INPUT);
  pinMode(barrR, INPUT);
  //串口及其它
  Serial.begin(9600);
  irrecv.enableIRIn(); // 初始化红外解码
  //  MsTimer2::set(200, controller); // 定时器间隔 0.5s （500ms = 0.5s）
  //  MsTimer2::start();//开始计时
  //多线程
  myThread.onRun(controller);
  myThread.setInterval(200);

  blueThread.onRun(blueController);
  blueThread.setInterval(50);

}

//单轮控制
void ahead(float spe, boolean ifLeft) {
  if (ifLeft) {
    digitalWrite(EN1, HIGH);
    analogWrite(PWM1, int(spe));
  } else {
    digitalWrite(EN2, HIGH);
    analogWrite(PWM2, int(spe));
  }
}
void back(float spe, boolean ifLeft) {
  if (ifLeft) {
    digitalWrite(EN1, LOW);
    analogWrite(PWM1, int(spe));
  } else {
    digitalWrite(EN2, LOW);
    analogWrite(PWM2, int(spe));
  }
}

//车身控制
void gohead(float spe) {
  ahead(spe, LEFT);
  ahead(spe, RIGHT);
  //  delay(80);
}
void goback(float spe) {
  back(spe, LEFT);
  back(spe, RIGHT);
}
//原地打转 true为顺时针
void gocircle(float spe, boolean ifwise) {
  if (ifwise) {
    ahead(spe, LEFT);
    back(spe, RIGHT);
  } else {
    back(spe, LEFT);
    ahead(spe, RIGHT);
  }
}
void _stop() {
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
}

void turnLeft(float spe, boolean ifAhead) {
  if (ifAhead) {
    ahead(spe, LEFT);
    ahead(spe * 2, RIGHT);
  } else {
    back(spe, LEFT);
    back(spe * 2, RIGHT);
  }
  //  delay(40);
}
void turnRight(float spe, boolean ifAhead) {
  if (ifAhead) {
    ahead(spe * 2, LEFT);
    ahead(spe , RIGHT);
  } else {
    back(spe * 2, LEFT);
    back(spe, RIGHT);
  }
  //  delay(40);
}

// 扫描地上的线构建状态
int scanLine() {
  return digitalRead(lineL) * 4 + digitalRead(lineM) * 2 + digitalRead(lineR) * 1;
}

// 扫描前方障碍物
int scanBarr() {
  return digitalRead(barrL) * 4 + digitalRead(barrM) * 2 + digitalRead(barrR) * 1;
}

// 读取遥控改变车的当前状态
void changeMode() {
  keepMode = false;
}

int lastState = 0;
//避障
void goAvoidBarr(boolean ifOn) {
  if (ifOn) {
    switch (scanBarr()) {
      case 0://前方都有障碍-右转
        lastState = 0;
        gocircle(speS, true);
        break;
      case 1://左有障碍
        gocircle(speB, true);
        break;
      case 2://左右有障碍
        gocircle(speS, true);
        gocircle(speS, false);
        delay(300);
        break;
      case 3://右方障碍
        if (lastState == 0) {
          gocircle(speS, true);
          delay(300);
          lastState = 3;
          break;
        }
        gocircle(speS, true);
        break;
      case 4://大角度右转
        gocircle(speS, false);
        break;
      case 5://两侧都有障碍
        turnLeft(speS, false);
        turnRight(speS, false);
        break;
      case 6://右方障碍
        gocircle(speS, false);
        break;
      case 7://前方无障碍
        gohead(speB);
        break;
      default:
        break; //changeMode遥控刷新控制状态，跳出while循环
    }
  }
}


// 主功能模块1：寻线
void goOnLine(boolean ifOn) {
  int lastStatus;
  if (ifOn) {
    switch (scanLine()) {
      case 0://直行
        gohead(speB);
        break;
      case 1://大角度左转
        rem_status = false;
        lastStatus = 2;
        gocircle(speB, false);
        break;
      case 2://左右摇摆前进
        turnLeft(50, AHEAD);
        turnRight(50, AHEAD);
        break;
      case 3://小角度左转
        rem_status = false;
        lastStatus = 3;
        gocircle(speS, false);
        break;
      case 4://大角度右转
        rem_status = true;
        lastStatus = 4;
        gocircle(speB, true);
        break;
      case 5://直行
        gohead(speB);
        break;
      case 6://小角度右转
        lastStatus = 6;
        rem_status = true;
        gocircle(speS, true);
        break;
      case 7://原地打转找线
        if (lastStatus == 4 || lastStatus == 1) {
          gocircle(speB, rem_status);
        } else {
          gohead(speS);
        }
        break;
      default:
        //      Serial.println(line_codes);
        break; //changeMode遥控刷新控制状态，跳出while循环
    }
  }

}

void controller() {
  if (irrecv.decode(&results)) {
    switch (results.value) {
      case btn2: gohead(100); Serial.println("ahead!");  break;
      case btn8: goback(100);  Serial.println("back!"); break;
      case btn4: turnLeft(speB, true);  Serial.println("left!"); break;
      case btn6: turnRight(speB, true);  Serial.println("right!"); break;
      case btn5: _stop();  goOnline = false; goAvoid = false; break;

      //小车模式切换1,3
      case btn1: goOnline = true; goAvoid = false; break;
      case btn3: goOnline = false; goAvoid = true; break;
      case btn7: gocircle(speB, true); break;
      case btn9: gocircle(speB, false); break;
      default:
        Serial.println(results.value);
        keepMode = false;
        break;
    }
    irrecv.resume();
  }
}

String message;
boolean keepAhead = false;
void blueController() {
  while (Serial.available())
  { //while there is data available on the serial monitor
    message += char(Serial.read()); //store string from serial command
  }
  if (!Serial.available())
  {
    if (message != "")
    { //if data is available
      Serial.println(message); //show the data
      switch (message.toInt()) {
        case 2: gohead(speB); Serial.println("ahead!");  break;
        case 8: goback(speB);  Serial.println("back!"); break;
        case 4: gocircle(speB, false);  Serial.println("left!"); break;
        case 6: gocircle(speB, true);  Serial.println("right!"); break;
        case 5: _stop();break;
        case 1: turnLeft(speB, true); break;
        case 3: turnRight(speB, true); break;
        case 7: turnLeft(speB, false); break;
        case 9: turnRight(speB, false); break;

        //小车模式切换1,3
        case 10: goOnline = true; goAvoid = false; speB = 80; break;
        case 11: goOnline = false; goAvoid = true; speB = 80; break;
        //        case 1003: keepAhead = ~keepAhead; break;
        case 12: gocircle(speB, true); break;
        case 13: goOnline = false; goAvoid = false;Serial.println("reset!"); break;
        default:
          if (message.toInt() <= 255 && message.toInt() >= 60) {
            speB = message.toInt();
//            speS = speB*0.8;
          }
          Serial.println(message + "\n");
          keepMode = false;
          break;
      }
      message = ""; //clear the data
    }
  }
  //  delay(100);
}

void loop()
{
  if (myThread.shouldRun())
    myThread.run();
  if (blueThread.shouldRun())
    blueThread.run();
  goAvoidBarr(goAvoid);
  goOnLine(goOnline);
}
