#include <Arduino.h>
#include "sensor.h"
#include "telemetria.h"

const char* WIFI_SSID = "ANDRES-LOPEZ";
const char* WIFI_PASS = "Gallego162783";
const char* MQTT_BROKER = "192.168.1.40";
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC = "telemetria/infrarrojo";

Sensor sensor(34);
Telemetria telemetria(WIFI_SSID, WIFI_PASS, MQTT_BROKER, MQTT_PORT);

unsigned long previousMillis = 0;
const unsigned long interval = 1000;

void setup() {
  Serial.begin(115200);
  telemetria.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  telemetria.mantenerConexion(currentMillis);

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int valor = sensor.leer();
    int error = sensor.verificarError(valor);
    telemetria.publicar(MQTT_TOPIC, "infrarrojo", valor, error);
  }
}