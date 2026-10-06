#include <Arduino.h>
#include "sensor.h"

Sensor sensor(34);

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println(sensor.read());
  }
}