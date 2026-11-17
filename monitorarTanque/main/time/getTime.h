#ifndef __GETTIME__
#define __GETTIME__
#include <inttypes.h> 
#include "esp_timer.h"

class getTime {
	public:
		int64_t get ();
		int64_t getDiff(int64_t t1, int64_t t2);
};

#endif