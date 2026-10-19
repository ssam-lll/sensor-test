#ifndef TELEMETRIA_H
#define TELEMETRIA_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class Telemetria {
private:
    const char* _ssid;
    const char* _pass;
    const char* _broker;
    int _port;
    const char* _clientId;
    WiFiClient _espClient;
    PubSubClient _mqtt;
    unsigned long _lastReconnectMillis;
    const unsigned long _reconnectInterval = 3000;

public:
    Telemetria(const char* ssid, const char* pass, const char* broker, int port, const char* clientId = "ESP32_SensorIR");
    void begin();
    void mantenerConexion(unsigned long currentMillis);
    void publicar(const char* topic, const char* nombreSensor, int valor, int error);
};

#endif
