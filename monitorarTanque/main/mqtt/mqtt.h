#ifndef MQTT_H
#define MQTT_H

#include "mqtt_client.h"
#include "sensorDistancia.h"

class MQTT {
private:
    const char* broker;
    const char* topico;
    esp_mqtt_client_handle_t client;

    SensorDistancia* sensor;

    float alturaTanque;
    float areaTanque;

    static void mqtt_event_handler(void* handler_args, esp_event_base_t base,
                                   int32_t event_id, void* event_data);
    void tratarMensagem(const char* mensagem);

public:
    MQTT(const char* broker, SensorDistancia* sensor, float alturaTanque, float areaTanque);
    void sub(const char* topico);
    void subHandler();
    void pub(const char* topico, const char* mensagem);
};

#endif