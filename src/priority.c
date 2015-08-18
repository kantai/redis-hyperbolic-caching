#include "redis.h"
#include <math.h>
#include <ctype.h>
#include <limits.h>

unsigned long long getObjectPriority(robj *o){
  unsigned long long idletime = estimateObjectIdleTime(o);
  unsigned long long cost = getObjectCost(o);
  switch(PRIORITY_TRACKING){
  case TRACKING_LFU: return idletime * cost;
  case TRACKING_LRU: return ULLONG_MAX - (idletime / cost);
  }
}
