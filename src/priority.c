#include "redis.h"
#include <math.h>
#include <ctype.h>
#include <limits.h>

unsigned long long getObjectPriority(robj *o){
  unsigned long long idletime = estimateObjectIdleTime(o);
  unsigned long long inverse_p = idletime / getObjectCost(o);
  
  return inverse_p; // reverse priority ordering, to ensure that bigger is better!
}
