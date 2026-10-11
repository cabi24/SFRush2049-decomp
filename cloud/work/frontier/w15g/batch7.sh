cd ~/rush2049/scratch/frontier/w15g
t(){ f=$1; n=$2; o=$3; shift 3; echo "$n <- $f $o $*: $(python3 sc.py ul/src/$f $n --ver L --flags "-g0 $o -mips2 -G 0 -non_shared" "$@" 2>&1|tail -1|cut -c1-60)"; }
t io/epirawread.c osEPiRawReadIo -O2 -D__osCurrentHandle=__osPiDevList
t io/epirawwrite.c osEPiRawStartDma -O2 -D__osCurrentHandle=__osPiDevList
t os/yieldthread.c osYieldThread -O1 -D__osRunQueue=__osActiveQueue -D__osEnqueueAndYield=__osCleanupThread
t os/resetglobalintmask.c osEPiRawWriteIo -O1
t os/setglobalintmask.c __osPiGetCmdQueue -O1
