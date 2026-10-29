#include "sensor.h"

BarraSensores::BarraSensores(Multiplexor& mux) : _mux(mux) {}

void BarraSensores::begin() {
}

void BarraSensores::leerBarra(int valores[8]) {
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        valores[i] = _mux.leerCanal(_canales[i]);
    }
}

int BarraSensores::verificarError(int valor) {
    if (valor <= 0 || valor > 4095) {
        return 101;
    }
    return 0;
}
