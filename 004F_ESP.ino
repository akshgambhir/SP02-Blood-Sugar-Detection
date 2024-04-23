#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";

IPAddress server(184, 106, 153, 149); // ThingSpeak server IP
const int httpPort = 80;

void setup() {
  Serial.begin(9600);
  delay(10);

  connectWiFi();

  Serial.println("Setup done");
}

void loop() {
  // Nothing to do in loop for this example
}

void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
