#ifndef __DELAY__
#define __DELAY__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"

#define delay_us(A) esp_rom_delay_us(A)
#define delay_ms(A) vTaskDelay(pdMS_TO_TICKS(A))

#endif