#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <inttypes.h> 

#include "sensorDistancia.h"
#include "delay.h"
#include "digital.h"


extern "C" void app_main() ;


void app_main()
{
  int64_t distancia;
  
  delay_ms(2000);
  printf("\n\nRODANDO\n\n");

  SR04 ultrasom = SR04(PIN16, PIN5);
  
  while (1)
  {
  	distancia = ultrasom.getDist();
  	printf("Distancia vale %ld centimetros\n",(long)distancia);
  }  
}