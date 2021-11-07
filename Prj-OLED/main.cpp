#include <Arduino.h>
#include <wire.h>
#include <WiFi.h>
#include "SSD1306.h"
//OLED地址以及IIC引脚定义
const int Address = 0x3C;
const int SDA_pin = 21;
const int SCL_pin = 22;
//WIFI名称及密码
const char* WIFI_SSID = "IIIIIIIIIIIIIII";  
const char* WIFI_PWD = "huxuzhuchen";
const char* ntpSever = "pool.ntp.org";
//时区信息
const long gmtoffset_sec = 8*3600;
const int daylightoffset_sec = 0;
//定义OLED类  
SSD1306 display(Address,SDA_pin,SCL_pin);
//保存自身的IP信息
IPAddress ipv4;
//保存IPV4 地址
uint8_t ip[4];

//WIFI连接
void wificonnect() { 
  char buff[16]; 
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(80);
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 18, "wait");
    display.display(); 
  }
  Serial.println("");
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Connected");
  display.drawString(0, 12, "Wifi:IIIIIIIIIIIIIII");
 
  ipv4=WiFi.localIP();

  for(int i=0;i<4;i++)
  {
    ip[i]=ipv4[i];
  }
  sprintf_P(buff, PSTR("%d.%d.%d.%d"), ip[0],ip[1],ip[2],ip[3]);
  Serial.printf("%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
  display.drawString(0,25,"IP:");
  display.drawString(20,25,String(buff));
  display.display();
  delay(500);
}

void Dislocaltime()
{
  char buff[16]; 
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    delay(500);
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Get time Error!");
    display.display();
  }
  sprintf_P(buff, PSTR("%d:%d:%d"),timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec );
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.drawString(15,20,String(buff));
  display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.println(); //println换行输出
  // 屏幕初始化
  display.init();
  display.clear();
  display.display();
  display.setContrast(255); //屏幕亮度
  // 用固定密码连接
  wificonnect();
  configTime(gmtoffset_sec, daylightoffset_sec, ntpSever);
  Dislocaltime();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(200);
}

 
 
void loop() {
  Dislocaltime();
}