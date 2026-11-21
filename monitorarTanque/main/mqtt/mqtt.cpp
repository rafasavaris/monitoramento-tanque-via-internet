#include <stdio.h>
#include <string.h>

#include "mqtt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define MQTT_CONNECTED_BIT BIT0

static EventGroupHandle_t mqtt_event_group;


MQTT::MQTT(const char* broker, SensorDistancia* sensor,
           float alturaTanque, float areaTanque)
{
    this->broker = broker;
    this->sensor = sensor;
    this->alturaTanque = alturaTanque;
    this->areaTanque = areaTanque;

    this->topico = NULL;
    this->client = NULL;

    mqtt_event_group = xEventGroupCreate();
}

void MQTT::mqtt_event_handler(void* handler_args, esp_event_base_t base,
                              int32_t event_id, void* event_data)
{
    MQTT* mqtt = (MQTT*) handler_args;
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT conectado!\n");
            xEventGroupSetBits(mqtt_event_group, MQTT_CONNECTED_BIT);
            break;

        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT desconectado!\n");
            xEventGroupClearBits(mqtt_event_group, MQTT_CONNECTED_BIT);
            break;

        case MQTT_EVENT_DATA:
            printf("Mensagem recebida:\n");
            printf("Topico: %.*s\n", event->topic_len, event->topic);
            printf("Dados: %.*s\n", event->data_len, event->data);

            char dados[200];

            if (event->data_len < sizeof(dados)) {
                memcpy(dados, event->data, event->data_len);
                dados[event->data_len] = '\0';
                mqtt->tratarMensagem(dados);
            }
            break;

        default:
            printf("Evento MQTT id: %d\n", event->event_id);
            break;
    }
}

void MQTT::tratarMensagem(const char* mensagem)
{
    char buffer[100];

    if (strcmp(mensagem, "leia distancia") == 0) {
        float distancia = sensor->getDist();
        sprintf(buffer, "Distancia: %.2f cm", distancia);
        pub("grupo1_tanque", buffer);
    }

    else if (strcmp(mensagem, "leia altura") == 0) {
        float distancia = sensor->getDist();
        float altura = alturaTanque - distancia;

        if (altura < 0)
            altura = 0;

        sprintf(buffer, "Altura: %.2f cm", altura);
        pub("grupo1_tanque", buffer);

    }

    else if (strcmp(mensagem, "leia litros") == 0) {
        float distancia = sensor->getDist();
        float altura = alturaTanque - distancia;

        if (altura < 0)
            altura = 0;

        float litros = (areaTanque * altura) / 1000.0;

        sprintf(buffer, "Quantidade: %.2f litros", litros);
        pub("grupo1_tanque", buffer);

    }

    else {
        printf("Comando desconhecido: %s\n", mensagem);
    }
}

void MQTT::sub(const char* topico)
{
    this->topico = topico;
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = broker;
    client = esp_mqtt_client_init(&mqtt_cfg);

    ESP_ERROR_CHECK(esp_mqtt_client_register_event(
        client,
        MQTT_EVENT_ANY,
        mqtt_event_handler,
        this
    ));

    ESP_ERROR_CHECK(esp_mqtt_client_start(client));

    printf("Aguardando conexao MQTT...\n");

    xEventGroupWaitBits(
        mqtt_event_group,
        MQTT_CONNECTED_BIT,
        pdFALSE,
        pdTRUE,
        portMAX_DELAY
    );

    ESP_ERROR_CHECK(esp_mqtt_client_subscribe(client, topico, 0));
    printf("Inscrito no topico: %s\n", topico);
}

void MQTT::subHandler()
{
    printf("Tratamento de mensagens MQTT configurado!\n");
}

void MQTT::pub(const char* topico, const char* mensagem)
{
    if (client == NULL)
        return;

    if (!(xEventGroupGetBits(mqtt_event_group) & MQTT_CONNECTED_BIT)) {
        printf("MQTT desconectado!\n");
        return;
    }

    esp_mqtt_client_publish(client, topico, mensagem, 0, 1, 0);
    printf("Mensagem publicada: %s\n", mensagem);
}