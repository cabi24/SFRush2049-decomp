# reloc_only_diff reconciliation evidence (generated)

For each target: OUR symbol names (from the reloc-aware target .o) + the ultralib
source. Reconcile = rewrite the ultralib source's external symbol names to OURS.

## __osSiGetAccess   @0x8000e5b0  tier=reloc_aware
  ultralib source: work/libultra/si/__osSiGetAccess/corpus_match.c
  OUR reloc symbols (target must link against these): __osSiInitialized, osSiInit, __osSiMesg, osRecvMesg

## __osSiRelAccess   @0x8000e5f4  tier=reloc_aware
  ultralib source: work/libultra/si/__osSiRelAccess/corpus_match.c
  OUR reloc symbols (target must link against these): __osSiMesg, osJamMesg

## guLookAt   @0x80009168  tier=raw_word
  ultralib source: work/libultra/gu/guLookAt/corpus_match.c
  OUR reloc symbols (target must link against these): (none — pure internal)

## guOrtho   @0x80008bb4  tier=raw_word
  ultralib source: work/libultra/gu/guOrtho/corpus_match.c
  OUR reloc symbols (target must link against these): (none — pure internal)

## guPerspective   @0x80008e50  tier=raw_word
  ultralib source: work/libultra/gu/guPerspective/corpus_match.c
  OUR reloc symbols (target must link against these): (none — pure internal)

## osContGetQuery   @0x800097ac  tier=reloc_aware
  ultralib source: work/libultra/cont/osContGetQuery/corpus_match.c
  OUR reloc symbols (target must link against these): __osContGetStatus

## osContGetReadData   @0x80009854  tier=reloc_aware
  ultralib source: work/libultra/cont/osContGetReadData/corpus_match.c
  OUR reloc symbols (target must link against these): __osPfsRequestType2, __osSiDmaBuffer

## osContStartQuery   @0x80009730  tier=reloc_aware
  ultralib source: work/libultra/cont/osContStartQuery/corpus_match.c
  OUR reloc symbols (target must link against these): __osSiGetAccess, __osPfsRequestType, __osContRamReset, __osSiDmaBuffer, __osSiRawStartDma, osRecvMesg, __osSiRelAccess

## osCreateMesgQueue   @0x80006a00  tier=reloc_aware
  ultralib source: work/libultra/os/osCreateMesgQueue/corpus_match.c
  OUR reloc symbols (target must link against these): __osEmptyMesgQueue

## osCreateThread   @0x80006f30  tier=reloc_aware
  ultralib source: work/libultra/thread/osCreateThread/corpus_match.c
  OUR reloc symbols (target must link against these): __osExceptionPanic, __osDisableInt, __osRunQueue, __osRestoreInt

## osDpSetNextBuffer   @0x80007b00  tier=reloc_aware
  ultralib source: work/libultra/dp/osDpSetNextBuffer/corpus_match.c
  OUR reloc symbols (target must link against these): osDpIsBusy, osVirtualToPhysical

## osGetTime   @0x800073b0  tier=reloc_aware
  ultralib source: work/libultra/os/osGetTime/corpus_match.c
  OUR reloc symbols (target must link against these): __osDisableInt, osGetCount, gViLastCount, gViTimeAccumHi, gViTimeAccumLo, __osRestoreInt

## osRecvMesg   @0x80007270  tier=reloc_aware
  ultralib source: work/libultra/os/osRecvMesg/corpus_match.c
  OUR reloc symbols (target must link against these): __osDisableInt, __osRestoreInt, __osRunningThread, __osCleanupThread, __osPopThread, osStartThread

## osSetTimer   @0x8000e3d0  tier=reloc_aware
  ultralib source: work/libultra/timer/osSetTimer/corpus_match.c
  OUR reloc symbols (target must link against these): dll_insert, __osTimerList, dll_reschedule

## osSpTaskYielded   @0x80007730  tier=reloc_aware
  ultralib source: work/libultra/sp/osSpTaskYielded/corpus_match.c
  OUR reloc symbols (target must link against these): bzero_alt

## osStartThread   @0x80007080  tier=reloc_aware
  ultralib source: work/libultra/thread/osStartThread/corpus_match.c
  OUR reloc symbols (target must link against these): __osDisableInt, __osActiveQueue, __osEnqueueThread, __osPopThread, __osRunningThread, __osDispatchThread, __osCleanupThread, __osRestoreInt

## osViGetCurrentFramebuffer   @0x80007790  tier=reloc_aware
  ultralib source: work/libultra/vi/osViGetCurrentFramebuffer/corpus_match.c
  OUR reloc symbols (target must link against these): __osDisableInt, __osViModeInfo, __osRestoreInt

## osVirtualToPhysical   @0x8000d5c0  tier=reloc_aware
  ultralib source: work/libultra/vi/osVirtualToPhysical/corpus_match.c
  OUR reloc symbols (target must link against these): __osTLBLookup
