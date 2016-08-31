#include "redis.h"
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <assert.h>

long double getPriority_LFU(robj *o){
    return getLFUCount(o);
}

long double getPriority_Hyper(robj *o, sds keystr){
    long double numer;
    unsigned long long denom;

    long double cost = getObjectCost(o);
#ifdef PRIORITY_INCORPORATE_SIZE
    long double byte_size = (rdbSavedObjectLen(o) + sdslen(keystr) + 
	sizeof(robj));
    cost /= byte_size;
#else
    (void)(keystr); // blanks : stupid way to get around UNUSED warns.
#endif

    numer = getLFUCount(o); 
    numer *= cost;

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
    return o->lru;
}

long double getObjectPriority(robj *o, sds key){
    switch(PRIORITY_FUNCTION){
    case PRIORITY_FUNC_LFU: 
	return getPriority_LFU(o);
    case PRIORITY_FUNC_HYPER: 
	return getPriority_Hyper(o, key);
    case PRIORITY_FUNC_LFRU:
	return getPriority_LFRU(o);
    case PRIORITY_FUNC_GD:
	return getPriority_GD(o);
    case PRIORITY_FUNC_DEFAULT:
	return getPriority_Default(o);
    }
}

