#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
private:
    uint8_t _pinSTBY;
    uint8_t _pinAIN1;
    uint8_t _pinAIN2;
    uint8_t _pinPWMA;
    uint8_t _pinBIN1;
    uint8_t _pinBIN2;
    uint8_t _pinPWMB;

    int _velM1;
    int _velM2;

public:
    Motor(uint8_t stby, uint8_t ain1, uint8_t ain2, uint8_t pwma,
          uint8_t bin1, uint8_t bin2, uint8_t pwmb);

    void begin();
    void setMotorIzquierdo(int velocidad);
    void setMotorDerecho(int velocidad);
    void setMotores(int velM1, int velM2);
    void frenar();
    void ejecutarPrueba();

    int getVelocidadM1() const { return _velM1; }
    int getVelocidadM2() const { return _velM2; }
};

#endif
