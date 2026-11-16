#include <stdlib.h>
#include "getTime.h"

int64_t getTime::get (void)
{
	return esp_timer_get_time();
}

int64_t getTime::getDiff (int64_t t2, int64_t t1)
{
	return (t2-t1); 
}