#include "mux.h"

Multiplexor::Multiplexor(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t sig)
    : _pinS0(s0), _pinS1(s1), _pinS2(s2), _pinS3(s3), _pinSig(sig) {}

void Multiplexor::begin() {
    pinMode(_pinS0, OUTPUT);
    pinMode(_pinS1, OUTPUT);
    pinMode(_pinS2, OUTPUT);
    pinMode(_pinS3, OUTPUT);
    pinMode(_pinSig, INPUT);

    digitalWrite(_pinS0, LOW);
    digitalWrite(_pinS1, LOW);
    digitalWrite(_pinS2, LOW);
    digitalWrite(_pinS3, LOW);
}

void Multiplexor::seleccionarCanal(uint8_t canal) {
    canal &= 0x0F;
    digitalWrite(_pinS0, (canal & 0x01) ? HIGH : LOW);
    digitalWrite(_pinS1, (canal & 0x02) ? HIGH : LOW);
    digitalWrite(_pinS2, (canal & 0x04) ? HIGH : LOW);
    digitalWrite(_pinS3, (canal & 0x08) ? HIGH : LOW);
    delayMicroseconds(5);
}

int Multiplexor::leerCanal(uint8_t canal) {
    seleccionarCanal(canal);
    return analogRead(_pinSig);
}
