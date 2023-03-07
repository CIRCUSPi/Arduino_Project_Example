/*
 * OLED
 * VCC --- 5V
 * GND --- GND
 * SCL --- A5
 * SDA --- A4
 * 
 * RFID
 * SDA --- D10
 * SCK --- D13
 * MOSI -- D11
 * MISO -- D12
 * GND --- GND
 * RST --- D9
 * 3.3V -- 3.3V
 */
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h> // 引用程式庫

#define RST_PIN 9 // 讀卡機的重置腳位
#define SS_PIN 10 // 晶片選擇腳位

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
MFRC522 mfrc522(SS_PIN, RST_PIN);

const unsigned char title[] U8X8_PROGMEM= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00, 0x24, 0xFC, 0x7F, 0x7C, 0x3E, 0x7E, 0xF0, 0x23,    0xFC, 0x00, 0xFC, 0x7F, 0x64, 0x0D, 0x44, 0x22, 0xC2, 0x10, 0x20, 0x84,    0x03, 0x04, 0x04, 0x64, 0x09, 0x44, 0x22, 0x82, 0x10, 0x20, 0x04, 0x02,    0xFC, 0x25, 0xB4, 0x7F, 0x7C, 0x3E, 0x82, 0x11, 0x20, 0x04, 0x06, 0x04,    0x24, 0xBC, 0x08, 0x80, 0x09, 0xC2, 0x10, 0x20, 0x04, 0x06, 0xF4, 0x1D,    0xFC, 0x3F, 0x80, 0x18, 0xC2, 0xF0, 0x23, 0x04, 0x06, 0x34, 0x1D, 0xE4,    0x08, 0xFE, 0x7F, 0x3E, 0x10, 0x20, 0x04, 0x06, 0xF4, 0x09, 0xA4, 0x7F,    0x60, 0x04, 0x62, 0x10, 0x20, 0x04, 0x06, 0x46, 0x0C, 0x04, 0x02, 0x18,    0x18, 0xC2, 0x10, 0x20, 0x04, 0x06, 0xC2, 0x0A, 0xE4, 0x12, 0x7E, 0x7E,    0xC2, 0x10, 0x20, 0x04, 0x03, 0x98, 0x50, 0xB4, 0x36, 0x4C, 0x22, 0x82,    0x11, 0x20, 0x84, 0x01, 0x1C, 0x57, 0x94, 0x24, 0x4C, 0x22, 0x02, 0x13,    0x20, 0xFC, 0x00, 0x14, 0x27, 0x96, 0x78, 0x7C, 0x3E, 0x00, 0x00, 0x00,    0x00, 0x00, 0xE4, 0x05, 0x92, 0x4F, 0x4C, 0x22, 0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {
  // Serial.begin(9600);
  u8g2.begin();
  SPI.begin();
  mfrc522.PCD_Init(); // 初始化MFRC522讀卡機模組

  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(20, 0, 88, 17, title);
  u8g2.drawBox(0,17,128,48);
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_inb16_mr);
  u8g2.drawStr(38,48,"wait");
  u8g2.sendBuffer();
}

void loop() {
  // 確認是否有新卡片
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte *id = mfrc522.uid.uidByte; // 取得卡片的UID
    byte idSize = mfrc522.uid.size; // 取得UID的長度
    
    // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

    String readCode = "";
    for (byte i = 0; i < idSize; i++) { // 逐一顯示UID碼
      /*
      Serial.print("id[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(id[i], HEX); // 以16進位顯示UID值
      */
      readCode += String(id[i], HEX);
    
    }
    mfrc522.PICC_HaltA(); // 讓卡片進入停止模式
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(22, 0, 88, 17, title);
    u8g2.drawBox(0,17,128,48);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_inb16_mr);
    u8g2.drawStr(8,48,readCode.c_str());
    
    u8g2.sendBuffer();
  }

}
