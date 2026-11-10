#include "telemetria.h"
#include <ArduinoJson.h>

static Telemetria* instanciaGlobal = nullptr;

static void mqttStaticCallback(char* topic, byte* payload, unsigned int length) {
    if (instanciaGlobal) {
        char buffer[length + 1];
        memcpy(buffer, payload, length);
        buffer[length] = '\0';
        instanciaGlobal->procesarMensaje(topic, (byte*)buffer, length);
    }
}

Telemetria::Telemetria(const char* ssid, const char* pass, const char* broker, int port, Motor& motor, const char* clientId)
    : _ssid(ssid), _pass(pass), _broker(broker), _port(port), _clientId(clientId),
      _motor(motor), _espClient(), _mqtt(_espClient),
      _lastReconnectMillis(0), _lastWifiReconnectMillis(0) {
    instanciaGlobal = this;
}

void Telemetria::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);
    _mqtt.setServer(_broker, _port);
    _mqtt.setCallback(mqttStaticCallback);
    _mqtt.setBufferSize(512);
}

void Telemetria::mantenerConexion(unsigned long currentMillis) {
    if (WiFi.status() != WL_CONNECTED) {
        if (currentMillis - _lastWifiReconnectMillis >= _wifiReconnectInterval) {
            _lastWifiReconnectMillis = currentMillis;
            WiFi.reconnect();
        }
    } else {
        if (!_mqtt.connected() && (currentMillis - _lastReconnectMillis >= _reconnectInterval)) {
            _lastReconnectMillis = currentMillis;
            if (_mqtt.connect(_clientId)) {
                _mqtt.subscribe(TOPIC_CONTROL_MOTORES, 1);
            }
        }
        if (_mqtt.connected()) {
            _mqtt.loop();
        }
    }
}

void Telemetria::procesarMensaje(char* topic, byte* payload, unsigned int length) {
    if (strcmp(topic, TOPIC_CONTROL_MOTORES) == 0) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, (char*)payload);
        if (error) return;

        const char* cmd = doc["cmd"] | "";
        int m1 = doc["m1"] | 0;
        int m2 = doc["m2"] | 0;

        if (strcmp(cmd, "set_motores") == 0) {
            _motor.setMotores(m1, m2);
            publicarEstadoMotores(_motor.getVelocidadM1(), _motor.getVelocidadM2(), "OK");
        } else if (strcmp(cmd, "stop") == 0) {
            _motor.frenar();
            publicarEstadoMotores(0, 0, "STOPPED");
        } else {
            publicarEstadoMotores(_motor.getVelocidadM1(), _motor.getVelocidadM2(), "UNKNOWN_CMD");
        }
    }
}

void Telemetria::publicarSensores(const int valores[8], int error) {
    JsonDocument doc;
    JsonArray arr = doc["sensores"].to<JsonArray>();
    for (int i = 0; i < 8; i++) {
        arr.add(valores[i]);
    }
    doc["error"] = error;

    char buffer[256];
    serializeJson(doc, buffer);

    if (_mqtt.connected()) {
        _mqtt.publish(TOPIC_SENSORES, buffer);
    }
}

void Telemetria::publicarEstadoMotores(int m1, int m2, const char* status) {
    JsonDocument doc;
    doc["m1"] = m1;
    doc["m2"] = m2;
    doc["status"] = status;

    char buffer[128];
    serializeJson(doc, buffer);

    if (_mqtt.connected()) {
        _mqtt.publish(TOPIC_ESTADO_MOTORES, buffer);
    }
}
