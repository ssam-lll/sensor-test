#ifndef TELEMETRIA_H
#define TELEMETRIA_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "motor.h"

#define TOPIC_SENSORES         "telemetria/sensores"
#define TOPIC_CONTROL_MOTORES  "control/motores"
#define TOPIC_ESTADO_MOTORES   "telemetria/motores/estado"

class Telemetria {
private:
    const char* _ssid;
    const char* _pass;
    const char* _broker;
    int _port;
    const char* _clientId;
    Motor& _motor;
    WiFiClient _espClient;
    PubSubClient _mqtt;
    unsigned long _lastReconnectMillis;
    const unsigned long _reconnectInterval = 3000;
    unsigned long _lastWifiReconnectMillis;
    const unsigned long _wifiReconnectInterval = 5000;

public:
    Telemetria(const char* ssid, const char* pass, const char* broker, int port, Motor& motor, const char* clientId = "ESP32_IDLine");
    void begin();
    void mantenerConexion(unsigned long currentMillis);
    void procesarMensaje(char* topic, byte* payload, unsigned int length);
    void publicarSensores(const int valores[8], int error);
    void publicarEstadoMotores(int m1, int m2, const char* status);
};

#endif
