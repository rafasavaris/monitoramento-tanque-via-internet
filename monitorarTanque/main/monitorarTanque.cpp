#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <inttypes.h> 

#include "sensor/sensorDistancia.h"
#include "delay/delay.h"
#include "digital/digital.h"
#include "tanque/tanque.h"
#include "mqtt/mqtt.h"
#include "wifi/wifi.h"
#include "time/getTime.h"

extern "C" void app_main() ;
#define GPIO_TRIG 23
#define GPIO_ECHO 22
char url_broker[9] = "grupo_rm";

/*
void minhaFuncao(const char* mensagem)
{
    char buffer[100];

    if (strcmp(mensagem, "leia altura") == 0) {
        float distancia = meuSensor->getDist();
        float altura = meuTanque->getAlturaColuna(distancia);

        if (altura < 0) {
            printf("Erro na leitura do sensor!\n");
            return;
        }

        snprintf(buffer, sizeof(buffer), "%.2f", altura);
        meuMqtt->pub(grupo_rm, buffer);
    }

    else if (strcmp(mensagem, "leia litros") == 0) {
        float distancia = meuSensor->getDist();
        float litros = meuTanque->getLitros(distancia);

        if (litros < 0) {
            printf("Erro no calculo do volume!\n");
            return;
        }

        snprintf(buffer, sizeof(buffer), "%.2f", litros);
        meuMqtt->pub(grupo_rm, buffer);
    }

    else {
        printf("Comando desconhecido: %s\n", mensagem);
    }
}
    */

void app_main()
{
  SensorDistancia sensor(GPIO_TRIG, GPIO_ECHO);

    printf("INICIO\n");

    while (1)
    {
        printf("Mede a distancia\n");
        float d = sensor.getDist();
        printf("Distancia = %.2f cm\n", d);
        delay_ms(1000);
    }
}