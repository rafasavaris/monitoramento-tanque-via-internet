#include <stdio.h>
#include <string.h>

#include "mqtt.h"

#define MQTT_CONNECTED_BIT BIT0

MQTT::MQTT(const char* broker)
{
    if (strstr(broker, "://") == NULL) {
        snprintf(
            this->broker,
            sizeof(this->broker),
            "mqtt://%s",
            broker
        );
    } else {
        snprintf(
            this->broker,
            sizeof(this->broker),
            "%s",
            broker
        );
    }

    client = NULL;
    funcaoHandler = NULL;

    mqtt_event_group = xEventGroupCreate();

    configASSERT(mqtt_event_group != NULL);
}

void MQTT::mqtt_event_handler(
    void* handler_args,
    esp_event_base_t base,
    int32_t event_id,
    void* event_data
)
{
    MQTT* mqtt = (MQTT*)handler_args;

    esp_mqtt_event_handle_t event =
        (esp_mqtt_event_handle_t)event_data;

    switch (event->event_id) {

        case MQTT_EVENT_CONNECTED:

            printf("MQTT conectado!\n");

            xEventGroupSetBits(
                mqtt->mqtt_event_group,
                MQTT_CONNECTED_BIT
            );

            break;

        case MQTT_EVENT_DISCONNECTED:

            printf("MQTT desconectado!\n");

            xEventGroupClearBits(
                mqtt->mqtt_event_group,
                MQTT_CONNECTED_BIT
            );

            break;

        case MQTT_EVENT_DATA: {

            printf("Mensagem recebida!\n");

            printf(
                "Topico: %.*s\n",
                event->topic_len,
                event->topic
            );

            printf(
                "Dados: %.*s\n",
                event->data_len,
                event->data
            );

            char dados[200];

            if (
                event->current_data_offset == 0 &&
                event->data_len == event->total_data_len &&
                event->data_len < sizeof(dados)
            ) {

                memcpy(
                    dados,
                    event->data,
                    event->data_len
                );

                dados[event->data_len] = '\0';

                if (mqtt->funcaoHandler != NULL) {
                    mqtt->funcaoHandler(dados);
                }
            }

            break;
        }

        default:

            printf(
                "Evento MQTT id: %ld\n",
                (long)event->event_id
            );

            break;
    }
}

void MQTT::sub(const char* topico)
{
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = broker;
    client = esp_mqtt_client_init(&mqtt_cfg);

    configASSERT(client != NULL);

    ESP_ERROR_CHECK(
        esp_mqtt_client_register_event(
            client,
            MQTT_EVENT_ANY,
            mqtt_event_handler,
            this
        )
    );

    ESP_ERROR_CHECK(
        esp_mqtt_client_start(client)
    );

    printf("Aguardando conexao MQTT...\n");

    xEventGroupWaitBits(
        mqtt_event_group,
        MQTT_CONNECTED_BIT,
        pdFALSE,
        pdTRUE,
        portMAX_DELAY
    );

    printf("Conexao MQTT estabelecida!\n");

    int msg_id = esp_mqtt_client_subscribe(
        client,
        topico,
        0
    );

    if (msg_id < 0) {
        printf("Erro ao se inscrever no topico!\n");
        return;
    }

    printf("Inscrito no topico: %s\n", topico);
}

void MQTT::subHandler(void (*funcao)(const char*))
{
    funcaoHandler = funcao;

    printf("Funcao de tratamento MQTT configurada!\n");
}

void MQTT::pub(const char* topico, const char* mensagem)
{
    if (client == NULL) {
        printf("Cliente MQTT nao inicializado!\n");
        return;
    }

    if (
        !(xEventGroupGetBits(mqtt_event_group) &
        MQTT_CONNECTED_BIT)
    ) {
        printf("MQTT desconectado!\n");
        return;
    }

    int msg_id = esp_mqtt_client_publish(
        client,
        topico,
        mensagem,
        0,
        1,
        0
    );

    if (msg_id < 0) {
        printf("Erro ao publicar mensagem!\n");
        return;
    }

    printf("Mensagem publicada: %s\n", mensagem);
}