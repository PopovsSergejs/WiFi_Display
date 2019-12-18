///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

char server[] = "api.telegram.org";

int status = WL_IDLE_STATUS;

WiFiSSLClient client;

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
  Serial.println("YOU ARE HERE!!!!");
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:







  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET /bot1025954619:AAEVIvRXUEz_hwkIzzY_iMIzUGrNrJxsLTw/getUpdates");
    if (update_ID)  {
      client.print("?offset=");
      client.print(String(update_ID, DEC));
    }
    client.println(" HTTP/1.1");
    client.println("Host: api.telegram.org");
    client.println("Connection: close");
    client.println();
  }
  while (!client.available());
  char counter = 0;
  while (counter != 11) {
    char c = client.read();
    if (c == '\n')  {
      counter++;
    }
      
  }


    DynamicJsonDocument doc(8192);
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
