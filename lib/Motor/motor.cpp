#include "motor.h"

Motor::Motor(uint8_t stby, uint8_t ain1, uint8_t ain2, uint8_t pwma,
             uint8_t bin1, uint8_t bin2, uint8_t pwmb)
    : _pinSTBY(stby), _pinAIN1(ain1), _pinAIN2(ain2), _pinPWMA(pwma),
      _pinBIN1(bin1), _pinBIN2(bin2), _pinPWMB(pwmb),
      _velM1(0), _velM2(0) {}

void Motor::begin() {
    pinMode(_pinSTBY, OUTPUT);
    pinMode(_pinAIN1, OUTPUT);
    pinMode(_pinAIN2, OUTPUT);
    pinMode(_pinPWMA, OUTPUT);
    pinMode(_pinBIN1, OUTPUT);
    pinMode(_pinBIN2, OUTPUT);
    pinMode(_pinPWMB, OUTPUT);

    digitalWrite(_pinSTBY, HIGH);
    frenar();
}

void Motor::setMotorIzquierdo(int velocidad) {
    _velM1 = constrain(velocidad, -255, 255);
    digitalWrite(_pinSTBY, HIGH);

    if (_velM1 > 0) {
        digitalWrite(_pinAIN1, HIGH);
        digitalWrite(_pinAIN2, LOW);
        analogWrite(_pinPWMA, _velM1);
    } else if (_velM1 < 0) {
        digitalWrite(_pinAIN1, LOW);
        digitalWrite(_pinAIN2, HIGH);
        analogWrite(_pinPWMA, -_velM1);
    } else {
        digitalWrite(_pinAIN1, LOW);
        digitalWrite(_pinAIN2, LOW);
        analogWrite(_pinPWMA, 0);
    }
}

void Motor::setMotorDerecho(int velocidad) {
    _velM2 = constrain(velocidad, -255, 255);
    digitalWrite(_pinSTBY, HIGH);

    if (_velM2 > 0) {
        digitalWrite(_pinBIN1, HIGH);
        digitalWrite(_pinBIN2, LOW);
        analogWrite(_pinPWMB, _velM2);
    } else if (_velM2 < 0) {
        digitalWrite(_pinBIN1, LOW);
        digitalWrite(_pinBIN2, HIGH);
        analogWrite(_pinPWMB, -_velM2);
    } else {
        digitalWrite(_pinBIN1, LOW);
        digitalWrite(_pinBIN2, LOW);
        analogWrite(_pinPWMB, 0);
    }
}

void Motor::setMotores(int velM1, int velM2) {
    setMotorIzquierdo(velM1);
    setMotorDerecho(velM2);
}

void Motor::frenar() {
    _velM1 = 0;
    _velM2 = 0;
    digitalWrite(_pinAIN1, LOW);
    digitalWrite(_pinAIN2, LOW);
    analogWrite(_pinPWMA, 0);
    digitalWrite(_pinBIN1, LOW);
    digitalWrite(_pinBIN2, LOW);
    analogWrite(_pinPWMB, 0);
}

void Motor::ejecutarPrueba() {
    setMotorIzquierdo(180);
    delay(400);
    frenar();
    delay(150);

    setMotorIzquierdo(-180);
    delay(400);
    frenar();
    delay(150);

    setMotorDerecho(180);
    delay(400);
    frenar();
    delay(150);

    setMotorDerecho(-180);
    delay(400);
    frenar();
}
