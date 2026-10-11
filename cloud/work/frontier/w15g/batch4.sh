cd ~/rush2049/scratch/frontier/w15g
for p in "libc/bcmp.s bcmp" "libc/bcopy.s bcopy" "libc/bzero.s bzero" "os/invalicache.s osInvalICache" "os/invaldcache.s osInvalDCache" "os/writebackdcache.s osWritebackDCache" "os/writebackdcacheall.s osWritebackDCacheAll" "os/getsr.s __osGetSR" "os/setsr.s __osSetSR" "os/setfpccsr.s __osSetFpcCsr" "os/getcause.s __osGetCause" "os/setcompare.s __osSetCompare" "os/getcount.s osGetCount" "gu/sqrtf.s sqrtf" "os/interrupt.s __osDisableInt"; do set -- $p
 for v in L K; do for fl in "-mips2" "-mips3 -32"; do
 echo "$2 $v [$fl]: $(python3 sc.py ul/src/$1 $2 --ver $v --flags "-g0 $fl -G 0 -non_shared" 2>&1 | tail -1 | cut -c1-110)"; done; done; done
