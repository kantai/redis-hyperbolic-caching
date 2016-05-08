#include "redis.h"
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <assert.h>

long double getPriority_LFU(robj *o){
#ifdef TRACKING_LFU
    return getLFUCount(o) * (long double) getObjectCost(o);
#else
    assert(0);
#endif
}

long double getPriority_Degrade_F(robj *o){
    long double numer;
    unsigned long long denom;

    numer = getPriority_LFU(o);    
    denom = estimateObjectIdleTime(o) / REDIS_LRU_CLOCK_RESOLUTION;
    if (denom == 0) { 
	return LDBL_MAX;
    }else{
	return numer / ((long double) denom);
    }
}

long double getPriority_LFRU(robj *o){
    long double count, scale_counts;
    unsigned long long time_delta;
    
    count = getPriority_LFU(o);
    time_delta = estimateObjectIdleTime(o) / REDIS_LRU_CLOCK_RESOLUTION;

    scale_counts = powl(LFU_DEGRADE, (long double) time_delta);

    return count * scale_counts;
}

long double getPriority_GD(robj *o){
    // again, I'm storing the GD priority value in the lfucount
    return getLFUCount(o);
}

long double getPriority_Default(robj *o){
    unsigned long long denom = getLFUCount(o);
    long double numer = 1.0;
    if (denom == 0) { 
	return LDBL_MAX;
    }else{
	return numer / ((long double) denom);
    }    
}

long double getObjectPriority(robj *o){
    switch(PRIORITY_FUNCTION){
    case PRIORITY_FUNC_LFU: 
	return getPriority_LFU(o);
    case PRIORITY_FUNC_DEGRADE_F: 
	return getPriority_Degrade_F(o);
    case PRIORITY_FUNC_LFRU:
	return getPriority_LFRU(o);
    case PRIORITY_FUNC_GD:
	return getPriority_GD(o);
    case PRIORITY_FUNC_DEFAULT:
	return getPriority_Default(o);
    }
}

