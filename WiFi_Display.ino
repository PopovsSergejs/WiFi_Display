#include <Wire.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

//Definition of addresses
#define PCA9540B_ADDRESS    0x70       //0x70
#define DRIVER_U2_ADDRESS   0b1100000       //0x60
#define DRIVER_U3_ADDRESS   0b1100011       //0x63
#define DRIVER_U5_ADDRESS   0b1100001       //0x61
#define DRIVER_U4_ADDRESS   0b1100010       //0x62

#define READ_ADDRES         0b00000001
#define WRITE_ADDRESS       0b00000000

//Definition of LED Drivers Registers
#define CONFIG_REG          0x00
#define COLUMN_DATA1        0x01
#define COLUMN_DATA2        0x02
#define COLUMN_DATA3        0x03
#define COLUMN_DATA4        0x04
#define COLUMN_DATA5        0x0563
#define COLUMN_DATA6        0x06
#define COLUMN_DATA7        0x07
#define COLUMN_DATA8        0x08
#define UPDATE_COLUMN       0x0C
#define LIGHTING_EFFECT     0x0D
#define AUDIO_EQ            0x0F

//Definition of I2C
#define CHANNEL0_EN          0b00000100
#define CHANNEL1_EN          0b00000101
#define DISABLE_CHANNELS    0b00000000

//Define Pins
#define SCL                 12
#define SDA                 11
#define SDB                 10

#define DELAY               50
DynamicJsonDocument doc(2048);


void setup() {
  Serial.begin(9600);
  connectToWiFi();
  
  doc = downloadJSON();
  pinMode(SDB, OUTPUT);
  digitalWrite(SDB, HIGH);
  Wire.begin();
    // Extract values
  Serial.println();
  Serial.println(F("Response:"));
  Serial.println(doc["height"].as<int>());
  Serial.println(doc["width"].as<int>());
  Serial.println(doc["speed_x"].as<int>());
  Serial.println(doc["speed_y"].as<int>());
  Serial.println(doc["data"][0].as<int>());
  Serial.println(doc["data"][1].as<int>());
  Serial.println(doc["data"][2].as<int>());


  
  Wire.beginTransmission(DRIVER_U2_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(DRIVER_U3_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(DRIVER_U4_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(DRIVER_U5_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();

  Wire.begin();
  Wire.beginTransmission(PCA9540B_ADDRESS);
  Wire.write(CHANNEL1_EN);
  Wire.endTransmission();
  Wire.beginTransmission(DRIVER_U2_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(DRIVER_U3_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(DRIVER_U4_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(DRIVER_U5_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(0b00000000);
  Wire.endTransmission();
  
}

void loop() {
  char c;
  char r;
  Wire.begin();
  Wire.beginTransmission(PCA9540B_ADDRESS);
  Wire.write(CHANNEL0_EN);
  Wire.endTransmission();
  delay(1);
  for (c = 0; c < 32; c++) {
    char column = char(((doc["data"][c].as<unsigned int>()) & (0xFF)));
    column = flipByte(column);
    if (c < 8)
      Wire.beginTransmission(DRIVER_U2_ADDRESS);
    else if (c < 16)
      Wire.beginTransmission(DRIVER_U3_ADDRESS);
    else if (c < 24)
      Wire.beginTransmission(DRIVER_U4_ADDRESS);
    else
      Wire.beginTransmission(DRIVER_U5_ADDRESS);
    Wire.write((c % 8) + COLUMN_DATA1);
    //Wire.write(rotl(unsigned char(((doc["data"][c].as<unsigned int>()) & (0xFF00)) >> 8)));
    Wire.write(column);
    Wire.write(byte(UPDATE_COLUMN));
    Wire.write(255);
    Wire.endTransmission();
    delay(1);
    //Serial.read();
  }
  Wire.begin();
  Wire.beginTransmission(PCA9540B_ADDRESS);
  Wire.write(CHANNEL1_EN);
  Wire.endTransmission();
  delay(1);
  for (c = 0; c < 32; c++) {
    char column = char(((doc["data"][c].as<unsigned int>()) & (0xFF00)) >> 8);
    column = flipByte(column);
    if (c < 8)
      Wire.beginTransmission(DRIVER_U2_ADDRESS);
    else if (c < 16)
      Wire.beginTransmission(DRIVER_U3_ADDRESS);
    else if (c < 24)
      Wire.beginTransmission(DRIVER_U4_ADDRESS);
    else
      Wire.beginTransmission(DRIVER_U5_ADDRESS);
    Wire.write((c % 8) + COLUMN_DATA1);
    //Wire.write(char(((doc["data"][c].as<unsigned int>()) & (0xFF))));
    Wire.write(column);
    Wire.write(byte(UPDATE_COLUMN));
    Wire.write(255);
    Wire.endTransmission();
    delay(1);
    //Serial.read();
  }
  while(1);
}

char flipByte(char c)
     {
       c = ((c>>1)&0x55)|((c<<1)&0xAA);
       c = ((c>>2)&0x33)|((c<<2)&0xCC);
       c = (c>>4) | (c<<4) ;

       return c;
     }
