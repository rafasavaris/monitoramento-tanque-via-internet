idf_component_register(
    SRCS
        "monitorarTanque.cpp"
        "sensor/sensorDistancia.cpp"
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