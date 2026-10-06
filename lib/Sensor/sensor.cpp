#include "sensor.h"

Sensor::Sensor(int pin) {
    _pin = pin;
}

int Sensor::read() {
    return analogRead(_pin);
}