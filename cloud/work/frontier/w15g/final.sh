cd ~/rush2049/scratch/frontier/w15g
O2="-g0 -O2 -mips2 -G 0 -non_shared"; O1="-g0 -O1 -mips2 -G 0 -non_shared"
sed "s/__osPfsGetStatus/osContStartReadData/;s/\b__osPfsLastChannel\b/__osContLastChannel/g;s/\b__osContLastCmd\b/__osPfsRequestType/g;s/\b__osPfsPifRam\b/__osPfsBuffer/g" ul/src/io/contramwrite.c > ul/src/io/crw.c
sed -i "/^extern s32 __osContLastChannel/a extern OSPifRam __osPfsBuffer; extern u8 __osPfsRequestType; s32 osContStartReadData(OSMesgQueue*, int);" ul/src/io/crw.c
echo "__osContRamWrite: $(python3 sc.py ul/src/io/crw.c __osContRamWrite --ver L --flags "$O2"|tail -1|cut -c1-70)"
python3 cslice.py ul/src/io/contpfs.c __osPfsRWInode ul/src/io/rw.c
sed -i "s/\b__osPfsInodeCacheBank\b/__osSiChannelMask/g;s/\b__osPfsInodeCacheChannel\b/__osSiLastChannel/g;s/\b__osPfsInodeCache\b/__osContPifInode/g" ul/src/io/rw.c
echo "__osPfsRWInode: $(python3 sc.py ul/src/io/rw.c __osPfsRWInode --ver L --flags "$O2"|tail -1|cut -c1-70)"
echo "osYieldThread: $(python3 sc.py ul/src/os/yieldthread.c osYieldThread --ver L --flags "$O1" -D__osRunQueue=__osActiveQueue|tail -1|cut -c1-70)"
echo "osEPiRawWriteIo(reset mask): $(python3 sc.py ul/src/os/resetglobalintmask.c osEPiRawWriteIo --ver L --flags "$O1" -D__OSGlobalIntMask=__osGlobalIntMask|tail -1|cut -c1-70)"
echo "__osPiGetCmdQueue(set mask): $(python3 sc.py ul/src/os/setglobalintmask.c __osPiGetCmdQueue --ver L --flags "$O1" -D__OSGlobalIntMask=__osGlobalIntMask|tail -1|cut -c1-70)"
echo "osMotorStop(repairid): $(python3 sc.py ul/src/io/pfsrepairid.c osMotorStop --ver L --flags "$O2"|tail -1|cut -c1-70)"
echo "osAiSetNextBuffer: $(python3 sc.py ul/src/io/aisetnextbuf.c osAiSetNextBuffer --ver L --flags "$O2"|tail -1|cut -c1-70)"
echo "osEPiRawReadIo: $(python3 sc.py ul/src/io/epirawread.c osEPiRawReadIo --ver L --flags "$O2" -D__osCurrentHandle=__osPiDevList|tail -1|cut -c1-70)"
echo "osEPiRawStartDma(rawwrite): $(python3 sc.py ul/src/io/epirawwrite.c osEPiRawStartDma --ver L --flags "$O2" -D__osCurrentHandle=__osPiDevList|tail -1|cut -c1-70)"
python3 cslice.py ul/src/io/si.c __osSiDeviceBusy ul/src/io/sib.c 2>/dev/null
echo "__osPiDeviceBusy(SiDeviceBusy): $(python3 sc.py ul/src/io/sib.c __osPiDeviceBusy --ver L --flags "$O2"|tail -1|cut -c1-70)"
