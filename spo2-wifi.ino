//Arduino Uno to MAX30105:
//5V pin to MAX30105's VCC pin.
//GND pin to MAX30105's GND pin.
// A4 (SDA) pin to MAX30105's SDA pin.
// A5 (SCL) pin to MAX30105's SCL pin.

//Arduino Uno to ESP01:
// D3 (TX) pin to ESP01's RX pin.
// D2 (RX) pin to ESP01's TX pin.
// GND pin to ESP01's GND pin.
// +5V pin to ESP01's VCC pin.

#include <Wire.h>
#include <MAX30105.h>
#include "spo2_algorithm.h"
#include <SoftwareSerial.h>

MAX30105 particleSensor;

const char* ssid = "YourSSID";
const char* password = "YourPassword";
const char* server = "api.thingspeak.com";
const char* apiKey = "YourAPIKey";

SoftwareSerial espSerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  Serial.println("Initializing MAX30105...");
  
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }
  
  particleSensor.setup();  
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  Serial.println("MAX30105 initialized successfully.");
  
  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  espSerial.println("AT+RST");
  delay(1000);
  espSerial.println("AT+CWMODE=1");
  delay(1000);
  espSerial.print("AT+CWJAP=\"");
  espSerial.print(ssid);
  espSerial.print("\",\"");
  espSerial.print(password);
  espSerial.println("\"");
}

void loop() {
  float red, ir;
  float spo2;
  
  if (particleSensor.available()) {
    particleSensor.check();  // Update the sensor data
    
    red = particleSensor.getRed();
    ir = particleSensor.getIR();

    // Implement Beer-Lambert Law to calculate blood sugar levels
    spo2 = calculateBloodSugar(red, ir);
    
    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(", IR: ");
    Serial.print(ir);
    Serial.print(", Blood Sugar (mg/dL): ");
    Serial.println(spo2);
    
    // Send data to ThingSpeak
    sendDataToThingSpeak(spo2);
    
    delay(1000); // Adjust delay as needed
  } else {
    Serial.println("No data available from MAX30105.");
  }
}

float calculateBloodSugar(float red, float ir) {
  // Implement your Beer-Lambert Law algorithm here
  // This is just a placeholder
  // Replace it with your actual algorithm
  return (red + ir) * 10; // Example calculation
}

void sendDataToThingSpeak(float spo2) {
  if (espSerial.available()) {
    while (espSerial.available()) {
      espSerial.read();
    }
  }
  
  espSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
  if (espSerial.find("OK")) {
    String postStr = "field1=" + String(spo2);

    String httpRequest = "POST /update HTTP/1.1\r\n";
    httpRequest += "Host: api.thingspeak.com\r\n";
    httpRequest += "Connection: close\r\n";
    httpRequest += "X-THINGSPEAKAPIKEY: " + String(apiKey) + "\r\n";
    httpRequest += "Content-Type: application/x-www-form-urlencoded\r\n";
    httpRequest += "Content-Length: " + String(postStr.length()) + "\r\n\r\n";
    httpRequest += postStr;

    String cmd = "AT+CIPSEND=";
    cmd += httpRequest.length();
    espSerial.println(cmd);

    if (espSerial.find(">")) {
      espSerial.print(httpRequest);

      Serial.println("Data sent to ThingSpeak");
    }
  }
  espSerial.println("AT+CIPCLOSE");
}
