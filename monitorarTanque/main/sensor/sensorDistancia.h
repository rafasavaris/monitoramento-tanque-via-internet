#ifndef __sensorDistancia__
#define __sensorDistancia__
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class SensorDistancia {
    private:
        int gpioTrig;
        int gpioEcho;
        QueueHandle_t gpio_evt_queue;
        static void IRAM_ATTR tratador_int_externa(void *args);

    void configurar();
         
    public:
        SensorDistancia(int trig, int echo);
        float getDist();
};

#endif