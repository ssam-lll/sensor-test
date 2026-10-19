#include "sensor.h"

Sensor::Sensor(int pin) {
    _pin = pin;
}

int Sensor::leer() {
    return analogRead(_pin);
}

int Sensor::verificarError(int valor) {
    if (valor <= 0 || valor > 4095) {
        return 101;
    }
    return 0;
}