#include "redis.h"
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>

long double getPriorityLFU(robj *o){
#ifdef TRACKING_LFU
    return getLFUCount(o) * (long double) getObjectCost(o);
#else
    assert(-1);
#endif
}

long double getPriorityDEGFREQ(robj *o){
    long double numer, pr;
    unsigned long long denom;
    numer = getPriorityLFU(o);
    
    denom = estimateObjectIdleTime(o) / REDIS_LRU_CLOCK_RESOLUTION;
    if (denom == 0) { 
	return LDBL_MAX;
    }else{
	pr = numer / ((long double) denom);
	return pr;
    }
}



long double getObjectPriority(robj *o){
    switch(PRIORITY_FUNCTION){
    case PRIORITY_FUNC_LFU: 
	return getPriorityLFU(o);
    case PRIORITY_FUNC_DEGRADE_F: 
	return getPriorityDEGFREQ(o);
    }
}

