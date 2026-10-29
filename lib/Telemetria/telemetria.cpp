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

Telemetria::Telemetria(const char* ssid, const char* pass, const char* broker, int port, const char* clientId)
    : _ssid(ssid), _pass(pass), _broker(broker), _port(port), _clientId(clientId),
      _mqtt(_espClient), _lastReconnectMillis(0), _motorCallback(nullptr) {
    instanciaGlobal = this;
}

void Telemetria::setMotorCallback(ComandoMotorCallback callback) {
    _motorCallback = callback;
}

void Telemetria::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);
    _mqtt.setServer(_broker, _port);
    _mqtt.setCallback(mqttStaticCallback);
    _mqtt.setBufferSize(512);
}

void Telemetria::mantenerConexion(unsigned long currentMillis) {
    if (WiFi.status() == WL_CONNECTED) {
        if (!_mqtt.connected() && (currentMillis - _lastReconnectMillis >= _reconnectInterval)) {
            _lastReconnectMillis = currentMillis;
            Serial.print("Conectando a broker MQTT...");
            if (_mqtt.connect(_clientId)) {
                Serial.println(" Conectado");
                _mqtt.subscribe("control/motores", 1);
                Serial.println("Suscrito a: control/motores (QoS 1)");
            } else {
                Serial.printf(" Fallo (rc=%d)\n", _mqtt.state());
            }
        }
        if (_mqtt.connected()) {
            _mqtt.loop();
        }
    }
}

void Telemetria::procesarMensaje(char* topic, byte* payload, unsigned int length) {
    if (strcmp(topic, "control/motores") == 0) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, (char*)payload);
        if (error) {
            Serial.printf("Error deserializando JSON MQTT: %s\n", error.c_str());
            return;
        }

        const char* cmd = doc["cmd"] | "";
        int m1 = doc["m1"] | 0;
        int m2 = doc["m2"] | 0;

        if (_motorCallback) {
            _motorCallback(cmd, m1, m2);
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
        _mqtt.publish("telemetria/sensores", buffer);
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
        _mqtt.publish("telemetria/motores/estado", buffer);
    }
}
