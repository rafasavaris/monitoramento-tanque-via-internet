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


extern "C" void app_main() ;


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