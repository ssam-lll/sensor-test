#include <Arduino.h>
#include "config.h"
#include "mux.h"
#include "sensor.h"
#include "motor.h"
#include "telemetria.h"

Multiplexor   mux(PIN_MUX_S0, PIN_MUX_S1, PIN_MUX_S2, PIN_MUX_S3, PIN_MUX_SIG);
BarraSensores barra(mux);
Motor         motor(PIN_STBY, PIN_AIN1, PIN_AIN2, PIN_PWMA, PIN_BIN1, PIN_BIN2, PIN_PWMB);
Telemetria    telemetria(WIFI_SSID, WIFI_PASS, MQTT_BROKER, MQTT_PORT, motor, MQTT_CLIENT_ID);

unsigned long previousMillis = 0;

void setup() {
    mux.begin();
    barra.begin();
    motor.begin();
    telemetria.begin();
}

void loop() {
    unsigned long currentMillis = millis();
    telemetria.mantenerConexion(currentMillis);

    if (currentMillis - previousMillis >= INTERVALO_LECTURA_MS) {
        previousMillis = currentMillis;

        int lecturas[8];
        int errorGeneral = barra.leerBarraConDiagnostico(lecturas);

        telemetria.publicarSensores(lecturas, errorGeneral);
    }
}
