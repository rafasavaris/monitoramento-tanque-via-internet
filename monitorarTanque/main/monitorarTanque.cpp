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

char url_broker = "grupo_rm"

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
  int64_t distancia;
  SensorDistancia meuSensor = SensorDistancia(PIN16, PIN5);
  
  delay_ms(2000);
  printf("\n\nRODANDO\n\n");
  
  while (1)
  {
  	distancia = meuSensor.getDist();
  	printf("Distancia vale %ld centimetros\n",(long)distancia);
  }  
}