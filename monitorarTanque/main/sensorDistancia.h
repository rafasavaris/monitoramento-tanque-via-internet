#ifndef __SR04__
#define __SR04__
#include <inttypes.h> 
#include "digital.h"
#include "getTime.h"

class SR04 {
    private:
        gpio_num_t pino_TRIGGER;
        gpio_num_t pino_ECHO;
        getTime tempo;
         
    public:
        SR04(gpio_num_t trigger, gpio_num_t echo);
        int64_t getDist(void);

};

#endif