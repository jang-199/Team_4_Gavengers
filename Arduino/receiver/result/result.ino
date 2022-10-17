#include "WiFiEsp.h"
#include <ArduinoJson.h>
 #include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include<IRremote.h>
#define BAT 7.4
LiquidCrystal_I2C lcd(0x27, 16, 2);

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif
String deviceId = "123";
String tempForBat = "-1";
String sensingResult = "";
char ssid[] = "asdf1234";            // your network SSID (name)
char pass[] = "123456789g";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "192.168.210.180";
WiFiEspClient client;

//receive
bool checkIrRecv1 = false; 
bool checkIrRecv2 = false;
int Recv_pin1 = 4;
int Recv_pin2 = 5;
unsigned long res1, res2;
IRrecv irrecv1(Recv_pin1); //IRrecv 객체 생성
decode_results results1; // 수신 데이터 저장 구조체
IRrecv irrecv2(Recv_pin2); //IRrecv 객체 생성
decode_results results2; // 수신 데이터 저장 구조체
unsigned long currentTime, previousTime = 0;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  lcd.begin();       // lcd 모듈 초기화
  lcd.backlight();  // lcd 백라이트 ON
  irrecv1.enableIRIn();
  irrecv2.enableIRIn();
  previousTime = millis();  
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
}

void loop()
{
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  String res = checkVoltage();
  displayLCD(lcd,res);
  sendBat();
//  sensingResult = sensing();
//  sendData("state",sensingResult,"/update/sensing");
}

//void sendBat(String bat)
//{
//    if (bat.toInt() == 100 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","100","/update/rxbattery");
//    }
//    else if (bat.toInt() == 90 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","90","/update/rxbattery");
//    }
//    else if (bat.toInt() == 80 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","80","/update/rxbattery");
//    }
//    else if (bat.toInt() == 70 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","70","/update/rxbattery");
//    }
//    else if (bat.toInt() == 60 && tempForBat != bat)
//    {
//     sendData("batteryCapacity","60","/update/rxbattery");
//    }
//    else if (bat.toInt() == 50 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","50","/update/rxbattery");
//    }
//    else if (bat.toInt() == 40 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","40","/update/rxbattery");
//    }
//    else if (bat.toInt() == 30 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","30","/update/rxbattery");
//    }
//    else if (bat.toInt() == 20 && tempForBat != bat )
//    {
//      sendData("batteryCapacity","20","/update/rxbattery");
//    }
//    else if (bat.toInt() == 10 && tempForBat != bat)
//    {
//      sendData("batteryCapacity","10","/update/rxbattery");
//    }
//    if (bat != tempForBat)
//    {
//        tempForBat = bat;
//    }
//}
void sendBat(){
  sendData("30");
}


String checkVoltage()
{
    float input_vol, cal_vol, temp;
    String res;
    input_vol = analogRead(A0);
    temp = input_vol/4.092;
    cal_vol = temp / 10;
    cal_vol = cal_vol * 0.901;  // 오차율 조절해야 함 (0.63613)
    cal_vol = (cal_vol / BAT) * 100;
    cal_vol = (int)cal_vol;
    res = String(cal_vol);
    return res;
}

void displayLCD(LiquidCrystal_I2C lcd, String mystring)
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(mystring);
    delay(1000);
}

String sensing(){
    currentTime = millis();
    String result ="";
    if (irrecv1.decode(&results1))
    {    
        irrecv1.resume();
    }
    if (irrecv2.decode(&results2))
    {
        irrecv2.resume();
    }

    if (results1.value == 518124555) // 
    {
    }
    else // 끊겼을 때 
    {
        checkIrRecv1 = true;
        if (checkIrRecv2 == true)
        {
            Serial.println("go_in");
            result = "in";
            checkIrRecv1 = false;
            checkIrRecv2 = false;
            delay(3000);
        }
    }

    if (results2.value == 518124555)
    {
    }
    else
    {
        checkIrRecv2 = true;

          if (checkIrRecv1 == true)
        {
            Serial.println("come_out");
            result = "out";
            checkIrRecv1 = false;
            checkIrRecv2 = false;
            delay(3000);
        }
    }
    currentTime = millis();
    if (currentTime - previousTime >= 2000)
    {
        previousTime = currentTime;
        if ((checkIrRecv1 == true && checkIrRecv2 == false) || (checkIrRecv1 == false && checkIrRecv2 == true)) 
        {
            checkIrRecv1 = false;
            checkIrRecv2 = false;
            previousTime = 0;
        }
    }
    return result;
}

String sendData(String result){
  if (client.connect(server, 8080)) {
      Serial.println("Connected to server");
      String jsondata = "";
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["batteryCapacity"] = result;
      root["deviceId"] = deviceId;

      root.printTo(jsondata);
      Serial.println(jsondata);

      client.print(F("POST /update/rxbattery"));
//      client.print(url);
      client.print(F(" HTTP/1.1\r\n"));
      client.print(F("Cache-Control: no-cache\r\n"));
      client.print(F("Host: 192.168.210.180:8080\r\n"));
      client.print(F("User-Agent: Arduino\r\n"));
      client.print(F("Content-Type: application/json;charset=UTF-8\r\n"));
      client.print(F("Content-Length: "));
      client.println(jsondata.length());
      client.println();
      client.println(jsondata);
      client.print(F("\r\n\r\n"));
    } 
}
