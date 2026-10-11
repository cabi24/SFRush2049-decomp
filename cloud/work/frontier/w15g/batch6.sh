cd ~/rush2049/scratch/frontier/w15g
t(){ for o in -O2 -O1; do echo "$2 <- $1 $o: $(python3 sc.py ul/src/$1 $2 --ver L --flags "-g0 $o -mips2 -G 0 -non_shared" 2>&1|tail -1|cut -c1-60)"; done; }
t os/resetglobalintmask.c osEPiRawWriteIo
t io/epirawwrite.c osEPiRawStartDma
t io/epirawread.c osEPiRawReadIo
t os/setglobalintmask.c __osPiGetCmdQueue
t io/pirawdma.c osEPiRawStartDma
