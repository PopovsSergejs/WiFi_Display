 ///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

char server[] = "krivoy.co.uk";

int status = WL_IDLE_STATUS;

WiFiClient client;

void connectToWiFi()  {
  //Check for WiFi Module
  if (WiFi.status() == WL_NO_MODULE)  {
    Serial.println("No WiFi Module");
    delay(10000);
    //RESET DEVICE
  }

  //Connect to wifi every 10 seconds
  while (status != WL_CONNECTED)  {
    Serial.print("Attempting to connect to SSID");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.println("WiFi Connected");
  printWiFiStatus();
}

DynamicJsonDocument downloadJSON() {
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /xmas-arduino-get");
    client.println("Host: krivoy.co.uk");
    client.println("Connection: close");
    client.println();
  }
  while(!client.available());
  
  DynamicJsonDocument doc(2048);
  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  }
  return doc;
}

void printWiFiStatus()  {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
