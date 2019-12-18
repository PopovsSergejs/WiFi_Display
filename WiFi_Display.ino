#include <Wire.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"
#include <String.h>




//Define Pins
#define SCL                 12
#define SDA                 11
#define SDB                 10

#define DELAY               50

#define UPDATE_PERIOD       5000

DynamicJsonDocument doc(8192);



String activeStr;
unsigned long   last_update;
unsigned long   update_ID = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SDB, OUTPUT);
  digitalWrite(SDB, HIGH);

  connectToWiFi();

  doc = downloadJSON();

  Wire.begin();


  setupLEDDrivers();

}

void loop() {
  //drawFrame();
  doc = downloadJSON();
  //JsonObject& result_0_message = doc["result"][0]["message"];
  last_update = millis();
  Serial.println("Telegram Updated");
  Serial.println(doc["ok"].as<String>());
  Serial.println(doc["result"]["text"].as<String>());
  Serial.print("Print Update_ID");
  update_ID = doc["result"][1]["update_id"].as<int>();
  Serial.println(String(update_ID, DEC));
  String str = doc["result"][0]["message"]["text"].as<String>();
  Serial.println(str);
  char copy[13];
  str.toCharArray(copy, 13);
  drawText(copy);
  /*
    drawText("RADIO Popova");*/
  while (last_update + UPDATE_PERIOD > millis());
}
