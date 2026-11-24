#include "sensorDistancia.h"
#include <stdio.h>
#include "./delay/delay.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LOW  0
#define HIGH 1

SensorDistancia::SensorDistancia(int trig, int echo)
{
    gpioTrig = trig;
    gpioEcho = echo;

    gpio_evt_queue = xQueueCreate(1, sizeof(int64_t));

    if (gpio_evt_queue == NULL)
    {
        printf("ERRO: nao foi possivel criar a fila\n");
        return;
    }
    configurar();
}


void SensorDistancia::configurar()
{
    gpio_install_isr_service(0);
    gpio_reset_pin((gpio_num_t)gpioTrig);
    gpio_set_direction((gpio_num_t)gpioTrig, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)gpioTrig, LOW);

    gpio_config_t gpioConfig = {
        .pin_bit_mask = (1ULL << gpioEcho),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    gpio_config(&gpioConfig);

    gpio_isr_handler_add(
        (gpio_num_t)gpioEcho,
        tratador_int_externa,
        this
    );
}

void IRAM_ATTR SensorDistancia::tratador_int_externa(void *args)
{
    SensorDistancia *sensor = (SensorDistancia *)args;
    static int64_t inicio = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int nivel = gpio_get_level((gpio_num_t)sensor->gpioEcho);

    if (nivel == HIGH)
    {
        inicio = getTime();
    }
    else
    {
        int64_t fim = getTime();

        int64_t diferenca_tempo = fim - inicio;

        xQueueSendFromISR(
            sensor->gpio_evt_queue,
            &diferenca_tempo,
            &xHigherPriorityTaskWoken
        );
    }

    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }
}

float SensorDistancia::getDist()
{
    int64_t tempo;

    if (gpio_evt_queue == NULL)
    {
        return 0.0f;
    }

    gpio_set_level((gpio_num_t)gpioTrig, HIGH);
    delay_us(10);
    gpio_set_level((gpio_num_t)gpioTrig, LOW);

    if (xQueueReceive(
            gpio_evt_queue,
            &tempo,
            pdMS_TO_TICKS(100))
        == pdPASS)
    {
        return (float)tempo / 58.0f;
    }

    return 0.0f;
}