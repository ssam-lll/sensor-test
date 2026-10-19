#include "telemetria.h"

Telemetria::Telemetria(const char* ssid, const char* pass, const char* broker, int port, const char* clientId)
    : _ssid(ssid), _pass(pass), _broker(broker), _port(port), _clientId(clientId), _mqtt(_espClient), _lastReconnectMillis(0) {}

void Telemetria::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);
    _mqtt.setServer(_broker, _port);
}

void Telemetria::mantenerConexion(unsigned long currentMillis) {
    if (WiFi.status() == WL_CONNECTED) {
        if (!_mqtt.connected() && (currentMillis - _lastReconnectMillis >= _reconnectInterval)) {
            _lastReconnectMillis = currentMillis;
            _mqtt.connect(_clientId);
        }
        if (_mqtt.connected()) {
            _mqtt.loop();
        }
    }
}

void Telemetria::publicar(const char* topic, const char* nombreSensor, int valor, int error) {
    char payload[96];
    snprintf(payload, sizeof(payload), "{\"sensor\":\"%s\",\"valor\":%d,\"error\":%d}", nombreSensor, valor, error);
    Serial.println(payload);
    if (_mqtt.connected()) {
        _mqtt.publish(topic, payload);
    }
}
