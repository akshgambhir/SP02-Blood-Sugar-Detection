#include <Wire.h>
#include "MAX30105.h"
#include <math.h>

MAX30105 particleSensor;

#define debug Serial //Uncomment this line if you're using an Uno or ESP
//#define debug SerialUSB //Uncomment this line if you're using a SAMD21

// Constants for Beer-Lambert law
const float pathLength = 1.0; // Placeholder value for path length of the sample in cm

void setup()
{
  debug.begin(9600);
  debug.println("MAX30102 Basic Readings Example");

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    debug.println("MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
}

// Function to calculate blood sugar using Beer-Lambert law
float calculateBloodSugar(float absorbance, float molarAbsorptivity) {
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

void loop()
{
  // Read absorbance from sensor (replace with actual sensor reading)
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
  }
  
  debug.println();

  delay(1000); // Add a delay to avoid flooding the serial monitor
}



