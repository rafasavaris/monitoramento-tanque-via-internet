#include "sensorDistancia.h"
#include "delay.h"

SR04::SR04(gpio_num_t trigger, gpio_num_t echo)
{
    pino_TRIGGER = trigger;
    pino_ECHO    = echo;

    digital.pinMode (pino_TRIGGER, OUTPUT);
    digital.pinMode (pino_ECHO, INPUT);
    digital.digitalWrite(pino_TRIGGER,LOW);
    tempo = getTime();
    delay_ms(1000);
}

int64_t SR04::getDist(void)
{
    int64_t t1, t2,tmp;

    digital.digitalWrite(pino_TRIGGER, HIGH);
    delay_us(10);
    digital.digitalWrite(pino_TRIGGER, LOW);

    while (digital.digitalRead(pino_ECHO)==LOW);
    t1 = tempo.get();
    while (digital.digitalRead(pino_ECHO)==HIGH);
    t2 = tempo.get();
    tmp = t2 - t1;
    delay_ms(80);

    return (tmp / 58);
}