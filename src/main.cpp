/* * * * * * * * * * * * * * * * * * * * * * *\
 *                                           *
 *                                           *
 *                                           *
\* * * * * * * * * * * * * * * * * * * * * * */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WiFiAP.h>

// This is to display the degree symbol.
const char DEGREE_SYMBOL[] = { 0xB0, '\0' };

// U8g2 board definition
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 5 , /* reset=*/ U8X8_PIN_NONE);

int scanTime = 15; //In seconds
BLEScan* pBLEScan;

//const char *ssid = "tilt4esp";
//const char *password = "tilt1234";
//WiFiServer server(80);

//Red:    A495BB10C5B14B44B5121370F02D74DE
//Green:  A495BB20C5B14B44B5121370F02D74DE
//Black:  A495BB30C5B14B44B5121370F02D74DE
//Purple: A495BB40C5B14B44B5121370F02D74DE
//Orange: A495BB50C5B14B44B5121370F02D74DE
//Blue:   A495BB60C5B14B44B5121370F02D74DE
//Yellow: A495BB70C5B14B44B5121370F02D74DE
//Pink:   A495BB80C5B14B44B5121370F02D74DE

String tiltUUID01 = "a495bb";
String tiltUUID02 = "0c5b14b44b5121370f02d74de";

unsigned int tempOutput;
unsigned int gravOutput;
float realGrav;
int ourIndex01;
int ourIndex02;
int lastChar;
String colorCode;
String beerTemp;
String beerGrav;
String oledTemp;
String goodGrav;
String dispColor;
String thisString;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    thisString = advertisedDevice.toString().c_str();
    ourIndex01 = thisString.indexOf(tiltUUID01);
    ourIndex02 = thisString.indexOf(tiltUUID02);
    if (ourIndex01 >= 1 && ourIndex02 >= 8) {
      // Utilizing the UUID we can see what color we are accessing
      colorCode = thisString.substring(ourIndex01 + 6, ourIndex02);
      lastChar = ourIndex01 + 32;
      beerTemp = thisString.substring(lastChar, lastChar + 4);
      beerGrav = thisString.substring(lastChar + 4, lastChar + 8);
      tempOutput = strtoul(beerTemp.c_str(), NULL, 16);
      gravOutput = strtoul(beerGrav.c_str(), NULL, 16);
      realGrav = float(gravOutput) / 1000.0;
      
      oledTemp = "Temp: " + String(tempOutput);
      goodGrav = "Gravity: " + String(realGrav, 3);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_pxplusibmvga9_tr);
      u8g2.drawStr(0, 35, oledTemp.c_str());

      u8g2.drawUTF8(72, 35, DEGREE_SYMBOL);
      u8g2.drawStr(82, 35, "F");
      u8g2.drawStr(0, 55, goodGrav.c_str());
      switch(colorCode.toInt()) {
        case 1:
          dispColor = "Red";
          break;
        case 2:
          dispColor = "Green";
          break;
        case 3:
          dispColor = "Black";
          break;
        case 4:
          dispColor = "Purple";
          break;
        case 5:
          dispColor = "Orange";
          break;
        case 6:
          dispColor = "Blue";
          break;
        case 7:
          dispColor = "Yellow";
          break;
        case 8:
          dispColor = "Pink";
          break;
        default:
          dispColor = "Unknown";
      }
      u8g2.drawStr(0, 15, dispColor.c_str());
      u8g2.sendBuffer();
    }
  }
};

void setup() {
  //Serial.begin(115200);
  //Serial.println();
  //Serial.println("Configuring access point...");
  
  //WiFi.softAP(ssid);
  //IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  //server.begin();

  //Serial.println("Server started");

  u8g2.begin();
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  //WiFiClient client = server.available();   // listen for incoming clients
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();
  delay(2000);
}
 
