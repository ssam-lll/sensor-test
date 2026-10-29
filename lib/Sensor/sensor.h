#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "mux.h"

class BarraSensores {
private:
    Multiplexor& _mux;
    static const uint8_t NUM_SENSORES = 8;
    const uint8_t _canales[NUM_SENSORES] = {4, 5, 6, 7, 8, 9, 10, 11};

public:
    BarraSensores(Multiplexor& mux);
    void begin();
    void leerBarra(int valores[8]);
    int verificarError(int valor);
};

typedef BarraSensores Sensor;

#endif
