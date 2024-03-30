#include <Wire.h>
#include <MAX30105.h>
#include "spo2_algorithm.h"

MAX30105 particleSensor;

void setup() {
  Serial.begin(9600);

  Serial.println("Initializing MAX30105...");
  
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }
  
  particleSensor.setup();  
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  Serial.println("MAX30105 initialized successfully.");
}

void loop() {
  float red, ir;

  if (particleSensor.available()) {
    particleSensor.check();  // Update the sensor data
    
    red = particleSensor.getRed();
    ir = particleSensor.getIR();
    
    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(", IR: ");
    Serial.println(ir);
    
    delay(1000);
  } else {
    Serial.println("No data available from MAX30105.");
  }
}
