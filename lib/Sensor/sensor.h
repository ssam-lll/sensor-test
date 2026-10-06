#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"

class Sensor {
private:
    int _pin;

public:
    Sensor(int pin);
    int read();

};

#endif