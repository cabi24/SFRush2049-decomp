cd ~/rush2049/scratch/frontier/w15g
try(){ f=$1; t=$2; for v in L K J I H; do for o in -O2 -O1; do r=$(python3 sc.py ul/src/$f $t --ver $v --flags "-g0 $o -mips2 -G 0 -non_shared" 2>&1 | tail -1 | cut -c1-110); echo "$t $f $v $o: $r"; done; done; }
try io/contramwrite.c __osContRamWrite
try io/epirawwrite.c osEPiRawWriteIo
try io/epirawread.c osEPiRawReadIo
try io/epirawdma.c osEPiRawStartDma
try io/pigetcmdq.c __osPiGetCmdQueue
try io/aisetnextbuf.c osAiSetNextBuffer
try os/yieldthread.c osYieldThread
try io/pi.c __osPiDeviceBusy
