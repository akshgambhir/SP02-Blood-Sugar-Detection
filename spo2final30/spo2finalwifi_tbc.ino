#include <Wire.h>
#include "MAX30105.h"
#include <SoftwareSerial.h>

MAX30105 particleSensor;
SoftwareSerial espSerial(2, 3); // RX, TX for ESP-01

#define debug Serial //Uncomment this line if you're using an Uno or ESP
//#define debug SerialUSB //Uncomment this line if you're using a SAMD21

// Wi-Fi credentials
const char* ssid = "Aksh's Galaxy S20 FE";
const char* password = "1234567890";

void setup()
{
  debug.begin(9600);
  espSerial.begin(9600);

  debug.println("MAX30102 Basic Readings Example");

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    debug.println("MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive

  // Connect to Wi-Fi
  connectWiFi();
}

// Function to connect to Wi-Fi
void connectWiFi() {
  debug.println("Connecting to WiFi...");
  espSerial.println("AT+RST"); // Reset ESP-01 module
  delay(1000);
  espSerial.println("AT+CWMODE=1"); // Set ESP-01 to station mode
  delay(1000);
  espSerial.print("AT+CWJAP=\"");
  espSerial.print(ssid);
  espSerial.print("\",\"");
  espSerial.print(password);
  espSerial.println("\""); // Connect to Wi-Fi network
  delay(5000); // Wait for connection
  if (espSerial.find("OK")) {
    debug.println("Connected to WiFi.");
  } else {
    debug.println("Failed to connect to WiFi.");
  }
}

// Function to send data to Blink
void sendDataToBlink(float data) {
  debug.println("Sending data to Blink...");
  espSerial.println("AT+CIPSTART=\"TCP\",\"things.ubidots.com\",80");
  if (espSerial.find("OK")) {
    debug.println("Connected to Blink server.");
    String postRequest = "POST /api/v1.6/devices/your-device-id HTTP/1.1\r\n";
    postRequest += "Host: things.ubidots.com\r\n";
    postRequest += "X-Auth-Token: your-token\r\n";
    postRequest += "Content-Type: application/json\r\n";
    postRequest += "Content-Length: 26\r\n\r\n";
    postRequest += "{\"value\":" + String(data) + "}\r\n";
    String postLength = String(postRequest.length());
    espSerial.println("AT+CIPSEND=" + postLength);
    if (espSerial.find(">")) {
      debug.println("Sending data...");
      espSerial.print(postRequest);
      if (espSerial.find("SEND OK")) {
        debug.println("Data sent successfully.");
      } else {
        debug.println("Failed to send data.");
      }
    }
  }
}

void loop()
{
  // Read absorbance from sensor
  float absorbance = getAbsorbance(); // You need a function to get absorbance from your sensor

  debug.print(" R[");
  debug.print(particleSensor.getRed());
  debug.print("] IR[");
  debug.print(particleSensor.getIR());
  debug.print("] Absorbance: ");
  debug.print(absorbance, 4); // Print absorbance with 4 decimal places
  
  if (particleSensor.getIR() < 5000) {
    debug.print(" No finger detected");
  } else {
    // Calculate molar absorptivity using a placeholder value
    float molarAbsorptivity = 0.1; // Placeholder value for molar absorptivity coefficient for glucose

    // Calculate blood sugar
    float bloodSugar = calculateBloodSugar(absorbance, molarAbsorptivity);

    // Print blood sugar
    debug.print(" Blood sugar: ");
    debug.print(bloodSugar);
    debug.print(" mg/dL");

    // Send data to Blink
    sendDataToBlink(bloodSugar);
  }
  
  debug.println();

  delay(1000); // Add a delay to avoid flooding the serial monitor
}

// Function to calculate blood sugar using Beer-Lambert law
float calculateBloodSugar(float absorbance, float molarAbsorptivity) {
    // Constants for Beer-Lambert law
    const float pathLength = 1.0; // Placeholder value for path length of the sample in cm
    // Calculate concentration using Beer-Lambert law: A = ε * c * l
    float concentration = absorbance*10 / (molarAbsorptivity * pathLength);
    return concentration;
}

// Function to get absorbance using the formula -log(irValue/65536)
float getAbsorbance() {
    // Replace this with actual IR value from your sensor
    int irValue = particleSensor.getIR(); // Get the IR reading

    // Calculate absorbance using the formula -log(irValue/65536)
    float absorbance = -log((float)irValue / 65536.0);
    return absorbance;
}
