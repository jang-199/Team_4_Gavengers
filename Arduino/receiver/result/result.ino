#include "WiFiEsp.h"
#include<IRremote.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define BAT 7.4
String tempForBat = "110";
String bat;
unsigned long currentTimeForBat, previousTimeForBat = 0;

String url = "";
char ssid[] = "U+NetCE64";    // your network SSID (name)
char pass[] = "DD45019067";         // your network password
int status = WL_IDLE_STATUS;        // the Wifi radio's status

const char server[] = "192.168.219.105";
const char host[] = "192.168.219.105:8080";
WiFiEspClient client; // WiFiEspClient 객체 선언

bool checkIrRecv1 = false; 
bool checkIrRecv2 = false;
int Recv_pin1 = 12;
int Recv_pin2 = 13;
unsigned long res1, res2;
IRrecv irrecv1(Recv_pin1); //IRrecv 객체 생성
decode_results results1; // 수신 데이터 저장 구조체
IRrecv irrecv2(Recv_pin2); //IRrecv 객체 생성
decode_results results2; // 수신 데이터 저장 구조체
unsigned long currentTime = 0, previousTime = 0;
String sensingData = "";

void setup() {
  Serial.begin(9600);  //시리얼모니터
  Serial3.begin(9600);   //와이파이 시리얼
  WiFi.init(&Serial3);   // initialize ESP module
  while ( status != WL_CONNECTED) {   // attempt to connect to WiFi network
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);    // Connect to WPA/WPA2 network
  }
  Serial.println("You're connected to the network");
  Serial.println();

  lcd.begin();
  lcd.backlight();
  irrecv1.enableIRIn();
  irrecv2.enableIRIn();
}

void loop() {
    if (!client.connected()) {   // 날씨정보 수신 종료됐으면
      Serial.println();
      client.stop();
    }

      currentTimeForBat = millis();
  //배터리 데이터 전송
  if (currentTimeForBat - previousTimeForBat >= 60000) //60000
    {
        previousTimeForBat = currentTimeForBat;
        bat = checkVoltage();
        Serial.println(bat);
        displayLCD(lcd, bat);
        sendBat();
    }

    //센싱 데이터 전송
    sensingData = sensing();
    if(sensingData){
      if(sensingData == "out"){
        Serial.println("Out");
        sendData("Out",2);
      }
      if(sensingData == "in"){
        Serial.println("In");
        sendData("In",2);
      }  
    }
}

String checkVoltage()
{
    float input_vol, cal_vol, temp;
    String res;
    input_vol = analogRead(A0);
    temp = input_vol/4.092;
    cal_vol = temp / 10;
    cal_vol = cal_vol *  0.63613 * 0.901;  // 오차율 조절해야 함
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

void sendData(String result, int number){
  switch(number){
    case 1:
      url = "/update/rxbattery/?batteryCapacity=" + result + "&" + "deviceId=" + "123";
      Serial.println("Starting connection to server...");
      if (client.connect(server, 8080)) {
        Serial.println("Connected to server");
        client.print("GET " + url + " HTTP/1.1\r\n" +  
           "Host: " + host + "\r\n" +  
           "Connection: close\r\n\r\n");
      }
      break;
      
    case 2:
      url = "/update/sensing/?state=" + result + "&" + "deviceId=" + "123" + "&" + "power=" + "On" + "&" + "userPk=" + "1234";
      Serial.println("Starting connection to server...");
      if (client.connect(server, 8080)) {
        Serial.println("Connected to server");
        client.print("GET " + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      }
      break; 
     default:
        Serial.println("Data select error");
  }
}

void sendBat()
{
    Serial.print("bat: ");
    Serial.println(bat);
    Serial.println();
    if (bat.toInt() == 100 && bat && bat < tempForBat)
    {
      sendData("100",1);
      Serial.print("bat: ");
      Serial.println(bat);
    }
    else if (bat.toInt() == 90 && bat< tempForBat)
    {
      sendData("90",1);
      Serial.println("bat: ");
      Serial.println(bat);
    }
    else if (bat.toInt() == 80 && bat< tempForBat)
    {
      sendData("80",1);
      Serial.println("bat: ");
      Serial.println(bat);
    }
    else if (bat.toInt() == 70 && bat< tempForBat)
    {
      sendData("70",1);
      Serial.println("bat: ");
      Serial.println(bat);
    }
    else if (bat.toInt() == 60 && bat< tempForBat)
    {
      sendData("60",1);
    }
    else if (bat.toInt() == 50 && bat< tempForBat)
    {
      sendData("50",1);
    }
    else if (bat.toInt() == 40 && bat< tempForBat)
    {
      sendData("40",1);
    }
    else if (bat.toInt() == 30 && bat< tempForBat)
    {
     sendData("30",1);
    }
    else if (bat.toInt() == 20 && bat< tempForBat)
    {
      sendData("20",1);
    }   
    else if (bat.toInt() == 10 && bat< tempForBat)
    {
      sendData("10",1);
    }   

    if (bat.toInt() < tempForBat.toInt()) //&& bat < tempForBat
    {
        tempForBat = bat;
    }
}