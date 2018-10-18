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
#define lineL 1  //左线
#define lineM 2  //中线
#define lineR 3  //右线

//char line_status[8][3] = {
//  '000', //直行
//  '001', //小角度左转
//  '010', //左右摇摆前进
//  '011', //大角度左转
//  '100', //小角度右转
//  '101', //直行
//  '110', //大角度右转
//  '111'  //原地打转找线
//}

int L_val, M_val, R_val;
int status_code;
boolean keepMode = true;

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
  //串口及其它
  //  Serial.begin(9600);
}

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
void gohead(float spe) {
  ahead(spe, LEFT);
  ahead(spe, RIGHT);
//  delay(80);
}

void goback(float spe) {
  back(spe, LEFT);
  back(spe, RIGHT);
}

void turnLeft(float spe, boolean ifAhead) {
  if (ifAhead) {
    ahead(spe, LEFT);
    ahead(spe * 1.5, RIGHT);
  } else {
    back(spe, LEFT);
    back(spe * 1.5, RIGHT);
  }
//  delay(40);
}
void turnRight(float spe, boolean ifAhead) {
  if (ifAhead) {
    ahead(spe * 1.5, LEFT);
    ahead(spe , RIGHT);
  } else {
    back(spe * 1.5, LEFT);
    back(spe, RIGHT);
  }
//  delay(40);
}

void gocircle(float spe, boolean ifwise) {
  if (ifwise) {
    ahead(spe, LEFT);
    back(spe, RIGHT);
  } else {
    back(spe, LEFT);
    ahead(spe, RIGHT);
  }
//  delay(40);
}

void _stop() {
  digitalWrite(EN1, LOW);
  analogWrite(PWM1, 0);
  digitalWrite(EN2, LOW);
  analogWrite(PWM2, 0);
}

int scanLine() {
  L_val = digitalRead(lineL);
  M_val = digitalRead(lineM);
  R_val = digitalRead(lineR);
  status_code = L_val * 4 + M_val * 2 + R_val * 1;
  Serial.println(status_code);
  return status_code;
}

// 读取遥控改变车的当前状态
void changeMode() {
  keepMode = false;
}



// 主功能模块1：寻线
void goOnLine() {
  while (keepMode) {
    status_code = scanLine();  //扫线更新线状态
    switch (status_code) {
      case 0://直行
        gohead(100);
        break;
      case 1://大角度左转
//        turnLeft(60, AHEAD);
        gocircle(60, false);
        break;
      case 2://左右摇摆前进
        turnLeft(40, AHEAD);
        turnRight(40, AHEAD);
        break;
      case 3://小角度左转
        //        turnLeft(50, AHEAD);
        gocircle(50, false);
        break;
      case 4://大角度右转
        //        turnRight(60, AHEAD);
        gocircle(60, true);
        break;
      case 5://直行
        gohead(100);
        break;
      case 6://小角度右转
        //        turnRight(50, AHEAD);
        gocircle(50, true);
        break;
      case 7://原地打转找线
        //        gocircle(50, false);
        gocircle(80, true);
        gocircle(80, false);
        break;
      default:
        Serial.println(status_code);
        break; //changeMode遥控刷新控制状态，跳出while循环
    }
  }
}

void loop()
{
//  gocircle(50, true);
    goOnLine();//进入寻线功能
}
