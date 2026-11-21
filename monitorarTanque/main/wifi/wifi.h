#ifndef WIFI_H
#define WIFI_H

#include "esp_event.h"

class WIFI {
private:
    const char* ssid;
    const char* password;

    static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                   int32_t event_id, void* event_data);

public:
    WIFI(const char* ssid, const char* password);

    void iniciar();
};

#endif