idf_component_register(
    SRCS
        "time/getTime.cpp"
        "monitorarTanque.cpp"
        "sensor/sensorDistancia.cpp"
        "digital/digital.cpp"
        "tanque/tanque.cpp"
        "wifi/wifi.cpp"
        "mqtt/mqtt.cpp"

    INCLUDE_DIRS
        "."

    REQUIRES
        mqtt
        esp_driver_gpio
        esp_wifi
        nvs_flash
)