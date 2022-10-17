#ifndef __JUNGWOO_H__
#define __JUNGWOO_H__

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <string.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define ssid "asdf1234"
#define password "123456789g"
#define BAT 7.4

void setupWifi();

String checkVoltage(); // Tx, Rx 전원 상태 체크 

void sendDataHttp(String servername, char message[], WiFiClient client);

void setupLCD(LiquidCrystal_I2C lcd);

void displayLCD(LiquidCrystal_I2C lcd, String mystring);

void initLCD(LiquidCrystal_I2C lcd);

void buzzer(int buzzerPin);

const char* getTime(NTPClient timeClient);

//////////////////////////////////////////////////////////////////

#endif