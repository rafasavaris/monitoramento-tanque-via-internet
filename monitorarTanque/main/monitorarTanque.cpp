#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sensor/sensorDistancia.h"
#include "tanque/tanque.h"
#include "mqtt/mqtt.h"
#include "wifi/wifi.h"
#include "delay/delay.h"

#define GPIO_TRIG 23
#define GPIO_ECHO 22

#define BROKER "mqtt://broker.emqx.io:1883"
#define TOPICO_COMANDOS "grupo_rm"
#define TOPICO_TANQUE "grupo_rm"

SensorDistancia* meuSensor;
Tanque* meuTanque;
MQTT* meuMqtt;

void minhaFuncao(const char* mensagem)
{
    char buffer[100];

    if (strcmp(mensagem, "leia distancia") == 0) {
        float distancia = meuSensor->getDist();

        snprintf(buffer, sizeof(buffer), "%.2f", distancia);
        meuMqtt->pub(TOPICO_TANQUE, buffer);
    }

    else if (strcmp(mensagem, "leia altura") == 0) {
        float distancia = meuSensor->getDist();
        float altura = meuTanque->getAlturaColuna(distancia);

        if (altura < 0) {
            printf("Erro na leitura do sensor!\n");
            return;
        }

        snprintf(buffer, sizeof(buffer), "%.2f", altura);
        meuMqtt->pub(TOPICO_TANQUE, buffer);
    }

    else if (strcmp(mensagem, "leia litros") == 0) {
        float distancia = meuSensor->getDist();
        float litros = meuTanque->getLitros(distancia);

        if (litros < 0) {
            printf("Erro no calculo do volume!\n");
            return;
        }

        snprintf(buffer, sizeof(buffer), "%.2f", litros);
        meuMqtt->pub(TOPICO_TANQUE, buffer);
    }

    else {
        printf("Comando desconhecido: %s\n", mensagem);
    }
}


extern "C" void app_main()
{
    printf("1 - Inicializando sensor\n");

    SensorDistancia sensor(GPIO_TRIG, GPIO_ECHO);
    meuSensor = &sensor;

    printf("2 - Criando WiFi\n");
    WIFI meuWifi("teste_planta_piloto", "12701@LPA");
    printf("3 - Iniciando WiFi\n");
    meuWifi.iniciar();
    printf("4 - WiFi conectado!\n");

    printf("5 - Criando tanque\n");
    Tanque tanque(9.0f, 113.1f);
    meuTanque = &tanque;

    printf("6 - Inicializando MQTT\n");
    MQTT mqtt(BROKER);
    meuMqtt = &mqtt;

    mqtt.subHandler(minhaFuncao);
    mqtt.sub(TOPICO_COMANDOS);
    printf("7 - MQTT conectado!\n");

    while (1) {
        delay_ms(500);
    }
}