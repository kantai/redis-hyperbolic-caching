
strategy=$1

if [[ "$strategy" == "" ]];
then
strategy="hyper"
fi

size=$2

if [[ "$strategy" == "hyper" ]];
then
REDIS_CFLAGS="-D PRIORITY_FUNCTION=1 -D TRACKING_LFU=1"
else
REDIS_CFLAGS="-D PRIORITY_FUNCTION=0"
fi

if [[ "$size" == "size-aware" ]];
then
REDIS_CFLAGS="$REDIS_CFLAGS -D PRIORITY_INCORPORATE_SIZE=1"
fi


make clean ; make REDIS_CFLAGS="$REDIS_CFLAGS"
