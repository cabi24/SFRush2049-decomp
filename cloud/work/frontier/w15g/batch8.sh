cd ~/rush2049/scratch/frontier/w15g
a(){ f=$1; fn=$2; tg=$3; python3 slice.py ul/src/$f $fn ul/src/os/s_$fn.s 2>/dev/null || { echo "$fn: slice fail"; return; }; for v in L; do echo "$tg <- $fn ($f) $v: $(python3 sc.py ul/src/os/s_$fn.s $tg --ver $v --flags "-g0 -O2 -mips3 -32 -G 0 -non_shared" 2>&1|tail -1|cut -c1-60)"; done; }
a os/interrupt.s __osDisableInt __osDisableInt
a os/interrupt.s __osRestoreInt __osRestoreInt
a os/exceptasm.s __osEnqueueAndYield __osEnqueueAndYield
a os/exceptasm.s __osEnqueueThread __osEnqueueThread
a os/exceptasm.s __osDispatchThread __osDispatchThread
a os/exceptasm.s __osCleanupThread __osCleanupThread
a os/exceptasm.s __osException __osExceptionPreamble
a os/exceptasm.s __ptException __osExceptionPanic
a os/exceptasm.s __ptExceptionPreamble __osException
a os/setintmask.s osSetIntMask osSetGlobalIntMask
a os/unmaptlball.s osUnmapTLBAll __osTlbFlush
a os/probetlb.s __osProbeTLB __osTLBLookup
