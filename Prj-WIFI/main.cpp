#include <Arduino.h>
#include <wifi.h>
#include <HTTPClient.h>
#include "../lib/ArduinoJson/ArduinoJson.h"

void wifi_Connect()
{
  WiFi.begin("************","***********"); //WiFi和密码
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(300);
    Serial.println("There is no connect!");
  }
}
String UID = "275908810";
String followerUrl = "http://api.bilibili.com/x/relation/stat?vmid=" + UID; 
long follower = 0;
DynamicJsonDocument doc(1024);

//	获取粉丝数
void getBiliBiliFollower()
{
	HTTPClient http;
	http.begin(followerUrl); //HTTP begin
	int httpCode = http.GET();

	if (httpCode > 0)
	{
		// httpCode will be negative on error
		Serial.printf("HTTP Get Code: %d\r\n", httpCode);

		if (httpCode == HTTP_CODE_OK) // 收到正确的内容
		{
			String resBuff = http.getString();

			//	输出示例：{"mid":123456789,"following":226,"whisper":0,"black":0,"follower":867}}
			Serial.println(resBuff);

			//	使用ArduinoJson_6.x版本，具体请移步：https://github.com/bblanchon/ArduinoJson
			deserializeJson(doc, resBuff); //开始使用Json解析
			follower = doc["data"]["follower"];
			Serial.printf("Follers: %ld \r\n", follower);
		}
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http.errorToString(httpCode).c_str());
	}

	http.end();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("Connecting....");
  wifi_Connect();
  Serial.println("Wifi connected");
  Serial.print("IP adddress: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
 // Serial.println(".");
 getBiliBiliFollower();
 delay(1000);
 delay(1000);
}