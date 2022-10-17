#include "jungwoo.h"
#include "Arduino.h" // c++ 파일을 작성할 때 "Arduino.h" 추가해야 함 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <string.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <math.h>

// 와이파이 연결 
void setupWifi()
{
    delay(100);

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP());
}

// 배터리 잔량 측정 
String checkVoltage()
{
    float input_vol, cal_vol, temp;
    String res;
    input_vol = analogRead(A0);
    temp = input_vol/4.092;
    cal_vol = temp / 10;
    cal_vol = cal_vol * 0.63613*0.9;  // 오차율 조절해야 함 (0.63613)
    //Serial.println(cal_vol);
    cal_vol = (int)round((cal_vol / BAT) * 100);
    temp = (int)cal_vol % 10;
    cal_vol -= temp;
    // cal_vol = cal_vol;
    res = String(cal_vol);
    return res;
}

// 서버로 데이터 전송 (http)
void sendDataHttp(String servername, char message[], WiFiClient espClient)
{
    HTTPClient http; 
   
    http.begin(espClient, servername);
    http.addHeader("Content-Type", "application/json");

    String httpRequestData = message;
    int httpResponseCode = http.POST(httpRequestData);
}

void setupLCD(LiquidCrystal_I2C lcd)
{
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(2, 0);
    lcd.print("Hello World!");
    lcd.setCursor(0, 1);
    lcd.print("Connecting to WiFi");
}



void displayLCD(LiquidCrystal_I2C lcd, String mystring)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BAT: ");
    lcd.print(mystring);
    lcd.print("%");
}

void initLCD(LiquidCrystal_I2C lcd)
{
    lcd.setCursor(0, 0);
    lcd.print("Hello");
}

void buzzer(int buzzerPin)
{
    tone(buzzerPin, 262);
    delay(500);
    noTone(buzzerPin);
    delay(500);
}

// 현재 시간 구하기 (리턴 값 string으로 할 수 있었는데 왜 char로 했었는지 모르겠음 ㅎㅎ)
const char* getTime(NTPClient timeClient) 
{
    // static으로 선언함으로서, arrBuff가 함수내부에서만 scope을 갖지 않고 함수 외부에서도 해당 값을 유지
    static char arrBuff[20]; // String 형식인 시간정보를 arr에 저장 

    String time, year, mon, day, hour, min_, sec, strBuff;

    time = timeClient.getFormattedDate();

    year = time.substring(0, 4);
    mon = time.substring(5, 7);
    day = time.substring(8, 10);
    hour = time.substring(11, 13);
    min_ = time.substring(14, 16);
    sec = time.substring(17, 19);

    strBuff = year + "-" + mon + "-" + day + "T" + hour + ":" + min_ + ":" + sec;

    strBuff.toCharArray(arrBuff, 20);
    Serial.print("log : ");
    Serial.println(arrBuff);
    
    return arrBuff;
}



