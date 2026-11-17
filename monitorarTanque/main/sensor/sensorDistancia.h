#ifndef __sensorDistancia__
#define __sensorDistancia__
#include <inttypes.h> 
#include "../digital/digital.h"
#include "../time/getTime.h"

class SensorDistancia {
    private:
        gpio_num_t pino_TRIGGER;
        gpio_num_t pino_ECHO;
        getTime tempo;
         
    public:
        SensorDistancia(gpio_num_t trigger, gpio_num_t echo);
        int64_t getDist(void);
};

#endif