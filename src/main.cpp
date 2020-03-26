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

// This is to display the degree symbol.
const char DEGREE_SYMBOL[] = { 0xB0, '\0' };

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 5 , /* reset=*/ U8X8_PIN_NONE);

int scanTime = 15; //In seconds
BLEScan* pBLEScan;

//Red:    A495BB10C5B14B44B5121370F02D74DE
//Green:  A495BB20C5B14B44B5121370F02D74DE
//Black:  A495BB30C5B14B44B5121370F02D74DE
//Purple: A495BB40C5B14B44B5121370F02D74DE
//Orange: A495BB50C5B14B44B5121370F02D74DE
//Blue:   A495BB60C5B14B44B5121370F02D74DE
//Yellow: A495BB70C5B14B44B5121370F02D74DE
//Pink:   A495BB80C5B14B44B5121370F02D74DE
//String tiltUUID = "a495bb70c5b14b44b5121370f02d74de"; // Yellow
String tiltUUID01 = "a495bb";
String tiltUUID02 = "0c5b14b44b5121370f02d74de";
//String tColor = "";

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String thisString = advertisedDevice.toString().c_str();
    int ourIndex01 = thisString.indexOf(tiltUUID01);
    int ourIndex02 = thisString.indexOf(tiltUUID02);
    if (ourIndex01 >= 1 && ourIndex02 >= 8) {
      // Utilizing the UUID we can see what color we are accessing
      String colorCode = thisString.substring(ourIndex01 + 6, ourIndex02);
      int lastChar = ourIndex01 + 32;
      String beerTemp = thisString.substring(lastChar, lastChar + 4);
      String beerGrav = thisString.substring(lastChar + 4, lastChar + 8);
      unsigned int tempOutput = strtoul(beerTemp.c_str(), NULL, 16);
      unsigned int gravOutput = strtoul(beerGrav.c_str(), NULL, 16);
      float realGrav = float(gravOutput) / 1000.0;
      
      String oledTemp = "Temp: " + String(tempOutput);
      String goodGrav = "Gravity: " + String(realGrav, 3);
      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_pxplusibmvga9_tf);
      u8g2.drawStr(0, 35, oledTemp.c_str());

      u8g2.drawUTF8(72, 35, DEGREE_SYMBOL);
      u8g2.drawStr(82, 35, "F");
      u8g2.drawStr(0, 55, goodGrav.c_str());
      switch(colorCode.toInt()) {
        case 1:
          u8g2.drawStr(0, 15, "Red");
          break;
        case 2:
          u8g2.drawStr(0, 15, "Green");
          break;
        case 3:
          u8g2.drawStr(0, 15, "Black");
          break;
        case 4:
          u8g2.drawStr(0, 15, "Purple");
          break;
        case 5:
          u8g2.drawStr(0, 15, "Orange");
          break;
        case 6:
          u8g2.drawStr(0, 15, "Blue");
          break;
        case 7:
          u8g2.drawStr(0, 15, "Yellow");
          break;
        case 8:
          u8g2.drawStr(0, 15, "Pink");
          break;
        default:
          u8g2.drawStr(0, 15, "Unknown");
      }
      u8g2.sendBuffer();
    }
  }
};

void setup() {
  u8g2.begin();
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();
  delay(2000);
}
 