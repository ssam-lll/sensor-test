#ifndef TELEMETRIA_H
#define TELEMETRIA_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <functional>

typedef std::function<void(const char* cmd, int m1, int m2)> ComandoMotorCallback;

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

    ComandoMotorCallback _motorCallback;

public:
    Telemetria(const char* ssid, const char* pass, const char* broker, int port, const char* clientId = "ESP32_IDLine");
    void setMotorCallback(ComandoMotorCallback callback);
    void begin();
    void mantenerConexion(unsigned long currentMillis);
    void procesarMensaje(char* topic, byte* payload, unsigned int length);
    void publicarSensores(const int valores[8], int error);
    void publicarEstadoMotores(int m1, int m2, const char* status);
};

#endif
