#include <IRremote.h>
 
int RECV_PIN = 11; // 红外一体化接收头连接到Arduino 11号引脚
int LEDpin = 13;//定义LED输出引脚
#define btnOn 16753245 //开关
#define btnMode  16736925 //mode
#define btnQuiet 16769565 //静音
#define btnPlay  16720605 //播放
#define btnBack  16712445 //退
#define btnAhead 16761405 //进
#define btnEq    16769055 //EQ
#define btnSub   16754775 //-
#define btnAdd   16748655 //+
IRrecv irrecv(RECV_PIN);
int btnNum[]={
  16738455,//0
  16724175,//1
  16718055,//2
  16743045,//3
  16716015,//4
  16726215,//5
  16734885,//6
  16728765,//7
  16730805,//8
  16732845//9
};
 
decode_results results; // 用于存储编码结果的对象
 
void setup()
{
    Serial.begin(9600);
//    pinMode(LEDpin,OUTPUT);
    irrecv.enableIRIn(); // 初始化红外解码
}


 
void loop() {
    if (irrecv.decode(&results))
    { 
        Serial.println( results.value);
        if( results.value == 0xFFA25D) //若收到按键ON按下的编码，打开LED
       {
           digitalWrite(LEDpin,HIGH);
       }
       else if(results.value==0xFFE21D) //收到OFF按下的编码，关闭LED
       {
           digitalWrite(LEDpin,LOW);
       }
        irrecv.resume(); // 接收下一个编码
    }
}
