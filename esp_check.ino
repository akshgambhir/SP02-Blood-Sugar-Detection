#include <SoftwareSerial.h>

SoftwareSerial espSerial(0, 1); // RX, TX

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    espSerial.write(c);
  }
  
  if (espSerial.available()) {
    char c = espSerial.read();
    Serial.write(c);
  }
}
