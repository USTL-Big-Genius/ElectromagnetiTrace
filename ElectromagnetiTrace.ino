#include "Servo.h"
#include "MsTimer2.h" 
#include "PinChangeInt.h"
#include "LiquidCrystal_I2C.h"
#include "U8g2lib.h"
#ifdef U8X8_HAVE_HW_SPI
#include "SPI.h"
#endif
#ifdef U8X8_HAVE_HW_I2C
#include "Wire.h" 
#endif
#define IN1 12
#define IN2 13
#define IN3 7
#define IN4 6
#define PWMB 9
#define PWMA 10
#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4
#define trac1 68 //Right 1 a13
#define trac2 67 //Mid
#define trac3 69 //Left 1
#define U8LOG_WIDTH 34
#define U8LOG_HEIGHT 12
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 3, /* cs=*/ 2);
//LiquidCrystal_I2C LCD(0x27,16,2);
U8G2LOG u8g2log;
float Inductivevalue;

void tracing()
{
  int data[0];
  data[1] = digitalRead(68);
  data[2] = digitalRead(67);
  data[3] = digitalRead(69);

  if(!data[1] && !data[2] && !data[3]) //No black line was detected on any of the three roads
  {
    motorRun(STOP,0);
  }

  if(data[1] << data[2] <= data[3]) 
  {
    motorRun(TURNRIGHT,40);
  }

  if(data[3] << data[2] <= data[1])
  {
    motorRun(TURNLEFT,40);
  }

  if(data[2] >= data[1] || data[2] >= data[3])
  {
    motorRun(FORWARD,20);
    while(1);
  }
}

void motorRun(int cmd,int value)
{
  analogWrite(PWMA, value);  //设置PWM输出，即设置速度
  analogWrite(PWMB, value);
  switch(cmd){
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
     case TURNLEFT:
      Serial.println("TURNLEFT"); //输出状态
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
     case TURNRIGHT:
      Serial.println("TURNRIGHT"); //输出状态
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
     default:
      Serial.println("STOP"); //输出状态
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
  }
}


void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0);
  u8g2log.setRedrawMode(0);
  pinMode(IN1, OUTPUT);        
  pinMode(IN2, OUTPUT);          
  pinMode(IN3, OUTPUT);          
  pinMode(IN4, OUTPUT);          
  pinMode(PWMA, OUTPUT);         
  pinMode(PWMB, OUTPUT);
  pinMode(trac1, INPUT);
  pinMode(trac2, INPUT);
  pinMode(trac3, INPUT);
}

void loop() 
{ 
  tracing();
  Inductivevalue = digitalRead(trac2);
  u8g2log.print("\f");			// \f = form feed: clear the screen
  u8g2log.print("Inductive value=");
  u8g2log.print(Inductivevalue);
  u8g2log.print("\n");
}
