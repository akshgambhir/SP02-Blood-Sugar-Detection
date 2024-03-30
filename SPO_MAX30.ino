#include <Wire.h>
#include <MAX30105.h>
#include "spo2_algorithm.h"

MAX30105 particleSensor;

// Beer-Lambert Law Constants (These need to be determined through calibration)
float molarAbsorptivity = 0.48; // Placeholder value, replace with actual value for glucose
float pathLength = 0.47; // Placeholder value, replace with actual path length in cm

void setup() {
  Serial.begin(115200);
  
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }
  
  particleSensor.setup();  
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
}

void loop() {
  float red, ir;

  while (!Serial.available()) {
    // Continue reading sensor data as long as no user input is received
    if (particleSensor.available()) {
      particleSensor.check();  // Update the sensor data
      
      red = particleSensor.getRed();
      ir = particleSensor.getIR();
      
      Serial.print("Red: ");
      Serial.print(red);
      Serial.print(", IR: ");
      Serial.println(ir);

      // Use Beer-Lambert Law to estimate glucose concentration
      float absorbance = -log10(ir / red); // Assuming red light is used for measurement
      
      // Apply Beer-Lambert Law
      float concentration = absorbance / (molarAbsorptivity * pathLength);

      Serial.print("Absorbance: ");
      Serial.print(absorbance);
      Serial.print(", Concentration: ");
      Serial.println(concentration);

      delay(1000);
    }
  }

  // If user input is available, stop the program
  Serial.println("User input detected. Program stopped.");
  while (Serial.available()) {
    Serial.read();  // Clear any remaining input
  }
  delay(1000);  // Delay for stability, adjust as needed
}
