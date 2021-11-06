#include <Arduino.h>
const int button_pin = 0;
const int led_pin    = 2;
int value = 0;
//中断服务函数
void led()
{
  delay(20);
  int  level=digitalRead(button_pin);
  if(level == 0)
  {
    Serial.printf("pin 0 level is:%d",level);
    Serial.println();
    digitalWrite(led_pin,value);
    value = !value;   //翻转电平
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //按键输入 低电平触发
  pinMode(button_pin,INPUT | PULLUP);
  //LED输出
  pinMode(led_pin,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(button_pin),led,CHANGE);

}


void loop() {
  // put your main code here, to run repeatedly:

}