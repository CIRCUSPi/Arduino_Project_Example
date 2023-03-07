/*
 * PMS5003T
 * red ---- 5V
 * black -- GND
 * yellow - D10
 * white -- D11
 * 
 * OLED
 * VCC ---- 3.3V
 * GND ---- GND
 * SCL ---- A5
 * SDA ---- A4
 */

#include <SoftwareSerial.h>
#include <U8g2lib.h>
#include <Wire.h>

const unsigned char title[] U8X8_PROGMEM= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,    0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00,    0x00, 0x24, 0xE2, 0x43, 0x7C, 0x3E, 0xFC, 0x7F, 0xF8, 0x7F, 0xFC, 0x7F,    0x24, 0x4A, 0x44, 0x22, 0x84, 0x44, 0x08, 0x00, 0x04, 0x04, 0x28, 0x4A,    0x44, 0x22, 0x84, 0x44, 0xF4, 0x3F, 0xF4, 0x25, 0xE0, 0x4B, 0x7C, 0x3E,    0x84, 0x44, 0x02, 0x00, 0x04, 0x14, 0x22, 0x4A, 0x80, 0x08, 0x40, 0x04,    0xFC, 0x1F, 0xF4, 0x15, 0x24, 0x4A, 0x40, 0x10, 0x20, 0x7C, 0x44, 0x12,    0x14, 0x09, 0xE8, 0x4B, 0xFE, 0x7F, 0x18, 0x00, 0x48, 0x13, 0xF4, 0x09,    0x20, 0x4A, 0x40, 0x02, 0xF0, 0x3F, 0x50, 0x11, 0x02, 0x0C, 0x20, 0x4A,    0x38, 0x1C, 0x00, 0x01, 0xFE, 0x17, 0x42, 0x4A, 0x28, 0x4A, 0x7E, 0x7E,    0x00, 0x01, 0x50, 0x51, 0x98, 0x4A, 0xE8, 0x4B, 0x44, 0x22, 0x00, 0x01,    0x48, 0x52, 0x18, 0x56, 0x44, 0x41, 0x44, 0x22, 0x00, 0x01, 0x44, 0x54,    0x14, 0x26, 0x24, 0x42, 0x7C, 0x3E, 0xFE, 0xFF, 0x42, 0x28, 0xF0, 0x01,    0x12, 0x7A, 0x44, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    0x00, 0x00};
  
SoftwareSerial pms5003tSerial(10, 11);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

long pmat10_ = 0;
long pmat25_ = 0;
long pmat100_ = 0;
long Temp_ = 0;
long Humid_ = 0;
char buf[50];

void retrievepm25() {
  int count = 0;
  unsigned char c;
  unsigned char high;
  while (pms5003tSerial.available()) {
     c = pms5003tSerial.read();
     if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
       break;
     }
     if(count > 27){
       break;
     }
      else if(count == 10 || count == 12 || count == 14 || count == 24 || count == 26) {
       high = c;
     }
     else if(count == 11){
       pmat10_ = 256*high + c;
     }
     else if(count == 13){
       pmat25_ = 256*high + c;
     }
     else if(count == 15){
       pmat100_ = 256*high + c;
     }
      else if(count == 25){
          Temp_ = (256*high + c)/10;
     }
     else if(count == 27){
                  Humid_ = (256*high + c)/10;
    }       count++;
  }  while(pms5003tSerial.available()) pms5003tSerial.read();
}

void setup() {
  //Serial.begin(9600);
  pms5003tSerial.begin(9600);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.drawXBMP(24, 0, 80, 17, title);
  u8g2.setFont(u8g2_font_inb16_mr);
  u8g2.drawStr(38,48,"wait");
  u8g2.sendBuffer();
  
  while (pmat10_ == 0) {
    retrievepm25();
    delay(1000);
  }
  
}

void loop() {
  retrievepm25();
  /*
  Serial.print("PM1.0: ");
  Serial.print(pmat10_);
  Serial.print(", PM2.5: ");
  Serial.print(pmat25_);
  Serial.print(", PM10: ");
  Serial.println(pmat100_);
  Serial.print("Temp: ");
  Serial.print(Temp_);
  Serial.print(", Humid: ");
  Serial.println(Humid_);
  */
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(24, 0, 80, 17, title);
    u8g2.setFont(u8g2_font_inb16_mr);
    u8g2.drawStr(8,48,("PM1.0:"+String(pmat10_)).c_str());
  } while ( u8g2.nextPage() );
  delay(1000);
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(24, 0, 80, 17, title);
    u8g2.setFont(u8g2_font_inb16_mr);
    u8g2.drawStr(8,48,("PM2.5:"+String(pmat25_)).c_str());
  } while ( u8g2.nextPage() );
  delay(1000);
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(24, 0, 80, 17, title);
    u8g2.setFont(u8g2_font_inb16_mr);
    u8g2.drawStr(8,48,("PM10 :"+String(pmat100_)).c_str());
  } while ( u8g2.nextPage() );
  delay(1000);
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(24, 0, 80, 17, title);
    u8g2.setFont(u8g2_font_inb16_mr);
    u8g2.drawStr(8,48,("Temp :"+String(Temp_)).c_str());
  } while ( u8g2.nextPage() );
  delay(1000);
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(24, 0, 80, 17, title);
    u8g2.setFont(u8g2_font_inb16_mr);
    u8g2.drawStr(8,48,("Humid:"+String(Humid_)).c_str());
  } while ( u8g2.nextPage() );
  delay(1000);

}
