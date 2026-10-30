#include <Arduino.h>
#include "mux.h"
#include "sensor.h"
#include "motor.h"
#include "telemetria.h"

const uint8_t PIN_MUX_S0  = 27;
const uint8_t PIN_MUX_S1  = 14;
const uint8_t PIN_MUX_S2  = 13;
const uint8_t PIN_MUX_S3  = 15;
const uint8_t PIN_MUX_SIG = 39;

const uint8_t PIN_STBY = 12;
const uint8_t PIN_AIN1 = 21;
const uint8_t PIN_AIN2 = 22;
const uint8_t PIN_PWMA = 4;
const uint8_t PIN_BIN1 = 25;
const uint8_t PIN_BIN2 = 26;
const uint8_t PIN_PWMB = 16;

const char* WIFI_SSID   = "ANDRES-LOPEZ";
const char* WIFI_PASS   = "Gallego162783";
const char* MQTT_BROKER = "192.168.1.40";
const int   MQTT_PORT   = 1883;

Multiplexor   mux(PIN_MUX_S0, PIN_MUX_S1, PIN_MUX_S2, PIN_MUX_S3, PIN_MUX_SIG);
BarraSensores barra(mux);
Motor         motor(PIN_STBY, PIN_AIN1, PIN_AIN2, PIN_PWMA, PIN_BIN1, PIN_BIN2, PIN_PWMB);
Telemetria    telemetria(WIFI_SSID, WIFI_PASS, MQTT_BROKER, MQTT_PORT);

unsigned long previousMillis = 0;
const unsigned long intervalLectura = 200;

void manejarComandoMotor(const char* cmd, int m1, int m2) {
    Serial.printf("Comando MQTT: [%s], m1=%d, m2=%d\n", cmd, m1, m2);

    if (strcmp(cmd, "set_motores") == 0) {
        motor.setMotores(m1, m2);
        telemetria.publicarEstadoMotores(motor.getVelocidadM1(), motor.getVelocidadM2(), "OK");
    } else if (strcmp(cmd, "stop") == 0) {
        motor.frenar();
        telemetria.publicarEstadoMotores(0, 0, "STOPPED");
    } else if (strcmp(cmd, "test") == 0) {
        telemetria.publicarEstadoMotores(0, 0, "TESTING");
        motor.ejecutarPrueba();
        telemetria.publicarEstadoMotores(0, 0, "TEST_COMPLETED");
    } else {
        telemetria.publicarEstadoMotores(motor.getVelocidadM1(), motor.getVelocidadM2(), "UNKNOWN_CMD");
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n--- IDLine ESP32 Sensor & Motor Hub ---");

    mux.begin();
    barra.begin();
    motor.begin();

    telemetria.setMotorCallback(manejarComandoMotor);
    telemetria.begin();
    Serial.println("Inicializacion completada.");
}

void loop() {
    unsigned long currentMillis = millis();
    telemetria.mantenerConexion(currentMillis);

    if (currentMillis - previousMillis >= intervalLectura) {
        previousMillis = currentMillis;

        int lecturas[8];
        barra.leerBarra(lecturas);

        int errorGeneral = 0;
        for (int i = 0; i < 8; i++) {
            int err = barra.verificarError(lecturas[i]);
            if (err != 0) {
                errorGeneral = err;
            }
        }

        telemetria.publicarSensores(lecturas, errorGeneral);
    }
}
