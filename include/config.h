#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Multiplexor CD74HC4067 (Pines de direccionamiento y señal analógica)
const uint8_t PIN_MUX_S0  = 27;
const uint8_t PIN_MUX_S1  = 14;
const uint8_t PIN_MUX_S2  = 13;
const uint8_t PIN_MUX_S3  = 15;
const uint8_t PIN_MUX_SIG = 39; // GPIO39 / VN (ADC1_CH3)

// Driver Puente H TB6612FNG
const uint8_t PIN_STBY = 12;
const uint8_t PIN_AIN1 = 21;
const uint8_t PIN_AIN2 = 22;
const uint8_t PIN_PWMA = 4;
const uint8_t PIN_BIN1 = 25;
const uint8_t PIN_BIN2 = 26;
const uint8_t PIN_PWMB = 17;

// Parámetros de Red y Broker MQTT
const char* const WIFI_SSID       = "ANDRES-LOPEZ";
const char* const WIFI_PASS       = "Gallego162783";
const char* const MQTT_BROKER     = "192.168.1.40";
const int         MQTT_PORT       = 1883;
const char* const MQTT_CLIENT_ID  = "ESP32_IDLine";

// Temporización de muestreo
const unsigned long INTERVALO_LECTURA_MS = 500;

#endif // CONFIG_H
