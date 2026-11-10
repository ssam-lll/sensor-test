#include "sensor.h"

BarraSensores::BarraSensores(Multiplexor& mux) : _mux(mux) {}

void BarraSensores::begin() {
}

void BarraSensores::leerBarra(int valores[8]) {
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        valores[i] = _mux.leerCanal(_canales[i]);
    }
}

int BarraSensores::leerBarraConDiagnostico(int valores[8]) {
    leerBarra(valores);
    int errorGeneral = 0;
    for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        int err = verificarError(valores[i]);
        if (err != 0) {
            errorGeneral = err;
        }
    }
    return errorGeneral;
}

int BarraSensores::verificarError(int valor) {
    if (valor < 0 || valor > 4095) {
        return 101;
    }
    return 0;
}
