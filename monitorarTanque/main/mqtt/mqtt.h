#ifndef MQTT_H
#define MQTT_H

#include "mqtt_client.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

class MQTT {
private:
    char broker[128];

    esp_mqtt_client_handle_t client;
    EventGroupHandle_t mqtt_event_group;

    void (*funcaoHandler)(const char*);

    static void mqtt_event_handler(
        void* handler_args,
        esp_event_base_t base,
        int32_t event_id,
        void* event_data
    );

public:
    MQTT(const char* broker);
    void sub(const char* topico);
    void subHandler(void (*funcao)(const char*));
    void pub(const char* topico, const char* mensagem);
};

#endif