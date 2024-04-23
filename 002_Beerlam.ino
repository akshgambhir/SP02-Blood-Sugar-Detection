#include <Wire.h>
#include <MAX30100_PulseOximeter.h>

#define MAX30100_ADDRESS 0x57
#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;
uint32_t lastReportTime = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pox.begin();
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  pox.update();

  if (millis() - lastReportTime > REPORTING_PERIOD_MS) {
    lastReportTime = millis();
    float redIntensity = pox.getRed();
    float blueIntensity = pox.getIR();
    
    float redAbsorbance = -log(redIntensity);
    float blueAbsorbance = -log(blueIntensity);
    
    // Use calibration curve to convert absorbance to blood glucose level
    float glucoseLevel = interpolateGlucose(redAbsorbance, blueAbsorbance);
    
    Serial.print("Blood Glucose Level: ");
    Serial.print(glucoseLevel);
    Serial.println(" mg/dL");
  }
}

float interpolateGlucose(float redAbsorbance, float blueAbsorbance) {
 float interpolateGlucose(float redAbsorbance, float blueAbsorbance) {
  // Example calibration curve
  // This is just a placeholder, you'll need to replace it with your actual calibration curve
  // These values are just for demonstration purposes
  float redCalibrationPoints[] = {0.1, 0.2, 0.3}; // Red absorbance values
  float glucoseLevels[] = {100, 150, 200}; // Corresponding blood glucose levels in mg/dL
  
  int numPoints = sizeof(redCalibrationPoints) / sizeof(redCalibrationPoints[0]);
  
  // Find the two closest calibration points for interpolation
  int index1 = 0;
  while (index1 < numPoints && redCalibrationPoints[index1] < redAbsorbance) {
    index1++;
  }
  
  // Handle cases where redAbsorbance is outside the calibration range
  if (index1 == 0) {
    return glucoseLevels[0]; // If below the lowest calibration point
  } else if (index1 == numPoints) {
    return glucoseLevels[numPoints - 1]; // If above the highest calibration point
  }
  
  // Perform linear interpolation
  int index0 = index1 - 1;
  float redFraction = (redAbsorbance - redCalibrationPoints[index0]) / (redCalibrationPoints[index1] - redCalibrationPoints[index0]);
  float glucoseLevel = glucoseLevels[index0] + (glucoseLevels[index1] - glucoseLevels[index0]) * redFraction;
  
  return glucoseLevel;
}

}
