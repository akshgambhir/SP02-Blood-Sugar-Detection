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
    Serial.print("IR: ");
    Serial.print(pox.getIR());
    Serial.print("  Red: ");
    Serial.println(pox.getRed());
  }
}
