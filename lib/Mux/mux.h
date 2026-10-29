#ifndef MUX_H
#define MUX_H

#include <Arduino.h>

class Multiplexor {
private:
    uint8_t _pinS0;
    uint8_t _pinS1;
    uint8_t _pinS2;
    uint8_t _pinS3;
    uint8_t _pinSig;

public:
    Multiplexor(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t sig);
    void begin();
    void seleccionarCanal(uint8_t canal);
    int leerCanal(uint8_t canal);
};

#endif
