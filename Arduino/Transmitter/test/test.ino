#include <IRremote.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <jungwoo.h>
#include <WiFiUdp.h>

String deviceId = "123";
int Send_pin = 14;

int sw = 0;

unsigned long currentTimeForBat, previousTimeForBat = 0;
String bat;
String tempForBat = "110";

LiquidCrystal_I2C lcd(0x3F, 16, 2);

WiFiClient client;
char messageToServer[150];

String servername = "http://124.60.219.83:8080/update/txbattery";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void setup()
{
    Serial.begin(115200);
    pinMode(A0, INPUT);
    lcd.init(); 
    lcd.backlight();
    initLCD(lcd);
    setupWifi();
    IrSender.begin(Send_pin, ENABLE_LED_FEEDBACK); // 송신기 작동 시작
    previousTimeForBat = millis();
}

void loop() 
{
    currentTimeForBat = millis();

    //Serial.printf("ir send");
    for (int i = 0; i < 3; i++) 
    { 
      IrSender.sendNECMSB(0x1EE1F40B,32);
      delay(40); // 해당 신호 3번 반복, 
    }

    // 1분 마다 배터리 체크하고 lcd에 띄움
    if (currentTimeForBat - previousTimeForBat >= 1000) //60000
    {
        previousTimeForBat = currentTimeForBat;
        bat = checkVoltage();
        displayLCD(lcd, bat);

        //Serial.printf(messageToServer);
        sendBat();
    }

    
}
  
void sendBat()
{
    Serial.printf("bat: %s",bat);
    Serial.println();
    Serial.printf("tempForBat: %s",tempForBat);
    Serial.println();
    if (bat.toInt() == 100 && bat && bat < tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"100\" }", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 90 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"90\"}", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 80 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"80\"}", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 70 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"70\"}", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 60 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"60\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 50 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"50\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 40 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"40\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 30 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"30\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 20 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"20\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }   
    else if (bat.toInt() == 10 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"10\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }   

    if (bat.toInt() < tempForBat.toInt()) //&& bat < tempForBat
    {
        tempForBat = bat;
    }
}

#include <IRremote.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <jungwoo.h>
#include <WiFiUdp.h>

String deviceId = "123";
int Send_pin = 14;

int sw = 0;

unsigned long currentTimeForBat, previousTimeForBat = 0;
String bat;
String tempForBat = "110";

LiquidCrystal_I2C lcd(0x3F, 16, 2);

WiFiClient client;
char messageToServer[150];

String servername = "http://124.60.219.83:8080/update/txbattery";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void setup()
{
    Serial.begin(115200);
    pinMode(A0, INPUT);
    lcd.init(); 
    lcd.backlight();
    initLCD(lcd);
    setupWifi();
    IrSender.begin(Send_pin, ENABLE_LED_FEEDBACK); // 송신기 작동 시작
    previousTimeForBat = millis();
}

void loop() 
{
    currentTimeForBat = millis();

    //Serial.printf("ir send");
    for (int i = 0; i < 3; i++) 
    { 
      IrSender.sendNECMSB(0x1EE1F40B,32);
      delay(40); // 해당 신호 3번 반복, 
    }

    // 1분 마다 배터리 체크하고 lcd에 띄움
    if (currentTimeForBat - previousTimeForBat >= 1000) //60000
    {
        previousTimeForBat = currentTimeForBat;
        bat = checkVoltage();
        displayLCD(lcd, bat);

        //Serial.printf(messageToServer);
        sendBat();
    }

    
}
  
void sendBat()
{
    Serial.printf("bat: %s",bat);
    Serial.println();
    Serial.printf("tempForBat: %s",tempForBat);
    Serial.println();
    if (bat.toInt() == 100 && bat && bat < tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"100\" }", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 90 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"90\"}", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 80 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"80\"}", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 70 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"70\"}", deviceId);
      Serial.printf("bat: %s",bat);
      Serial.printf("tempForBat: %s",tempForBat);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 60 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"60\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 50 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"50\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 40 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"40\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 30 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"30\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }
    else if (bat.toInt() == 20 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"20\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }   
    else if (bat.toInt() == 10 && bat< tempForBat)
    {
      sprintf(messageToServer, "{\"deviceId\": \"%s\", \"batteryCapacity\":\"10\"}", deviceId);
      Serial.printf(messageToServer);
      sendDataHttp(servername, messageToServer , client);
    }   

    if (bat.toInt() < tempForBat.toInt()) //&& bat < tempForBat
    {
        tempForBat = bat;
    }
}
