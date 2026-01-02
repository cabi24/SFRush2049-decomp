# Complete Function Work Breakdown

## Overview

| Category | Count | Compiler Flags | Priority |
|----------|-------|----------------|----------|
| libc | 8 | -O2 | HIGH |
| libm | 10 | -O2 | HIGH |
| libgcc | 16 | N/A (asm) | LOW |
| libultra_os | 39 | -O1 | HIGH |
| libultra_vi | 13 | -O1 | MEDIUM |
| libultra_ai | 3 | -O1 | MEDIUM |
| libultra_pi | 13 | -O1 | MEDIUM |
| libultra_si | 4 | -O1 | MEDIUM |
| libultra_sp | 6 | -O1 | MEDIUM |
| libultra_dp | 4 | -O1 | MEDIUM |
| libultra_gu | 10 | -O2 | HIGH |
| libultra_pfs | 20 | -O1 | MEDIUM |
| libultra_cont | 14 | -O1 | MEDIUM |
| libultra_motor | 4 | -O1 | LOW |
| libultra_timer | 21 | -O1 | MEDIUM |
| libultra_thread | 12 | -O1 | MEDIUM |
| libultra_cache | 6 | -O1 | MEDIUM |
| libultra_tlb | 3 | N/A (asm) | LOW |
| inflate | 16 | -O2 | MEDIUM |
| dll | 6 | -O2 | MEDIUM |
| scheduler | 14 | -O1 | MEDIUM |
| boot_misc | ~80 | varies | LOW |
| game | 554 | -O2 | HIGH |

**Total: ~1360 functions**

---

## Status Legend

- `TODO` - Not started
- `WIP` - Work in progress (claimed by session)
- `MATCHED` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `STUB` - Placeholder implementation
- `ASM` - Must remain assembly (inline asm)

---

## LIBC (8 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80002730 | `memchr` |  |
| TODO | 0x80002790 | `memset` |  |
| TODO | 0x80007C00 | `strchr` | find char in string |
| MATCHED | 0x80007C40 | `strlen` | get string length |
| TODO | 0x80007C68 | `memcpy` | copy memory |
| TODO | 0x80008590 | `bzero` | optimized zero-fill |
| TODO | 0x8000D2A0 | `bzero_alt` | alternative bzero (zeroes memory) |
| TODO | 0x8000D2B0 | `bcopy` | copy memory with overlap handling (memmove) |

---

## LIBM (10 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x800028A0 | `modf` | split double into integer and fractional parts |
| TODO | 0x80002A64 | `modff` | float version of modf |
| TODO | 0x80002BF0 | `__isinf` |  |
| TODO | 0x80002C60 | `__isnan` |  |
| TODO | 0x80002CD0 | `fcvt` | convert float to string |
| TODO | 0x80003C3C | `__ecvt_internal` | float to string internal helper |
| TODO | 0x80004674 | `__round_helper` | rounding helper for float conversion |
| TODO | 0x80008730 | `sinf` | sine approximation using polynomial arcade:fmath.c |
| TODO | 0x800088F0 | `cosf` | cosine approximation using polynomial arcade:fmath |
| TODO | 0x8000E3C0 | `sqrtf` | square root (float) arcade:fmath.c:fsqrt() |

---

## LIBGCC (16 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x800099F0 | `__fixdfdi` | double to long long |
| TODO | 0x80009A0C | `__fixsfdi` | float to long long |
| TODO | 0x80009A28 | `__fixunsdfdi` | double to unsigned long long |
| TODO | 0x80009AC8 | `__fixunssfdi` | float to unsigned long long |
| TODO | 0x80009B64 | `__floatdidf` | long long to double |
| TODO | 0x80009B7C | `__floatdisf` | long long to float |
| TODO | 0x80009B94 | `__floatundidf` | unsigned long long to double |
| TODO | 0x80009BC8 | `__floatundisf` | unsigned long long to float |
| TODO | 0x8000D8F0 | `__lshrdi3` | logical right shift |
| TODO | 0x8000D91C | `__umoddi3` | unsigned modulo |
| TODO | 0x8000D958 | `__udivdi3` | unsigned divide |
| TODO | 0x8000D994 | `__ashldi3` | arithmetic left shift |
| TODO | 0x8000D9FC | `__divdi3` | signed divide |
| TODO | 0x8000DA58 | `__muldi3` | multiply |
| TODO | 0x8000DAE8 | `__moddi3` | signed modulo |
| TODO | 0x8000DB84 | `__ashrdi3` | arithmetic right shift |

---

## LIBULTRA_OS (39 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80000450 | `osCreateScheduler` | create OSSc-like scheduler struct, sets up msg que |
| TODO | 0x800005D4 | `osScAddClient` | link client into scheduler clientList |
| MATCHED | 0x80006A00 | `osCreateMesgQueue` |  |
| TODO | 0x80006A30 | `osSetEventMesg` | set event message |
| TODO | 0x80006DA0 | `osSetIntMask` | set interrupt mask |
| TODO | 0x80006E10 | `osSetEventMesgAlt` | alternative event message setter |
| TODO | 0x800071D0 | `osSetGlobalIntMask` | set global interrupt mask |
| TODO | 0x80007270 | `osRecvMesg` | receive message from queue |
| TODO | 0x800073B0 | `osGetTime` | get system time |
| TODO | 0x80007440 | `osSendMesg` | send message to queue |
| TODO | 0x800075E0 | `osJamMesg` | insert message at front of queue |
| TODO | 0x80007E80 | `__osInitialize_common` | OS init: installs exception vectors, sets up PI/AI |
| TODO | 0x80008210 | `osCreatePiManager` | create PI DMA manager thread and queues |
| TODO | 0x80008380 | `osCreateViManager` | create VI manager thread (handles retrace/prenmi) |
| TODO | 0x800098E0 | `__osPackReadData` | pack read-buttons command into PIF buffer |
| TODO | 0x80009C00 | `osPhysicalToVirtual` | convert physical address to KSEG0 virtual |
| TODO | 0x8000C4B0 | `__osDisableInt` | disable interrupts, return previous state |
| TODO | 0x8000C520 | `__osRestoreInt` | restore interrupt state |
| TODO | 0x8000C660 | `osGetActiveQueue` | get active thread queue |
| TODO | 0x8000C970 | `osGetCount` | read CP0 Count register |
| TODO | 0x8000C980 | `__osException` | CPU exception handler (handwritten asm) |
| TODO | 0x8000C990 | `__osExceptionPreamble` | exception preamble - saves context to __osExceptio |
| TODO | 0x8000CEDC | `__osSendInterrupt` | send interrupt message to event state table queue  |
| TODO | 0x8000D298 | `__osExceptionPanic` | exception panic - called after ERET falls through  |
| TODO | 0x8000D770 | `__osSetSR` | set CP0 Status register |
| TODO | 0x8000D780 | `__osGetSR` | get CP0 Status register |
| TODO | 0x8000D790 | `__osSetFpcCsr` | set FP control/status register |
| TODO | 0x8000DBB0 | `osCauseGet` | read CP0 Cause register |
| TODO | 0x8000DBB0 | `__osGetCause` | get CP0 Cause register |
| TODO | 0x8000EB00 | `__osSumcalc` | byte-sum checksum helper (used by PFS) |
| TODO | 0x8000EB74 | `__osIdCheckSum` | calculate ID checksum (Adler-like) |
| TODO | 0x8000EC70 | `__osRepairId` | repair/rewrite ID to 4 redundant pages |
| TODO | 0x8000EFC0 | `__osCheckId` | check ID consistency across 4 pages |
| TODO | 0x8000F124 | `__osGetId` | top-level ID retrieval during pak init |
| TODO | 0x8000FB90 | `__osSetCompare` | set CP0 Compare register for timer |
| TODO | 0x8000FBA0 | `__osEnqueueAndYield` | save thread context, enqueue to queue, yield to sc |
| TODO | 0x8000FDA0 | `osEPiRawWriteIo` | raw PI I/O write (no device manager, direct HW acc |
| TODO | 0x8000FE00 | `osEPiRawStartDma` | raw PI DMA start (no device manager, direct HW acc |
| TODO | 0x8000FF60 | `osEPiRawReadIo` | raw PI I/O read (no device manager, direct HW acce |

---

## LIBULTRA_VI (13 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80006BB0 | `vi_manager_main` | VI manager main loop (handles VI_RETRACE, PRENMI e |
| TODO | 0x80007590 | `osViSetMode` | set VI mode |
| TODO | 0x80007790 | `osViGetCurrentFramebuffer` | get current framebuffer |
| TODO | 0x800077D0 | `osViGetFramebuffer` | get configured framebuffer from context |
| TODO | 0x80007840 | `osViModeTableGet` | get VI mode from table |
| TODO | 0x8000794C | `osViModeNtscLan1` | get NTSC LAN1 VI mode |
| TODO | 0x80007AB4 | `osViModeNtscLpn1` | get NTSC LPN1 VI mode |
| TODO | 0x80007D20 | `osViSetSpecialFeatures` | set VI special features (gamma, dither, etc.) |
| TODO | 0x8000C540 | `osViInit` | video interface initialization |
| TODO | 0x8000C670 | `__osViSwapContext` | swap VI context (writes to VI hardware registers) |
| TODO | 0x8000D5C0 | `osVirtualToPhysical` | convert virtual address to physical |
| TODO | 0x80094C30 | `vi_config_offset` | writes VI offsets 0x18/0x1A |
| TODO | 0x800DE4DC | `vi_retrace_callback` | retrace video timing config |

---

## LIBULTRA_AI (3 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x8000BE70 | `osAiSetNextBuffer` | set AI DMA buffer |
| TODO | 0x8000BF00 | `osAiSetFrequency` | set AI sample rate |
| TODO | 0x8000FB60 | `__osAiDeviceBusy` | check if AI FIFO is full (returns 1 if busy) |

---

## LIBULTRA_PI (13 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80008128 | `__osPiReadDeviceType` | save PI BSD DOM1/DOM2 timing registers to globals |
| TODO | 0x800081D0 | `osPiRawReadWord` | synchronized ROM word read with PI access control |
| TODO | 0x80008630 | `__osPiRawStartDma` | PI raw DMA request via message queue |
| TODO | 0x8000D7A0 | `osPiReadIo` | read from physical I/O address |
| TODO | 0x8000D7F0 | `osPiWriteWord` | write word to cartridge space |
| TODO | 0x8000DBC0 | `osPiInit` | peripheral interface initialization |
| TODO | 0x8000DC10 | `osPiGetAccess` | acquire PI mutex |
| TODO | 0x8000DC54 | `osPiReleaseAccess` | release PI mutex |
| TODO | 0x8000DC80 | `osPiReadWord` | read word from cartridge |
| TODO | 0x8000DCD0 | `osPiStartDma` | start PI DMA transfer |
| TODO | 0x8000DDA0 | `osPiSetDeviceTiming` | configure PI bus timing registers |
| TODO | 0x8000FD70 | `__osPiDeviceBusy` | check if PI device is busy (returns status) |
| TODO | 0x800100C0 | `__osPiGetCmdQueue` | get PI command queue for device manager |

---

## LIBULTRA_SI (4 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x8000E4B0 | `__osSiRawStartDma` | SI raw DMA to/from PIF |
| TODO | 0x8000E560 | `osSiInit` | serial interface initialization |
| TODO | 0x8000E5B0 | `__osSiGetAccess` | acquire SI mutex |
| TODO | 0x8000E5F4 | `__osSiRelAccess` | release SI mutex |

---

## LIBULTRA_SP (6 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80007730 | `osSpTaskYielded` | check if SP task yielded |
| TODO | 0x8000D640 | `__osSpSetStatus` | set RSP status register |
| TODO | 0x8000D650 | `__osSpSetPc` | set RSP program counter (only if halted) |
| TODO | 0x8000D680 | `__osSpDma` | SP DMA transfer (RSP <-> RDRAM) |
| TODO | 0x8000D710 | `__osSpDeviceBusy` | check if RSP is busy (bits 0x1C) |
| TODO | 0x8000DF80 | `osSpTaskLoad_full` | load RSP task microcode and data |

---

## LIBULTRA_DP (4 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80007B00 | `osDpSetNextBuffer` | set RDP display list |
| TODO | 0x80007B80 | `osDpWait` | wait for RDP ready |
| TODO | 0x800099B0 | `osDpGetCounters` | get RDP performance counters |
| TODO | 0x8000D740 | `osDpIsBusy` | check if RDP is busy |

---

## LIBULTRA_GU (10 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80008A60 | `guOrthoF` | orthographic projection matrix (float) |
| TODO | 0x80008BB4 | `guOrtho` | orthographic projection matrix (fixed-point) |
| TODO | 0x80008C20 | `guPerspectiveF` | perspective projection matrix (float) |
| TODO | 0x80008E50 | `guPerspective` | perspective projection matrix (fixed-point) |
| TODO | 0x80008EB0 | `guLookAtF` | look-at view matrix (float) |
| TODO | 0x80009168 | `guLookAt` | look-at view matrix (fixed-point) |
| TODO | 0x800091E0 | `guMtxF2L` | convert float matrix to N64 fixed-point |
| TODO | 0x800092E0 | `guMtxL2F` | convert N64 fixed-point to float matrix |
| MATCHED | 0x80009394 | `guMtxIdentF` | create 4x4 identity matrix (float) |
| TODO | 0x8000941C | `guMtxIdent` | create N64 identity matrix (fixed-point) |

---

## LIBULTRA_PFS (20 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80009C10 | `osPfsInitPak` | initialize controller pak |
| TODO | 0x8000A350 | `osPfsAllocate` | allocate controller pak pages |
| TODO | 0x8000A520 | `osPfsDeleteFile` | delete controller pak file |
| TODO | 0x8000A700 | `osPfsRename` | rename controller pak file |
| TODO | 0x8000A8D8 | `osPfsFindFile` | find file in controller pak |
| TODO | 0x8000A970 | `osPfsReadWriteFile` | read/write controller pak file |
| TODO | 0x8000ACA4 | `__osPfsDeclearPage` | page allocation helper for file allocation |
| TODO | 0x8000ADF0 | `osPfsGetFileStat` | get controller pak file status |
| TODO | 0x8000AEC4 | `osPfsGetFileSize` | get controller pak file size |
| TODO | 0x8000B240 | `osPfsFreeBlocks` | get free blocks on controller pak |
| TODO | 0x8000B3E0 | `osPfsChecker` | check controller pak integrity |
| TODO | 0x8000B918 | `__osPfsCheckPages` | build inode usage bitmap for integrity check |
| TODO | 0x8000BBF0 | `__osPfsPageCheck` | validate inode references, detect duplicates |
| TODO | 0x8000BD90 | `osPfsChecker_full` | validate controller pak filesystem |
| TODO | 0x8000BE50 | `osPfsReAllocate` | reallocate controller pak file pages |
| TODO | 0x8000E850 | `__osPfsSelectBank` | select controller pak bank |
| TODO | 0x8000F2D0 | `osPfsReadWriteFile_pages` | read/write controller pak file pages |
| TODO | 0x8000F3A4 | `__osPfsRWInode` | read/write controller pak inode table |
| TODO | 0x8000F3A4 | `osPfsFileState` | get/verify controller pak file state |
| TODO | 0x8000FAC0 | `__osPfsDataChecksum` | calculate PFS page data checksum (32 bytes) |

---

## LIBULTRA_CONT (14 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80009450 | `__osContBuildPacket` | build controller pak read/write command |
| TODO | 0x800095AC | `__osContGetStatus` | get controller status |
| TODO | 0x80009658 | `__osContRamReset` | reset controller pak RAM |
| TODO | 0x80009730 | `osContStartQuery` | start controller status query |
| TODO | 0x800097AC | `osContGetQuery` | get controller query results (wrapper for __osCont |
| TODO | 0x800097D0 | `osContStartReadData2` | alternative start controller read |
| TODO | 0x80009854 | `osContGetReadData` | parse PIF response into OSContPad |
| TODO | 0x8000E620 | `osContStartReadData` | start controller read |
| TODO | 0x8000E6F0 | `__osContBuildRequest` | build controller request packet |
| TODO | 0x8000E7B4 | `__osContParseResponse` | parse controller response |
| TODO | 0x8000E8D0 | `__osContRamRead` | read from controller pak RAM |
| TODO | 0x8000F680 | `__osContRamWrite` | write to controller pak RAM (PFS page write) |
| TODO | 0x8000F9F0 | `__osContAddressCrc` | calculate address CRC for controller pak |
| TODO | 0x8000FA4C | `__osContDataCrc` | calculate data CRC for controller pak |

---

## LIBULTRA_MOTOR (4 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80009F20 | `osMotorInit` | rumble pak motor initialization |
| TODO | 0x8000A088 | `__osMotorAccess` | build rumble pak PIF command |
| TODO | 0x8000A194 | `osMotorStart` | start rumble pak motor (writes 0x01 to 0xC000) |
| TODO | 0x8000A2F0 | `osMotorStop` | stop rumble pak motor (writes 0x00 to 0xC000) |

---

## LIBULTRA_TIMER (21 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80006ED0 | `osSetTimerIntr` | set timer interrupt |
| TODO | 0x80009E18 | `__osTimerInterrupt` | timer interrupt handler |
| TODO | 0x8000E390 | `__osInsertTimer` | insert timer into sorted timer queue |
| TODO | 0x8000E3D0 | `osSetTimer` | set timer (initializes OSTimer and inserts to queu |
| TODO | 0x80091C04 | `entity_timer_update` | update entity timers |
| TODO | 0x80094DB0 | `Input_ConfigRepeatTimers` | writes repeat/debounce params to pad entry offsets |
| TODO | 0x800C8920 | `draw_timer` | show race time MM:SS.ms (228 bytes) |
| TODO | 0x800D2C2C | `race_timer_display` | race timer display - race elapsed time (176 bytes) |
| TODO | 0x800E2800 | `hud_timer_render` | hud timer render - draw race timer |
| TODO | 0x800FB928 | `hud_timer_display` | HUD timer display (712 bytes) |
| TODO | 0x800FE08C | `game_timer_start` | game timer start (1072 bytes) |
| TODO | 0x800FE4BC | `game_timer_stop` | game timer stop (100 bytes) |
| TODO | 0x800FE520 | `game_timer_reset` | game timer reset (144 bytes) |
| TODO | 0x800FE5B0 | `game_timer_elapsed` | game timer elapsed - returns centiseconds (412 byt |
| TODO | 0x800FE7A4 | `game_timer_pause` | game timer pause (164 bytes) |
| TODO | 0x800FE848 | `game_timer_resume` | game timer resume (220 bytes) |
| TODO | 0x800FE924 | `game_timer_lap` | game timer lap - records lap time (228 bytes) |
| TODO | 0x800FEA08 | `game_timer_split` | game timer split (668 bytes) |
| TODO | 0x800FECA4 | `game_timer_display` | game timer display (352 bytes) |
| TODO | 0x80100DF0 | `timer_reset` | timer reset (244 bytes) - resets race timer to 0 |
| TODO | 0x801011DC | `lap_timer_split` | lap timer split (1340 bytes) - records lap complet |

---

## LIBULTRA_THREAD (12 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x800021A4 | `idle_thread_entry` | boot idle thread: inits PI manager, creates game t |
| TODO | 0x800024FC | `audio_thread_entry` | audio thread entry point (priority 5) |
| TODO | 0x80006D40 | `osSetThreadPri` | set thread priority |
| TODO | 0x80006F30 | `osCreateThread` | create thread |
| TODO | 0x80007080 | `osStartThread` | start thread |
| TODO | 0x8000CFC4 | `__osCleanupThread` | save thread context and check for dispatch (except |
| TODO | 0x8000D0C4 | `__osEnqueueThread` | enqueue thread to priority queue |
| TODO | 0x8000D10C | `__osPopThread` | pop highest priority thread |
| TODO | 0x8000D11C | `__osDispatchThread` | dispatch next runnable thread |
| TODO | 0x8000D298 | `__osDispatchThreadYield` | dispatch thread after yield (jump target in except |
| TODO | 0x80010110 | `osYieldThread` | yield current thread (calls __osEnqueueAndYield wi |
| TODO | 0x800E7808 | `render_thread_entry` | render thread entry point (thread 5) |

---

## LIBULTRA_CACHE (6 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80007810 | `osInvalICache` | invalidate I-cache |
| TODO | 0x80007CA0 | `osWritebackDCache` | writeback D-cache |
| TODO | 0x80008460 | `osInvalICache_full` | invalidate instruction cache (cache op 0x00) |
| TODO | 0x800084E0 | `osInvalDCache` | invalidate D-cache |
| TODO | 0x800086F0 | `osWritebackDCache_full` | writeback data cache region |
| TODO | 0x8000870C | `osWritebackDCacheAll` | writeback all data cache |

---

## LIBULTRA_TLB (3 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x8000D840 | `__osTlbFlush` | flush all TLB entries |
| TODO | 0x8000D890 | `__osTlbInit` | initialize TLB for 0xC0000000 region |
| TODO | 0x8000FCB0 | `__osTLBLookup` | TLB lookup for virtual to physical translation |

---

## INFLATE (16 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80002660 | `lzss_decompress` | LZSS decompress with DMA |
| TODO | 0x800026C0 | `inflate_decompress` | inflate decompress with DMA |
| TODO | 0x80004A10 | `inflate_io_wait` | wait for DMA buffer ready |
| TODO | 0x80004AFC | `lzss_decode` | LZSS back-reference decoder |
| TODO | 0x80004D50 | `inflate_flush_window` | flush output window to destination |
| TODO | 0x80004D6C | `huft_alloc` | allocate Huffman table memory |
| TODO | 0x80004D98 | `huft_build` | build Huffman decoding tables |
| TODO | 0x80005464 | `inflate_free_window` | free huffman decode tables |
| TODO | 0x8000595C | `inflate_stored` | handle uncompressed block (type 0) |
| TODO | 0x80005B7C | `inflate_fixed` | handle fixed Huffman block (type 1) |
| TODO | 0x80005D44 | `inflate_dynamic` | handle dynamic Huffman block (type 2) |
| TODO | 0x800064D4 | `inflate_block` | decode one deflate block |
| TODO | 0x80006678 | `inflate_loop` | main inflate decompression loop |
| TODO | 0x800066D4 | `inflate_read_bits` | read compressed bits from buffered DMA stream |
| TODO | 0x80006814 | `inflate_entry` | main decompression entry point |
| TODO | 0x8000697C | `inflate_entry_alt` | alternative entry (heap alloc) |

---

## DLL (6 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x8000C050 | `dll_remove` | remove node from singly-linked list |
| TODO | 0x8000C090 | `dll_init` | initialize timer queue (doubly-linked list) |
| TODO | 0x8000C11C | `dll_update` | process expired timers (delta queue maintenance) |
| TODO | 0x8000C294 | `dll_reschedule` | reschedule timer interrupt (__osSetTimerIntr) |
| TODO | 0x8000C308 | `dll_insert` | insert timer into delta queue (__osInsertTimer) |
| TODO | 0x8000C490 | `dll_get_priority` | get thread priority helper |

---

## SCHEDULER (13 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x8000063C | `__scMain` | scheduler thread main loop (handles events 0x29A-0 |
| TODO | 0x80000780 | `__scSchedule` | drain command queue, append tasks to pending lists |
| TODO | 0x80000880 | `__scHandleRetrace` | VI retrace handler, sends msgs to clients |
| TODO | 0x80000950 | `__scHandleRSP` | SP interrupt handler, finalizes RSP tasks |
| TODO | 0x80000ACC | `__scHandleRDP` | DP interrupt handler, reschedules graphics |
| TODO | 0x80000BA4 | `__scTaskReady` | validate task timing before scheduling |
| TODO | 0x80000C54 | `__scExecTask` | process task msgQ, handle watchdog |
| TODO | 0x80000D6C | `__scAppendList` | insert task into audio/gfx pending list |
| TODO | 0x80000E20 | `__scExec` | launch next task to RSP/RDP |
| TODO | 0x80000F88 | `__scHandlePreNMI` | handle pre-NMI for audio tasks |
| TODO | 0x80000FE8 | `__scScheduleCore` | main scheduler algorithm |
| TODO | 0x80091C04 | `scheduler_recv` | scheduler message receive |
| TODO | 0x80091CA4 | `scheduler_jam` | scheduler message send |

---

## BOOT_MISC (30 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80000400 | `entrypoint` | clears BSS, sets stack, jumps to main |
| TODO | 0x80001350 | `viTickStart` | snapshot counter, compute frame duration |
| TODO | 0x800013C0 | `viEnableAccum` | enable time accumulation |
| TODO | 0x800013DC | `viDisableAccum` | disable time accumulation |
| TODO | 0x800013F4 | `viUpdateTime` | update frame delta timing |
| TODO | 0x800014F0 | `viScheduleTick` | schedule next tick deadline |
| TODO | 0x8000153C | `viAddTicks` | add ticks to deadline |
| TODO | 0x80001578 | `viGetTimeToDeadline` | get float time until deadline |
| TODO | 0x800015BC | `viDeadlinePassed` | check if deadline passed |
| TODO | 0x800015DC | `viStub` | no-op stub |
| TODO | 0x800015F0 | `display_update` | update display list with viewport interpolation |
| TODO | 0x80001B44 | `viewport_setup` | camera/viewport bounds setup |
| TODO | 0x80001D60 | `display_mode_tick` | countdown and apply pending display mode |
| TODO | 0x80001DFC | `get_tv_offset` | return TV-type specific offset (0 for PAL=0, 0xE f |
| TODO | 0x80001E58 | `apply_display_mode` | apply current display mode settings |
| TODO | 0x80001E84 | `get_viewport_pos` | get current viewport position |
| TODO | 0x80001ECC | `get_viewport_offset` | get viewport offset from base |
| TODO | 0x80001F2C | `update_viewport` | update viewport with bounds checking |
| TODO | 0x800020F0 | `main` | real main function (called from entrypoint) |
| TODO | 0x80002238 | `game_init` | main game initialization, creates threads |
| TODO | 0x80002540 | `dma_queue_init` | initialize DMA message queue |
| TODO | 0x80002598 | `dma_wait` | wait for DMA completion |
| TODO | 0x8000262C | `dma_signal` | signal DMA complete |
| TODO | 0x80004878 | `__write_exponent` | write exponent string (e+XX or e-XX) |
| TODO | 0x80004990 | `sprintf` | sprintf wrapper (calls fcvt) |
| TODO | 0x800049F0 | `__setfpcsr` |  |
| TODO | 0x8000D9C0 | `__umoddi3_alt` | unsigned 64-bit modulo (duplicate) |
| TODO | 0x8000DA88 | `__qdivrem` | quotient and remainder |
| TODO | 0x8000F8D0 | `bcmp` | compare two memory regions (returns 0 if equal, 1  |
| TODO | 0x800140B0 | `read_controllers` | controller input reading - controller input readin |

---

## GAME (1088 functions)

| Status | Address | Name | Notes |
|--------|---------|------|-------|
| TODO | 0x80087068 | `render_init_setup` | render initialization setup |
| TODO | 0x80087118 | `render_mode_setup` | RDP render mode setup (1772 bytes) |
| TODO | 0x800878EC | `render_flush_buffer` | render buffer flush operation |
| TODO | 0x80087A08 | `object_render` | (10048 bytes) - 3D object rendering with transform |
| TODO | 0x8008A398 | `audio_sync_wait` | audio synchronization wait (76 bytes) |
| TODO | 0x8008A474 | `render_sync_pipeline` | sync/pipeline setup (464 bytes) |
| TODO | 0x8008A650 | `texture_load_start` | texture loading start |
| TODO | 0x8008A6D0 | `sync_release_video` | simple sync release wrapper (44 bytes) |
| TODO | 0x8008A710 | `audio_task_start` | audio task start handler |
| TODO | 0x8008A750 | `audio_call_wrapper` | forwards to downstream audio routine |
| TODO | 0x8008A77C | `audio_queue_process` | audio queue processing (676 bytes) |
| TODO | 0x8008AA20 | `controller_rumble_thunk` | thunk to controller_rumble (32 bytes) |
| TODO | 0x8008AA40 | `dma_wait_complete` | wait for DMA completion |
| TODO | 0x8008ABF4 | `message_send_async` | asynchronous message send |
| TODO | 0x8008AC74 | `task_complete_signal` | task completion signal |
| TODO | 0x8008AD48 | `pointer_offset_wrapper` | adds offset 4 and calls sound_position_set (36 byt |
| TODO | 0x8008AE8C | `model_data_load` | model/mesh data loading |
| TODO | 0x8008B0D8 | `model_transform_setup` | model transform matrix setup |
| TODO | 0x8008B26C | `model_bounds_calc` | model bounding box calculation |
| TODO | 0x8008B474 | `vector_copy_scale` | vector copy with scale (80 bytes) arcade:vecmath.c |
| TODO | 0x8008B4C4 | `vector_normalize_length` | vector normalize and get length arcade:fmath.c:Slo |
| TODO | 0x8008B660 | `vector3d_store_transform` | vector transform and store arcade:unitvecs.c:bodto |
| TODO | 0x8008B69C | `physics_velocity_integrate_a` | physics velocity integration |
| TODO | 0x8008B964 | `physics_velocity_integrate_b` | physics velocity integration variant |
| TODO | 0x8008BA84 | `physics_velocity_integrate_c` | physics velocity integration variant |
| TODO | 0x8008BB8C | `physics_velocity_integrate_d` | physics velocity integration variant |
| TODO | 0x8008BC94 | `physics_velocity_integrate_e` | physics velocity integration variant |
| TODO | 0x8008BD9C | `physics_velocity_integrate_f` | physics velocity integration variant |
| TODO | 0x8008C688 | `audio_channel_update` | audio channel state update |
| TODO | 0x8008C724 | `audio_buffer_process` | audio buffer processing (68 bytes) |
| TODO | 0x8008C76C | `audio_param_apply` | apply audio parameters |
| TODO | 0x8008C884 | `entity_render_setup` | entity rendering setup (736 bytes) |
| TODO | 0x8008D120 | `entity_process_main` | major entity processing (1524 bytes) |
| TODO | 0x8008D6B0 | `math_utility` | math/vector utility (67 calls) arcade:math.c |
| TODO | 0x8008D714 | `matrix_rotation_build` | build rotation matrix arcade:fmath.c:CreateRYPMatr |
| TODO | 0x8008D764 | `euler_to_matrix` | euler angle to rotation matrix (372 bytes) arcade: |
| TODO | 0x8008D8D8 | `matrix_scale_apply` | apply scale to matrix arcade:fmath.c:ScaleUV() |
| TODO | 0x8008D93C | `anim_state_update` | entity animation state update (796 bytes) |
| TODO | 0x8008E0C8 | `render_flag_check` | check render flags |
| TODO | 0x8008E19C | `render_mode_select` | select render mode |
| TODO | 0x8008E280 | `render_state_init` | initialize render state |
| TODO | 0x8008E398 | `sign_extend_call` | sign extend and call (40 bytes) |
| TODO | 0x8008E440 | `render_state_process` | major render state processing (1472 bytes) |
| TODO | 0x8008EA10 | `entity_spawn_init` | entity spawn and initialization (1656 bytes) |
| TODO | 0x80090088 | `entity_spawn_callback` | entity spawn callback handler |
| TODO | 0x80090228 | `sound_call_simple` | sign-extends param, calls gfx_flush (44 bytes) |
| TODO | 0x80090254 | `sound_call_minimal` | sign-extends param, calls gfx_flush (48 bytes) |
| TODO | 0x80090310 | `physics_velocities_K` | integrate accel to velocity |
| TODO | 0x80090310 | `entity_callback_register` | entity with callback registration (1036 bytes) |
| TODO | 0x8009079C | `entity_transform_apply` | apply entity transformation matrix |
| TODO | 0x800908A0 | `physics_velocities_L` | integrate accel to velocity |
| TODO | 0x800908A0 | `entity_physics_update` | entity movement/physics update (716 bytes) |
| TODO | 0x80090B70 | `physics_forces_handler_A` | force accumulation (812 bytes) |
| TODO | 0x80090B70 | `entity_collision_detect` | entity collision detection (820 bytes) |
| TODO | 0x80090EA4 | `entity_frame_advance` | entity frame advance processing |
| TODO | 0x80090F4C | `entity_callback_invoke` | invoke entity callback function |
| TODO | 0x80090FEC | `physics_forces_handler_B` | force accumulation with flags |
| TODO | 0x80090FEC | `entity_update_callback` | entity update callback (2184 bytes) |
| TODO | 0x80091874 | `physics_velocities_M` | integrate accel to velocity |
| TODO | 0x80091874 | `entity_anim_texture` | entity animation with texture loading (616 bytes) |
| TODO | 0x80091CA4 | `entity_transform_calc` | calculate entity transforms |
| TODO | 0x80091E5C | `client_sync` | client synchronization (Jam+Recv) |
| TODO | 0x80091E5C | `entity_scale_sync` | entity scale/transform with sync (560 bytes) |
| TODO | 0x80091F34 | `retrace_sync` | retrace synchronization (Jam+Recv) |
| TODO | 0x80091F34 | `entity_hierarchy_update` | update entity parent-child hierarchy |
| TODO | 0x8009229C | `entity_state_init` | entity state initialization (600 bytes) |
| TODO | 0x80092360 | `frame_sync` | per-frame synchronization |
| TODO | 0x80092360 | `entity_flags_apply` | apply entity flag settings |
| TODO | 0x800924F4 | `buffer_swap` | buffer swap with message |
| TODO | 0x800924F4 | `physics_velocities_N` | integrate accel to velocity |
| TODO | 0x800924F4 | `entity_spawn_full` | entity spawn with full initialization (1636 bytes) |
| TODO | 0x80092C58 | `entity_name_copy` | copy entity name string |
| TODO | 0x80092E2C | `string_copy_format` | string/name copy with formatting (720 bytes) |
| TODO | 0x800930A4 | `physics_velocities_O` | integrate accel to velocity |
| TODO | 0x800930A4 | `entity_tick_main` | major entity processing loop (2684 bytes) |
| TODO | 0x80093B20 | `physics_forces_handler_C` | force accumulation with flags |
| TODO | 0x80093B20 | `drone_ai_update` | entity AI/behavior update for drones (3440 bytes) |
| TODO | 0x80094890 | `audio_channel_setup` | audio parameter setup (460 bytes) |
| TODO | 0x80094A54 | `audio_channel_priority` | set audio channel priority |
| TODO | 0x80094C30 | `audio_volume_pan` | audio volume and pan control (668 bytes) |
| TODO | 0x80094D20 | `input_connected_check` | handle pad connection/reset - handle pad connectio |
| TODO | 0x80094D20 | `Input_SetPadEnabledFlag` | set/clear bit 0x80 enable flag at byte 0x15 |
| TODO | 0x80094D68 | `input_button_adjust` | adjust button bits - adjust button bits |
| TODO | 0x80094D68 | `Input_SetPadSecondaryFlag` | set/clear bit 0x04 (rumble/lock) at byte 0x15 |
| TODO | 0x80094DB0 | `input_status_update` | update status bytes/edge tracking - update status  |
| TODO | 0x80094E00 | `input_analog_write` | write primary analog values - write primary analog |
| TODO | 0x80094E00 | `Input_SetAnalogBounds` | sets analog/deadzone bounds at offsets 0x18-0x1E |
| TODO | 0x80094E8C | `Input_InitPadHandlers` | initializes pad entry with handler pointers and ti |
| TODO | 0x80094EC8 | `input_handler` | input processing (60 calls) - input processing (60 |
| TODO | 0x80094EC8 | `Input_ApplyPadConfig` | copies controller config struct into pad buffer sl |
| TODO | 0x80094EC8 | `audio_param_update` | audio parameter update helper |
| TODO | 0x80094F88 | `input_new_data_wrapper` | toggle new data flag - toggle new data flag |
| TODO | 0x80094F88 | `object_byte26_set` | sets byte at object offset 26 |
| TODO | 0x80094FF0 | `input_dispatch` | dispatch from raw pad to input buffer - dispatch f |
| TODO | 0x80094FF0 | `audio_channel_reset` | audio channel stop and reset (156 bytes) |
| TODO | 0x8009508C | `pointer_compare_thunk` | thunk calling sound_position_set (32 bytes) |
| TODO | 0x80095120 | `validate_and_call` | validates pointers, calls triangle_draw (52 bytes) |
| TODO | 0x8009515C | `sound_volume_helper` | sound volume calculation helper (60 bytes) |
| TODO | 0x800951E0 | `sound_position_update` | 3D sound position update (492 bytes) |
| TODO | 0x80095240 | `physics_forces1` | tire/drag forces (arcade: forces1 in drivsym.c:391 |
| TODO | 0x80095360 | `sfx_play` | play sound effect |
| TODO | 0x80095380 | `poll_input` | controller/button polling - controller/button poll |
| TODO | 0x800953A0 | `draw_sprites` | sprite rendering |
| TODO | 0x800953C0 | `physics_forces2` | sum all forces (arcade: forces2 in drivsym.c:494) |
| TODO | 0x800953CC | `audio_fade_control` | audio fade control over time (220 bytes) |
| TODO | 0x800954A8 | `gfx_setup_e700` | display list setup (E700 clear) |
| TODO | 0x800954A8 | `sound_channel_enable` | sound enable/disable channel (128 bytes) |
| TODO | 0x800954E0 | `drone_move_update` | drone movement/position update (arcade: maxpath.c  |
| TODO | 0x80095528 | `music_track_control` | music track playback control (484 bytes) |
| TODO | 0x80095600 | `hud_update` | HUD element refresh |
| TODO | 0x8009570C | `audio_bus_route` | audio bus routing (244 bytes) |
| TODO | 0x80095720 | `anim_frame_update` | animation frame stepping |
| TODO | 0x80095800 | `audio_effect_setup` | reverb/effect setup (292 bytes) |
| TODO | 0x80095840 | `physics_torques` | calculate moments (arcade: torques in drivsym.c:53 |
| TODO | 0x80095924 | `audio_timing_sync` | audio timing sync with video (184 bytes) |
| TODO | 0x80095960 | `bgm_playback` | background music handling |
| TODO | 0x800959DC | `audio_pitch_adjust` | audio pitch adjustment (72 bytes) |
| TODO | 0x80095A24 | `sound_priority_set` | sound priority management (236 bytes) |
| TODO | 0x80095A80 | `ui_render` | UI rendering |
| TODO | 0x80095B10 | `audio_stream_control` | audio streaming control (236 bytes) |
| TODO | 0x80095BA0 | `jump_handler` | jump mechanic logic |
| TODO | 0x80095BFC | `audio_buffer_manage` | audio DMA buffer management (268 bytes) |
| TODO | 0x80095CC0 | `npc_interaction` | NPC interaction handler |
| TODO | 0x80095D04 | `audio_state_save` | audio state save/restore (468 bytes) |
| TODO | 0x80095DE0 | `particles_animate` | particle effect animation |
| TODO | 0x80095F00 | `voice_playback` | voice line playback |
| TODO | 0x80095FD8 | `audio_reverb_update` | reverb state update |
| TODO | 0x800960D4 | `dma_queue_sync` | DMA queue synchronization |
| TODO | 0x800960D4 | `audio_effect_process` | audio effect processing |
| TODO | 0x8009614C | `dma_complete_wait` | DMA completion wait |
| TODO | 0x8009614C | `entity_sound_attach` | entity sound attachment with sync (216 bytes) |
| TODO | 0x80096240 | `sound_3d_update` | 3D sound position/volume update |
| TODO | 0x80096298 | `slot_value_get` | get value from slot array |
| TODO | 0x800962D4 | `dma_request` | DMA request with queue |
| TODO | 0x800962D4 | `audio_loop_control` | audio loop point control |
| TODO | 0x8009638C | `display_list_alloc` | display list memory allocation |
| TODO | 0x800963E8 | `display_list_traverse` | display list traversal (452 bytes) |
| TODO | 0x800965BC | `entity_render_mode` | entity render setup (244 bytes) |
| TODO | 0x80096734 | `entity_lod_select` | entity LOD selection by distance (716 bytes) |
| TODO | 0x80096A00 | `entity_cull_check` | entity view frustum culling (348 bytes) |
| TODO | 0x80096B5C | `lookup_with_output` | table lookup with output pointer |
| TODO | 0x80096CC4 | `entity_render_transform` | entity render with transforms (1216 bytes) |
| TODO | 0x80097184 | `audio_queue_recv` | audio queue receive only |
| TODO | 0x80097184 | `entity_collision_response` | entity collision response (876 bytes) |
| TODO | 0x8009731C | `audio_buffer_sync` | audio buffer synchronization |
| TODO | 0x8009731C | `entity_bounds_update` | update entity bounding box |
| TODO | 0x80097384 | `audio_helper` | audio processing helper |
| TODO | 0x80097470 | `audio_dma_sync` | audio DMA synchronization |
| TODO | 0x80097470 | `entity_visibility_calc` | calculate entity visibility state |
| TODO | 0x800974EC | `audio_task_complete` | audio task completion |
| TODO | 0x80097798 | `audio_frame_sync` | audio per-frame sync |
| TODO | 0x80097798 | `vehicle_physics_sync` | vehicle physics/collision sync |
| TODO | 0x800979A8 | `mp_interval_pos` | calc car position relative to maxpath interval (ar |
| TODO | 0x80097AFC | `MP_TargetSteerPos` | calc target steering position from maxpath (arcade |
| TODO | 0x80097CA0 | `camera_transform` | camera/view transformation |
| TODO | 0x80098554 | `resource_process_thunk` | thunk to texture_set |
| TODO | 0x80098574 | `camera_update_a` | nested camera update helper |
| TODO | 0x800985F4 | `param_reshuffle_wrapper` | reorganize params, call matrix_translate (44 bytes |
| TODO | 0x80098620 | `camera_update_b` | nested camera update helper |
| TODO | 0x80098620 | `matrix_push_offset` | push matrix with offset |
| TODO | 0x800986B0 | `pointer_offset8_call` | call matrix_identity with offset (36 bytes) |
| TODO | 0x800987E8 | `entity_state_check` | simple state check (140 bytes) |
| TODO | 0x80098874 | `entity_flag_check` | entity flag check (108 bytes) |
| TODO | 0x800988E0 | `entity_damage_update` | entity damage/health update (516 bytes) |
| TODO | 0x80098AE4 | `engine_sound_sync` | engine/sound synchronization |
| TODO | 0x80098AE4 | `entity_physics_step` | entity physics integration step (1244 bytes) |
| TODO | 0x80098FC0 | `world_collision` | world/environment collision detection |
| TODO | 0x80098FC0 | `entity_ground_check` | entity ground raycast (708 bytes) |
| TODO | 0x800992AC | `track_render` | track generation/rendering |
| TODO | 0x800992AC | `entity_ai_pathfind` | entity AI pathfinding (2388 bytes) |
| TODO | 0x80099BFC | `render_object` | (10236 bytes) - 3D model rendering, uses G_DL |
| TODO | 0x80099BFC | `render_display_list` | large display list builder (10236 bytes) |
| TODO | 0x80099BFC | `render_object` | 3D object rendering (10KB) |
| TODO | 0x8009C5BC | `camera_update_c` | camera update helper |
| TODO | 0x8009C5BC | `lighting_setup` | lighting setup helper |
| TODO | 0x8009C5E0 | `camera_update_d` | camera update helper |
| TODO | 0x8009C5E0 | `lighting_calc` | surface lighting calculation (1560 bytes) |
| TODO | 0x8009C8F0 | `particle_system` | particle system/effects |
| TODO | 0x8009C8F0 | `track_geometry_process` | track geometry processing for rendering (5368 byte |
| TODO | 0x8009DC50 | `track_collision` | track/road surface collision |
| TODO | 0x8009DC50 | `track_segment_setup` | track segment initialization |
| TODO | 0x8009DD18 | `track_collision_edge` | track edge collision helper |
| TODO | 0x8009DD18 | `track_texture_setup` | track texture loading and setup |
| TODO | 0x8009DD88 | `track_collision_wall` | track wall collision helper |
| TODO | 0x8009DD88 | `track_segment_render` | render single track segment with display list (330 |
| TODO | 0x8009EA70 | `track_collision_curb` | track curb/bump collision |
| TODO | 0x8009EA70 | `track_bounds_check` | check position within track boundaries (168 bytes) |
| TODO | 0x8009EB18 | `gfx_setup_fc` | display list setup (E7/FC) |
| TODO | 0x8009EB18 | `track_height_query` | get terrain height at position (168 bytes) |
| TODO | 0x8009EBC0 | `physics_float_calc` | float-heavy physics calculation |
| TODO | 0x8009EBC0 | `track_surface_type` | get surface material type at position (1188 bytes) |
| TODO | 0x8009F064 | `viewport_perspective` | calls guPerspective (5216 bytes) |
| TODO | 0x8009F064 | `track_collision_test` | raycast against track geometry (8600 bytes) |
| TODO | 0x8009FDC4 | `audio_init_stub` | audio init stub (LUI) |
| TODO | 0x8009FF00 | `InitDrones` | initialize drone system for race (arcade: drones.c |
| TODO | 0x800A0000 | `DoDrones` | main drone housekeeping per frame (arcade: drones. |
| TODO | 0x800A0100 | `EndDrones` | cleanup drone system at race end (arcade: drones.c |
| TODO | 0x800A04C4 | `render_scene` | (2720 bytes) - scene setup, viewport modes, uses G |
| TODO | 0x800A04C4 | `Input_ProcessGameplayPad` | per-frame gameplay/menu input (2720 bytes), reads  |
| TODO | 0x800A0800 | `drone_steering_calc` | calculate drone steering angle (arcade: maxpath.c: |
| TODO | 0x800A0B00 | `MaxPath` | main maxpath update per frame (arcade: maxpath.c:M |
| TODO | 0x800A0BAC | `drone_path_branch` | drone path branching decision (arcade: maxpath.c b |
| TODO | 0x800A0C00 | `MP_FindInterval` | find current maxpath interval (arcade: maxpath.c:M |
| TODO | 0x800A0D00 | `MP_IntervalPos` | calc position relative to maxpath interval (arcade |
| TODO | 0x800A0F74 | `MP_TargetSpeed` | calc target speed from maxpath (arcade: maxpath.c: |
| TODO | 0x800A0FDC | `NextMaxPath` | get next maxpath index wrapping at lap end (arcade |
| TODO | 0x800A1000 | `MaxPathControls` | drone path following controller (arcade: maxpath.c |
| TODO | 0x800A1080 | `next_maxpath` | next maxpath index with loop flag (arcade: maxpath |
| TODO | 0x800A1180 | `prev_maxpath` | prev maxpath index with loop flag (arcade: maxpath |
| TODO | 0x800A11E4 | `PrevMaxPath` | get previous maxpath index (arcade: maxpath.c:Prev |
| TODO | 0x800A1244 | `InitMaxPath` | initialize maxpath system for drones (arcade: maxp |
| TODO | 0x800A12D4 | `gfx_lui_setup` | graphics register setup |
| TODO | 0x800A12D4 | `assign_default_paths` | assign default paths to drones (arcade: maxpath.c: |
| TODO | 0x800A133C | `sync_maxpath_to_checkpoint` | sync maxpath index to checkpoint (arcade: maxpath. |
| TODO | 0x800A13E8 | `check_mpath_save` | check if lap faster and save maxpath (arcade: maxp |
| TODO | 0x800A14C0 | `adjust_maxpath_due_to_lap` | adjust maxpath for lap crossing (arcade: maxpath.c |
| TODO | 0x800A153C | `physics_step_forces` | physics force accumulation step |
| TODO | 0x800A1648 | `track_segment_load` | (2256 bytes) load track segment from ROM, GZIP dec |
| TODO | 0x800A1660 | `gfx_store_word` | graphics store helper |
| TODO | 0x800A17F4 | `audio_branch_handler` | audio conditional handler |
| TODO | 0x800A1800 | `avoid_areas` | obstacle avoidance for drones (arcade: maxpath.c:a |
| TODO | 0x800A19E4 | `drone_path_select` | drone path selection logic (arcade: maxpath.c) |
| TODO | 0x800A1D38 | `gfx_lui` | graphics register load |
| TODO | 0x800A1F18 | `track_data_decompress` | (644 bytes) decompress GZIP/ZLIB/LZSS track data |
| TODO | 0x800A214C | `physics_contact_test` | physics contact conditional test |
| TODO | 0x800A21A4 | `track_collision_setup` | (732 bytes) setup 64x64 collision height/surface g |
| TODO | 0x800A2378 | `MaxPathZeroControls` | zero out drone controls (arcade: maxpath.c) |
| TODO | 0x800A2504 | `slot_state_lookup` | accesses player/slot state array (58 calls) |
| TODO | 0x800A2564 | `gfx_store` | graphics store helper |
| TODO | 0x800A25C0 | `AdjustSteer` | adjust drone steering toward target pos (arcade: m |
| TODO | 0x800A2680 | `physics_velocities_P` | integrate accel to velocity |
| TODO | 0x800A2680 | `AdjustSpeed` | adjust drone throttle/brake to target speed (arcad |
| TODO | 0x800A27AC | `audio_branch_handler_b` | audio conditional handler |
| TODO | 0x800A2990 | `drone_set_catchup` | set catchup speed scaling for drone (arcade: drone |
| TODO | 0x800A29A8 | `physics_force_limit` | physics force limit conditional |
| TODO | 0x800A2B30 | `drone_speed_adjust` | adjust drone speed based on target (arcade: maxpat |
| TODO | 0x800A2CE4 | `set_catchup` | set catchup mode for drones (arcade: drones.c:set_ |
| TODO | 0x800A2D0C | `no_catchup` | disable catchup for tournament mode (arcade: drone |
| TODO | 0x800A2D1C | `gfx_store_b` | graphics store helper |
| TODO | 0x800A2D4C | `track_process_main` | (3116 bytes) per-frame track segment processing |
| TODO | 0x800A2E64 | `audio_branch_handler_c` | audio conditional handler |
| TODO | 0x800A30AC | `gfx_lui_b` | graphics register load |
| TODO | 0x800A3654 | `track_spline_interp` | (296 bytes) Catmull-Rom spline interpolation |
| TODO | 0x800A373C | `drone_rwtobod` | convert real-world to body coords for drone arcade |
| TODO | 0x800A377C | `track_render_process` | (3156 bytes) per-frame track rendering |
| TODO | 0x800A4508 | `car_lod_select` | car model LOD selection (visuals: level-of-detail) |
| TODO | 0x800A48C8 | `audio_start` | start audio subsystem (called from init.c) |
| TODO | 0x800A4934 | `sound_init` | initialize sound system (called from init.c) |
| TODO | 0x800A4940 | `wheel_rotation_update` | update wheel rotation and slip (tires: visual rota |
| TODO | 0x800A4AC4 | `wheel_setup_initial` | wheel initial setup |
| TODO | 0x800A4B48 | `resource_update_global` | update global resource via matrix stack |
| TODO | 0x800A4B6C | `wheel_params_set` | set wheel parameters |
| TODO | 0x800A4C54 | `init_wait_completion` | wait for initialization completion |
| TODO | 0x800A4CC0 | `suspension_update` | suspension compression update (tires.c: dotireforc |
| TODO | 0x800A4E60 | `tire_skid_mark` | tire skid mark generation (visuals: DoSkid) |
| TODO | 0x800A501C | `suspension_setup` | suspension spring setup |
| TODO | 0x800A511C | `suspension_params_init` | suspension parameters init |
| TODO | 0x800A5160 | `suspension_damper_set` | suspension damper settings |
| TODO | 0x800A51E0 | `car_damage_visual` | car damage visual update (visuals: damage deformat |
| TODO | 0x800A5588 | `engine_particle_effect` | engine particle effect spawn (visuals: dust/sparks |
| TODO | 0x800A5744 | `exhaust_smoke_effect` | exhaust smoke effect render (visuals: StartSmoke) |
| TODO | 0x800A5908 | `arb_rate_set` | anti-roll bar rate set |
| TODO | 0x800A5A74 | `tire_params_load` | tire parameters load |
| TODO | 0x800A5B60 | `render_state_init` | initialize render state variables |
| TODO | 0x800A5BB8 | `tire_compound_set` | tire compound type set |
| TODO | 0x800A5D34 | `car_shadow_render` | car shadow rendering (visuals: shadow) |
| TODO | 0x800A6094 | `car_lights_render` | headlight/taillight rendering (visuals) |
| TODO | 0x800A6100 | `car_steering_response` | arcade:fswheel.c:PotWheelPos N64 car steering resp |
| TODO | 0x800A6244 | `brake_light_update` | brake light update (visuals: App_M_BRAKE_LIGHTS) |
| TODO | 0x800A6404 | `car_render_full` | car full render with effects (visuals: main car re |
| TODO | 0x800A6BE4 | `wheel_render_full` | wheel full render |
| TODO | 0x800A7508 | `viewport_scale` | scale viewport dimensions (display.c) |
| TODO | 0x800A78C8 | `car_setup_physics` | car physics setup |
| TODO | 0x800A7A10 | `Input_AllocPadSlot` | scans pad buffer for free slot, initializes offset |
| TODO | 0x800A7AE4 | `car_mass_set` | set car mass and inertia |
| TODO | 0x800A7C9C | `car_gear_shift` | arcade:drivetra.c:autoshift manual gear shift hand |
| TODO | 0x800A7D88 | `car_get_gear` | arcade:drivetra.c return current gear value (-1=R, |
| TODO | 0x800A7DF0 | `object_process_thunk` | thunk to camera_target_set |
| TODO | 0x800A7E00 | `tire_force_calc` | (1200 bytes) tire force calculation using friction |
| TODO | 0x800A80D0 | `car_cg_height_set` | set car center of gravity height |
| TODO | 0x800A8D9C | `car_collision_init` | car collision bounds init |
| TODO | 0x800A8F64 | `car_collision_update` | car collision update |
| TODO | 0x800A9390 | `car_crash_detect` | car crash detection |
| TODO | 0x800A99C8 | `car_crash_response` | car crash impulse response |
| TODO | 0x800AA454 | `car_reset_position` | car reset to track position |
| TODO | 0x800AA708 | `car_spawn_at_checkpoint` | car spawn at checkpoint |
| TODO | 0x800AAB44 | `car_velocity_clamp` | car velocity clamping |
| TODO | 0x800AAE68 | `car_angular_velocity_clamp` | car angular velocity clamp |
| TODO | 0x800AB544 | `drivetrain_update` | arcade:drivetra.c:drivetrain main drivetrain simul |
| TODO | 0x800AB70C | `engine_rpm_calc` | arcade:drivetra.c:engine engine torque calculation |
| TODO | 0x800AB758 | `object_transform_set` | set object transform matrix |
| TODO | 0x800AB7D8 | `engine_torque_calc` | arcade:drivetra.c:enginetorque bilinear torque cur |
| TODO | 0x800ABA18 | `clutch_slip_calc` | arcade:drivetra.c:whatslips clutch slip calculatio |
| TODO | 0x800ABB04 | `autoshift_update` | arcade:drivetra.c:autoshift automatic transmission |
| TODO | 0x800ABBD0 | `transmission_shift` | arcade:drivetra.c:upshift,downshift gear shift wit |
| TODO | 0x800ABCC8 | `transmission_ratio_get` | arcade:drivetra.c:transmission calculate transrati |
| TODO | 0x800ABD60 | `find_best_gear` | arcade:drivetra.c:find_best_gear find appropriate  |
| TODO | 0x800ABDE8 | `drivetrain_interp` | arcade:drivetra.c:interp linear interpolation help |
| TODO | 0x800AC3D8 | `differential_output` | arcade:drivetra.c:drivetrain(39-57) magic differen |
| TODO | 0x800AC668 | `wheel_torque_apply` | arcade:drivetra.c:drivetrain(41-56) apply dwtorque |
| TODO | 0x800AC6F4 | `fp_call_wrapper` | function pointer call wrapper |
| TODO | 0x800AC75C | `brake_force_apply` | apply brake force |
| TODO | 0x800AC820 | `resource_slot_get` | get resource slot value |
| TODO | 0x800AC840 | `slot_deactivate` | deactivate model slot |
| TODO | 0x800AC898 | `player_state_get` | get player state value |
| TODO | 0x800ACA9C | `handbrake_apply` | apply handbrake |
| TODO | 0x800ACB74 | `vector_diff_process` | process vector difference |
| TODO | 0x800ACC18 | `traction_control` | traction control system |
| TODO | 0x800AD008 | `abs_brake_control` | ABS brake control |
| TODO | 0x800AD0A0 | `steering_angle_calc` | steering angle calculation |
| TODO | 0x800AD128 | `steering_sensitivity` | steering sensitivity adjust |
| TODO | 0x800AD734 | `input_process_controller` | process controller input |
| TODO | 0x800ADD58 | `input_deadzone_apply` | apply input deadzone |
| TODO | 0x800AEB54 | `camera_position_update` | camera position update |
| TODO | 0x800AED2C | `object_init_cleared` | initialize cleared object |
| TODO | 0x800AED64 | `camera_target_track` | camera target tracking |
| TODO | 0x800AEFE8 | `camera_collision_check` | camera collision avoidance |
| TODO | 0x800AF06C | `save_write_data` | save game data to buffer |
| TODO | 0x800AF51C | `camera_smooth_lerp` | camera smooth interpolation |
| TODO | 0x800AF5E0 | `save_slot_valid` | check save slot validity |
| TODO | 0x800AF690 | `save_load_data` | load save from controller pak |
| TODO | 0x800AF8CC | `save_delete` | delete save from slot |
| TODO | 0x800AFB38 | `save_validate` | validate save data integrity |
| TODO | 0x800AFD5C | `cpak_init` | controller pak initialization |
| TODO | 0x800B0180 | `cpak_read` | controller pak read - read data from controller pa |
| TODO | 0x800B04D0 | `pool_linked_list_init` | initialize pool as linked list (128 bytes) - sets  |
| TODO | 0x800B0550 | `struct_fields_init` | initialize structure fields (see game.c comment) |
| TODO | 0x800B0580 | `audio_channel_alloc` | allocate audio channel |
| TODO | 0x800B0618 | `audio_channel_free` | free audio channel |
| TODO | 0x800B066C | `audio_voice_set` | set audio voice parameters |
| TODO | 0x800B0868 | `PhysicsObjectList_Update` | per-frame physics/vehicle list updater |
| TODO | 0x800B0868 | `car_physics_update` | car physics update - called in race loop |
| TODO | 0x800B0868 | `PhysicsObjectList_Update` | - walks linked list at 0x801491F0, physics update |
| TODO | 0x800B0870 | `physics_update_all` | physics object linked list update (called when phy |
| TODO | 0x800B0904 | `audio_init` | audio system initialization - set up channels and  |
| TODO | 0x800B0A90 | `audio_shutdown` | audio system shutdown - stop all sounds and disabl |
| TODO | 0x800B0C48 | `audio_frame_update` | per-frame audio update - process fades and channel |
| TODO | 0x800B0F68 | `sound_bank_load` | load sound bank from ROM via DMA (364 bytes) |
| TODO | 0x800B10D4 | `sound_bank_unload` | unload sound bank and stop dependent sounds (1248  |
| TODO | 0x800B15B4 | `music_seq_load` | load music sequence from ROM (1428 bytes) |
| TODO | 0x800B1B48 | `music_control` | music playback control - stop/play/pause/resume/re |
| TODO | 0x800B1B90 | `radio_stop` | arcade:carsnd.c:StopRadio stop radio/music (48 byt |
| TODO | 0x800B1BC0 | `radio_start` | arcade:carsnd.c:StartRadio start radio/music by st |
| TODO | 0x800B1F44 | `music_volume_set` | set music volume level 0.0-1.0 (200 bytes) |
| TODO | 0x800B200C | `music_tempo_set` | set music tempo/playback speed (1612 bytes) |
| TODO | 0x800B24EC | `sfx_play` | play sound effect with volume and pan (464 bytes) |
| TODO | 0x800B2658 | `sfx_stop` | stop sound effect by handle or sound ID (256 bytes |
| TODO | 0x800B2828 | `sfx_volume_set` | set sound effect volume (196 bytes) |
| TODO | 0x800B2928 | `sfx_position_3d` | 3D sound position - calculates attenuation and pan |
| TODO | 0x800B2C20 | `sound_state_init` | arcade:carsnd.c:InitSndState initialize sound stat |
| TODO | 0x800B2C40 | `car_sounds_init` | arcade:carsnd.c:InitCarSnds initialize car sounds  |
| TODO | 0x800B2D20 | `listener_position_set` | set audio listener position and forward vector (21 |
| TODO | 0x800B2D60 | `car_sounds_interrupt` | arcade:carsnd.c:DoIntCarSounds high-priority car s |
| TODO | 0x800B2D80 | `engine_sound_start` | arcade:carsnd.c:StartEngineSound start engine soun |
| TODO | 0x800B2DF8 | `engine_sound_update` | arcade:carsnd.c:DoEngineSound update engine sound  |
| TODO | 0x800B3340 | `engine_sound_stop` | arcade:carsnd.c:StopEngineSound stop engine sound  |
| TODO | 0x800B338C | `tire_sound_update` | arcade:carsnd.c:DoTireSqueals update tire squeal s |
| TODO | 0x800B34C0 | `collision_force_check` | arcade:carsnd.c:check_forces_on_car check collisio |
| TODO | 0x800B358C | `sound_stop` | stop sound (called when engine stops) |
| TODO | 0x800B3590 | `Input_RegisterActivePad` | marks pad slot active (byte 0x16 = 2), inserts int |
| TODO | 0x800B3590 | `collision_peak_detect` | arcade:carsnd.c:get_force_and_peak force peak dete |
| TODO | 0x800B3598 | `bump_state_init` | arcade:carsnd.c:init_bump initialize bump state by |
| TODO | 0x800B35A0 | `bump_sounds_init` | arcade:carsnd.c:init_bump_sounds initialize bump c |
| TODO | 0x800B3620 | `scrape_sound_stop` | arcade:carsnd.c:kill_scrape_sound stop scrape soun |
| TODO | 0x800B362C | `collision_sound_play` | arcade:carsnd.c:do_bump_sounds trigger collision/i |
| TODO | 0x800B3700 | `skid_sounds_init` | arcade:carsnd.c:init_skids initialize skid sounds  |
| TODO | 0x800B3710 | `wind_sound_update` | arcade:carsnd.c:DoCarSounds update wind noise base |
| TODO | 0x800B37E8 | `sound_control` | audio/sound (called with -1 as stop signal) |
| TODO | 0x800B39BC | `crowd_cheer_play` | play crowd cheer sound for stunts/checkpoints (396 |
| TODO | 0x800B3A00 | `target_in_car_check` | arcade:carsnd.c:target_inside_box target-car colli |
| TODO | 0x800B3A40 | `target_sound_play` | arcade:carsnd.c:target_sound target collision soun |
| TODO | 0x800B3B4C | `ambient_sound_set` | set ambient/environment sound volume by ID (468 by |
| TODO | 0x800B3D18 | `sound_update_channel` | audio sync/update - synchronizes audio state (228  |
| TODO | 0x800B3D20 | `voice_play` | play announcer voice clip - start/position/lap ann |
| TODO | 0x800B3F00 | `object_type_byte2_get` | returns byte at offset 2 from current object |
| TODO | 0x800B3F28 | `object_type_byte3_get` | returns byte at offset 3 from current object |
| TODO | 0x800B3F50 | `object_helper` | object helper (77 calls) |
| TODO | 0x800B3F50 | `object_bytes_sum_global` | returns sum of mode_byte_2 + current_object offset |
| TODO | 0x800B3FA4 | `object_utility` | object management (107 calls) |
| TODO | 0x800B3FA4 | `object_manager_update` | object management update (107 calls) |
| TODO | 0x800B41C0 | `object_byte9_set` | sets byte at current_object+9 |
| TODO | 0x800B4200 | `slot_state_setup` | player/slot state management (171 calls) |
| TODO | 0x800B4208 | `slot_state_init` | player slot initialization |
| TODO | 0x800B4208 | `slot_state_init` | player slot state initialization |
| TODO | 0x800B42F0 | `object_create` | allocate and initialize object wrapper |
| TODO | 0x800B4360 | `fcvt_wrapper` | float to string conversion wrapper (57 calls) |
| TODO | 0x800B438C | `voice_stop` | stop currently playing voice and clear queue (224  |
| TODO | 0x800B45BC | `sound_handles_clear` | stops and clears sound handles in object array |
| TODO | 0x800B466C | `format_string_parse` | format string parser with jump table (55 calls) |
| TODO | 0x800B4738 | `voice_stop_2` | text formatting helper function (224 bytes) |
| TODO | 0x800B4818 | `audio_bus_mix` | set volume levels for audio buses/categories (892  |
| TODO | 0x800B4B94 | `reverb_setup` | arcade:carsnd.c:init_reverb configure reverb effec |
| TODO | 0x800B4C80 | `reverb_update` | arcade:carsnd.c:handle_reverb update reverb from e |
| TODO | 0x800B4DAC | `audio_pause` | pause/resume all game audio |
| TODO | 0x800B4E70 | `audio_ducking` | audio ducking - reduce lower priority sounds |
| TODO | 0x800B4FB0 | `sound_priority_set` | set priority for sound handle |
| TODO | 0x800B557C | `ambient_sounds_clear` | clear all ambient and environment sound handles |
| TODO | 0x800B55FC | `visual_objects_update` | iterates and updates visual objects |
| TODO | 0x800B5694 | `sound_priority_mgr` | sound priority management (184 bytes) |
| TODO | 0x800B574C | `sound_loop_set` | enable/disable looping for playing sound (340 byte |
| TODO | 0x800B58A0 | `sound_pitch_set` | set pitch for playing sound (float to N64 fixed-po |
| TODO | 0x800B5948 | `sound_volume_set` | set volume 0.0-1.0 for playing sound (176 bytes) |
| TODO | 0x800B59F8 | `audio_spatialize` | 3D sound spatialization - pan/attenuation/doppler  |
| TODO | 0x800B5F4C | `audio_distance_atten` | calculate volume attenuation from distance (464 by |
| TODO | 0x800B5F88 | `audio_doppler` | calculate doppler pitch shift from relative motion |
| TODO | 0x800B5FC4 | `resource_type_select` | select resource type based on bit flags |
| TODO | 0x800B6024 | `audio_occlusion` | calculate audio occlusion between listener and sou |
| TODO | 0x800B6138 | `set_race_state` | may set 0x00200000 state |
| TODO | 0x800B6138 | `entity_audio_update` | update all audio for game entity (car/drone/object |
| TODO | 0x800B61B0 | `resource_alloc_conditional` | conditionally allocate resource or return -1 |
| TODO | 0x800B61FC | `audio_doppler_full` | full doppler calculation with velocity |
| TODO | 0x800B65B4 | `render_helper` | rendering helper (65 calls) |
| TODO | 0x800B65B8 | `render_helper_main` | rendering helper function (65 calls) |
| TODO | 0x800B6788 | `audio_doppler_calc` | doppler calculation - listener/source velocity (11 |
| TODO | 0x800B6BEC | `menu_input_process` | process menu controller input |
| TODO | 0x800B7128 | `object_bytes23_sum` | returns sum of bytes at offsets 2 and 3 |
| TODO | 0x800B7170 | `sound_pitch_diff_calc` | calculate difference with sound_pitch_set result |
| TODO | 0x800B71A0 | `sound_pitch_diff_halved` | calculate halved difference with sound_pitch_set |
| TODO | 0x800B71D4 | `state_utility` | state helper (125 calls) |
| TODO | 0x800B71DC | `sound_state_helper` | sound state helper function (125 calls) |
| TODO | 0x800B7440 | `audio_listener_update` | update audio listener state from camera |
| TODO | 0x800B74A0 | `dispatch_handler` | event dispatch (152 calls) |
| TODO | 0x800B74A0 | `event_dispatch_handler` | event dispatch handler (152 calls) |
| TODO | 0x800B78F8 | `particles_init` | initialize particle system |
| TODO | 0x800B7A40 | `particles_update` | update all active particles |
| TODO | 0x800B8000 | `particles_spawn` | spawn new particle emitter |
| TODO | 0x800B80C8 | `particles_spawn_emitter` | spawn particles for specific emitter index |
| TODO | 0x800B811C | `effects_update_emitters` | particle/trail/FX system updater |
| TODO | 0x800B811C | `visual_effects_update` | called in race loop |
| TODO | 0x800B811C | `Effects_UpdateEmitters` | - loops over 152-byte effect slots at 0x80150B70 |
| TODO | 0x800B811C | `particle_type_set` | set particle type/behavior |
| TODO | 0x800B821C | `particle_position_set` | set particle spawn position |
| TODO | 0x800B82C8 | `particle_velocity_set` | set particle initial velocity |
| TODO | 0x800B8374 | `particle_lifetime_set` | set particle lifetime in frames |
| TODO | 0x800B8650 | `particle_render` | render particle sprites |
| TODO | 0x800B8820 | `particle_collision` | particle collision detection |
| TODO | 0x800B8C14 | `physics_sym` | main per-frame physics update (arcade: sym in driv |
| TODO | 0x800B9000 | `physics_regular` | standard physics cycle (arcade: regular in drivsym |
| TODO | 0x800B9130 | `physics_step` | single physics integration step |
| TODO | 0x800B9194 | `physics_collision_test` | test physics collision between objects |
| TODO | 0x800B93A8 | `physics_response` | calculate physics collision response |
| TODO | 0x800B9400 | `physics_positions` | integrate velocity to position (arcade: positions  |
| TODO | 0x800B9774 | `physics_ground_trace` | trace ray to ground for terrain contact |
| TODO | 0x800B9A0C | `physics_velocity_clamp` | clamp velocity to maximum |
| TODO | 0x800B9D68 | `physics_friction_apply` | apply friction force to object |
| TODO | 0x800BA000 | `tire_calc_force` | full tire force calculation (arcade: dotireforce i |
| TODO | 0x800BA00C | `audio_channel_alloc` | allocate audio channel based on priority |
| TODO | 0x800BA2E8 | `audio_channel_free` | free audio channel |
| TODO | 0x800BA46C | `audio_priority_find` | find channel for sound based on priority (472 byte |
| TODO | 0x800BA644 | `audio_heap_alloc` | allocate memory from audio heap (380 bytes) |
| TODO | 0x800BA7C4 | `audio_mixer_main` | main audio mixer - mix all channels to output (199 |
| TODO | 0x800BA800 | `tire_suspension_update` | suspension compression calc (arcade: suscomp) |
| TODO | 0x800BAAA0 | `audio_effect_apply` | apply audio effect to output (172 bytes) |
| TODO | 0x800BAD58 | `audio_effect_remove` | remove audio effect (252 bytes) |
| TODO | 0x800BADE0 | `audio_dsp_process` | DSP processing - reverb/EQ effects chain (1372 byt |
| TODO | 0x800BAE24 | `physics_forces` | force calculation (arcade: forces in drivsym.c:382 |
| TODO | 0x800BAF64 | `mode_flags_clear` | clear audio mode flags |
| TODO | 0x800BAF98 | `audio_output_setup` | configure audio output mode (408 bytes) |
| TODO | 0x800BB000 | `collision_detect` | main collision detection (arcade: collision) |
| TODO | 0x800BB044 | `audio_hw_sync` | sync audio buffer submission with hardware (392 by |
| TODO | 0x800BB140 | `audio_interrupt_handler` | audio DMA interrupt handler (756 bytes) |
| TODO | 0x800BB400 | `collision_car_car` | car-to-car collision (arcade: setFBCollisionForce) |
| TODO | 0x800BB69C | `camera_reset` | reset camera to default state (292 bytes) |
| TODO | 0x800BB800 | `collision_car_wall` | car-to-wall collision (arcade: ForceApart) |
| TODO | 0x800BB834 | `camera_lerp_position` | lerp camera position toward target (808 bytes) |
| TODO | 0x800BB9BC | `camera_orbit_delta` | orbit camera around target by delta angles (548 by |
| TODO | 0x800BBC00 | `collision_point_in_body` | point inside collision box test (arcade: PointInBo |
| TODO | 0x800BC1E8 | `player_flag_clear_process` | process player flag clear |
| TODO | 0x800BC2BC | `camera_dolly` | move camera along view direction/zoom (460 bytes) |
| TODO | 0x800BCBB8 | `camera_follow_target` | camera follows target with look-ahead (7640 bytes) |
| TODO | 0x800BCEE4 | `camera_collision_avoid` | prevent camera clipping through geometry (1596 byt |
| TODO | 0x800BD104 | `camera_smooth_follow` | smooth interpolation to follow position (872 bytes |
| TODO | 0x800BD2C8 | `race_setup_1` | race setup part 1 |
| TODO | 0x800BD2D0 | `camera_collision_check` | raycast collision check from target to camera (197 |
| TODO | 0x800BDAA8 | `camera_shake_start` | initiate camera shake effect (852 bytes) |
| TODO | 0x800BDDFC | `camera_shake_update` | update camera shake per frame (192 bytes) |
| TODO | 0x800BDEBC | `camera_zoom_fov` | adjust camera FOV for zoom effect (444 bytes) |
| TODO | 0x800BE078 | `camera_auto_follow` | smooth auto-follow with spring physics (1136 bytes |
| TODO | 0x800BE4B4 | `camera_cinematic_mode` | set up camera for cinematic sequences (936 bytes) |
| TODO | 0x800BE4F8 | `camera_cut_to` | instant camera cut to new position/target |
| TODO | 0x800BE7BC | `race_position_update` | update race position |
| TODO | 0x800BE9A0 | `music_fade` | music fade in/out over duration (388 bytes) |
| TODO | 0x800BE9E8 | `music_tempo_adjust` | adjust music playback tempo |
| TODO | 0x800BEAA0 | `race_setup_2` | race setup part 2 |
| TODO | 0x800BEE2C | `camera_blend_between` | blend between two camera states (924 bytes) |
| TODO | 0x800BF024 | `results_screen_update` | update results screen display |
| TODO | 0x800BF0A4 | `results_time_display` | display race time on results |
| TODO | 0x800BF148 | `leaderboard_update` | update leaderboard display |
| TODO | 0x800BF1C8 | `camera_fov_control` | set camera field of view (236 bytes) |
| TODO | 0x800BF2B8 | `camera_clip_planes` | set camera near/far clipping planes (220 bytes) |
| TODO | 0x800BF394 | `camera_aspect_ratio` | set camera aspect ratio (200 bytes) |
| TODO | 0x800BF45C | `camera_look_at_point` | orient camera to look at target point (988 bytes) |
| TODO | 0x800BF838 | `camera_first_person` | first person view from inside vehicle (948 bytes) |
| TODO | 0x800BFBEC | `camera_third_person` | third person chase camera behind vehicle (380 byte |
| TODO | 0x800BFD68 | `select_screen_update` | update selection screen state |
| TODO | 0x800BFD94 | `camera_top_down` | top-down overhead camera view (844 bytes) |
| TODO | 0x800C0000 | `CheckCPs` | main checkpoint detection per frame (arcade: check |
| TODO | 0x800C00E0 | `camera_free_look` | free camera controlled by player input |
| TODO | 0x800C0288 | `camera_path_follow` | follow predefined camera path with interpolation ( |
| TODO | 0x800C02A0 | `camera_replay_mode` | playback camera from recorded replay data |
| TODO | 0x800C04CC | `camera_track_spline` | moves camera along a spline path |
| TODO | 0x800C085C | `camera_build_view_matrix` | builds view matrix from camera state arcade:fmath. |
| TODO | 0x800C0AC0 | `camera_update` | main camera update - calls appropriate mode handle |
| TODO | 0x800C0AC4 | `camera_transition` | blend between two camera states over time (1736 by |
| TODO | 0x800C1188 | `camera_matrix_build` | build view matrix from camera state (876 bytes) |
| TODO | 0x800C14F4 | `camera_frustum_extract` | extract frustum planes from camera (768 bytes) |
| TODO | 0x800C1604 | `camera_process_input` | processes player input for camera control |
| TODO | 0x800C17F4 | `camera_viewport_setup` | set viewport for camera rendering (1224 bytes) |
| TODO | 0x800C1B8C | `camera_check_constraints` | enforces camera position constraints |
| TODO | 0x800C1CBC | `camera_split_screen_setup` | configure split-screen for multiplayer (2392 bytes |
| TODO | 0x800C2020 | `camera_debug_display` | shows camera debug information |
| TODO | 0x800C2228 | `camera_save_state` | saves camera state for replay or undo |
| TODO | 0x800C244C | `camera_restore_state` | restores camera state from saved data |
| TODO | 0x800C26E0 | `camera_multi_view_setup` | sets up camera for split screen rendering |
| TODO | 0x800C2960 | `camera_split_screen_config` | configures split screen mode for multiplayer |
| TODO | 0x800C2BE0 | `camera_scene_manager` | manages camera state transitions between game stat |
| TODO | 0x800C3594 | `effect_slot_init` | initialize effect slot with index |
| TODO | 0x800C3614 | `effect_system_init` | initialize effect system |
| TODO | 0x800C4200 | `camera_trigger_check` | check camera trigger zones (1232 bytes) |
| TODO | 0x800C46D0 | `camera_follow_path` | camera following path logic (3956 bytes) |
| TODO | 0x800C55E4 | `effect_cleanup` | cleanup effect slots |
| TODO | 0x800C5644 | `camera_play_script` | execute scripted camera sequence (3520 bytes) |
| TODO | 0x800C6404 | `camera_victory` | victory lap camera mode (1468 bytes) |
| TODO | 0x800C69C0 | `entity_iterate` | iterate all entities with callback (224 bytes) |
| TODO | 0x800C6AA0 | `entity_update` | full entity physics and state update (1564 bytes) |
| TODO | 0x800C70BC | `scene_cleanup_slots` | scene slot cleanup |
| TODO | 0x800C7110 | `draw_ui_element` | draw UI sprite element (id, x, y, w, h, alpha) |
| TODO | 0x800C7308 | `object_render_cleanup` | cleanup object render reference |
| TODO | 0x800C734C | `draw_text` | draw text string at x,y with color |
| TODO | 0x800C7350 | `hud_element_render` | render HUD element by ID |
| TODO | 0x800C760C | `draw_number` | draw numeric value at position (524 bytes) |
| TODO | 0x800C7818 | `draw_speedometer` | draw speedometer dial with needle (1724 bytes) |
| TODO | 0x800C7ED4 | `draw_lap_counter` | display current/total laps (2540 bytes) (arcade: c |
| TODO | 0x800C84C0 | `player_mode_set` | set player display mode |
| TODO | 0x800C84FC | `hud_speed_display` | render speedometer with speed value |
| TODO | 0x800C85F0 | `checkpoint_check` | checkpoint collision/plane test (arcade: checkpoin |
| TODO | 0x800C878C | `object_type7_create` | create object type 7 |
| TODO | 0x800C87F4 | `object_type1_create` | create object type 1 |
| TODO | 0x800C8864 | `draw_position` | show race position (1st, 2nd, etc.) (188 bytes) |
| TODO | 0x800C8A00 | `track_logic` | track handling logic |
| TODO | 0x800C8B8C | `hud_setup` | HUD initialization with positioning and parameters |
| TODO | 0x800C8DA0 | `hud_tachometer_update` | update tachometer display |
| TODO | 0x800C8E50 | `hud_nitro_update` | update nitro gauge |
| TODO | 0x800C8F00 | `hud_full_update` | full HUD refresh |
| TODO | 0x800C8F80 | `hud_fade_effect` | HUD fade/transition effect |
| TODO | 0x800C8FA4 | `display_enable` | enable/disable display output |
| TODO | 0x800C90E0 | `player_cleanup_slots` | cleanup player mode slots |
| TODO | 0x800C9158 | `player_state_set` | set player UI state |
| TODO | 0x800C9194 | `sync_entry_register` | create and register sync entry for menu state |
| TODO | 0x800C9210 | `speed_set` | set menu animation speed |
| TODO | 0x800C92DC | `speed_mode0_wrapper` | speed mode 0 wrapper |
| TODO | 0x800C9334 | `resource_type_select` | select resource type for menu |
| TODO | 0x800C9334 | `resource_slot_clear` | clear a resource slot by ID |
| TODO | 0x800C937C | `resource_slots_clear_multiple` | clear multiple resource slots |
| TODO | 0x800C93AC | `speed_mode1_wrapper` | speed mode 1 wrapper |
| TODO | 0x800C9404 | `draw_minimap` | draw track minimap with car positions (300 bytes) |
| TODO | 0x800C9528 | `input_init_flag_get` | load input initialized flag (8 bytes) - prologue f |
| TODO | 0x800C9530 | `input_callback_process` | process input callback table |
| TODO | 0x800C95DC | `controller_poll` | per-frame pad poll/state clear (928 bytes) |
| TODO | 0x800C997C | `process_inputs` | ProcessPDUs equivalent or state switch - ProcessPD |
| TODO | 0x800C997C | `process_inputs` | ProcessPDUs equivalent or state switch - game loop |
| TODO | 0x800C9AE0 | `game_mode_handler` | - in_game_mode handling (arcade: game.c:620) |
| TODO | 0x800C9BE0 | `init_state_begin` | called from GSTATE_INIT |
| TODO | 0x800C9BE0 | `session_flags_init` | arcade:money.c:init_credits N64 session/flags init |
| TODO | 0x800CA300 | `state_change_preprocess` | called when gstate != gstate_prev |
| TODO | 0x800CA300 | `state_change_preprocess` | state change preprocessing (HUD fade) |
| TODO | 0x800CA308 | `draw_message` | draw centered HUD message (checkpoint, etc.) (172  |
| TODO | 0x800CA3B4 | `playgame_state_change` | walks global mode flags, handles enter/exit gamepl |
| TODO | 0x800CADA4 | `hud_render` | main HUD rendering function (2468 bytes) |
| TODO | 0x800CB400 | `menu_process_input` | handle controller input for menus (628 bytes) |
| TODO | 0x800CB680 | `menu_cursor_move` | navigate between menu items (516 bytes) |
| TODO | 0x800CB9A0 | `menu_render_list` | render menu item list |
| TODO | 0x800CBBD4 | `menu_item_select` | execute selection on menu item (564 bytes) |
| TODO | 0x800CBE08 | `menu_back` | return to previous menu (132 bytes) |
| TODO | 0x800CBE8C | `menu_transition` | switch to new menu with animation (340 bytes) |
| TODO | 0x800CC000 | `menu_animation_update` | handle menu transitions and element animations (90 |
| TODO | 0x800CC3C0 | `sound_play_menu` | play menu sound effects (380 bytes) |
| TODO | 0x800CC540 | `menu_text_scroll` | scroll long text horizontally (904 bytes) |
| TODO | 0x800CC804 | `menu_item_value_get` | get menu item value |
| TODO | 0x800CC880 | `menu_highlight_set` | set highlight state for menu item (316 bytes) |
| TODO | 0x800CC9AC | `menu_list_render` | render entire menu with items (1112 bytes) |
| TODO | 0x800CD300 | `menu_confirm_render` | render confirmation dialog (580 bytes) |
| TODO | 0x800CD544 | `menu_confirm_dialog` | show confirmation and handle input (412 bytes) |
| TODO | 0x800CD6E0 | `menu_dialog_close` | close active dialog with animation (184 bytes) |
| TODO | 0x800CD738 | `keyboard_init` | initialize on-screen keyboard (340 bytes) |
| TODO | 0x800CD748 | `object_data_allocate` | allocate object data |
| TODO | 0x800CD88C | `keyboard_input_process` | handle keyboard navigation and selection (500 byte |
| TODO | 0x800CDA90 | `object_byte71_set_sync` | set object byte 71 with sync |
| TODO | 0x800CDAE0 | `menu_text_input` | handle full text entry with keyboard (460 bytes) |
| TODO | 0x800CDCAC | `menu_option_toggle` | toggle game option and apply immediately (576 byte |
| TODO | 0x800CDEEC | `menu_save_options` | save game options to Controller Pak (764 bytes) |
| TODO | 0x800CE1EC | `menu_load_options` | load game options from Controller Pak (364 bytes) |
| TODO | 0x800CE358 | `menu_options_screen` | main options menu handler (2532 bytes) |
| TODO | 0x800CED3C | `menu_audio_settings` | audio options submenu (1364 bytes) |
| TODO | 0x800CF000 | `menu_system_update` | main menu system update (large, ~12KB) |
| TODO | 0x800CF290 | `menu_video_settings` | video options submenu (228 bytes) |
| TODO | 0x800CF374 | `menu_control_settings` | controller options submenu (808 bytes) |
| TODO | 0x800CF69C | `menu_controller_remap` | controller button remapping screen (1976 bytes) |
| TODO | 0x800CFE74 | `menu_vibration_test` | test controller rumble feature (404 bytes) |
| TODO | 0x800D0000 | `menu_render` | render active menu screen |
| TODO | 0x800D000C | `track_select_handler` | arcade:TrackSel() - track selection menu (2264 byt |
| TODO | 0x800D000C | `track_select_screen` | main track selection menu (2264 bytes) |
| TODO | 0x800D000C | `track_select_screen` | track select screen - main track selection menu (2 |
| TODO | 0x800D03DC | `sound_position_set` | set 3D sound position (72 bytes) - applies Y/X/Z c |
| TODO | 0x800D08E4 | `track_preview_handler` | arcade:ShowTrackSelect - renders track thumbnail ( |
| TODO | 0x800D08E4 | `track_preview_render` | renders track thumbnail/preview image (692 bytes) |
| TODO | 0x800D08E4 | `track_preview_render` | track preview render - renders track thumbnail (69 |
| TODO | 0x800D0BA0 | `track_info_handler` | arcade:track stats display (1192 bytes) |
| TODO | 0x800D0BA0 | `track_info_display` | shows track statistics and info (1192 bytes) |
| TODO | 0x800D0BA0 | `track_info_display` | track info display - shows track statistics (1192  |
| TODO | 0x800D1000 | `menu_input_handle` | process menu controller input |
| TODO | 0x800D138C | `car_select_handler` | arcade:CarSel/init_car_select (804 bytes) |
| TODO | 0x800D138C | `car_select_screen` | main car selection menu (804 bytes) |
| TODO | 0x800D138C | `track_unlock_check` | track unlock check - checks if track is unlocked ( |
| TODO | 0x800D16B0 | `car_preview_handler` | arcade:AnimateCarSel - rotating 3D car model (564  |
| TODO | 0x800D16B0 | `car_preview_render` | renders rotating 3D car model (564 bytes) |
| TODO | 0x800D16B0 | `car_select_screen` | car select screen - car selection menu (564 bytes) |
| TODO | 0x800D18E4 | `car_stats_handler` | arcade:car performance stats (152 bytes) |
| TODO | 0x800D18E4 | `car_stats_display` | shows car performance stats (152 bytes) |
| TODO | 0x800D18E4 | `car_preview_render` | car preview render - 3D car model preview (152 byt |
| TODO | 0x800D197C | `car_unlock_handler` | arcade:car unlock check (316 bytes) |
| TODO | 0x800D197C | `car_unlock_check` | checks if car is unlocked (316 bytes) |
| TODO | 0x800D197C | `car_stats_display` | car stats display - speed/accel/handling bars (316 |
| TODO | 0x800D1AB8 | `car_color_handler` | arcade:car color selection (552 bytes) |
| TODO | 0x800D1AB8 | `car_color_select` | applies selected color to car preview (552 bytes) |
| TODO | 0x800D1AB8 | `car_color_select` | car color select - car paint color picker (552 byt |
| TODO | 0x800D1CE0 | `race_setup_screen` | configure race options before starting (1960 bytes |
| TODO | 0x800D1CE0 | `car_setup_confirm` | car setup confirm - finalize car selection (1960 b |
| TODO | 0x800D2000 | `menu_track_select` | track selection screen handler |
| TODO | 0x800D24C8 | `race_mode_select` | configures race mode settings (1120 bytes) |
| TODO | 0x800D24C8 | `race_countdown_display` | race countdown display - 3-2-1-GO countdown (1120  |
| TODO | 0x800D2928 | `lap_count_select` | sets number of laps for race (332 bytes) |
| TODO | 0x800D2928 | `race_position_display` | race position display - 1st/2nd/3rd etc (332 bytes |
| TODO | 0x800D2A74 | `lap_counter_display` | lap X/Y display (440 bytes) (arcade: checkpoint.c: |
| TODO | 0x800D2A74 | `difficulty_select` | sets AI difficulty and rubber-banding (440 bytes) |
| TODO | 0x800D2C2C | `mirror_mode_toggle` | flips track left-right (176 bytes) |
| TODO | 0x800D2CDC | `split_time_display` | checkpoint split times (240 bytes) (arcade: checkp |
| TODO | 0x800D2CDC | `weather_select` | sets weather conditions (240 bytes) |
| TODO | 0x800D2DCC | `time_of_day_select` | sets lighting conditions (200 bytes) |
| TODO | 0x800D2E94 | `multiplayer_setup` | configures multiplayer game options (1544 bytes) |
| TODO | 0x800D2E94 | `minimap_render` | minimap render - track minimap with car positions  |
| TODO | 0x800D3000 | `menu_car_select` | car selection screen handler |
| TODO | 0x800D349C | `player_join_screen` | players press start to join (1676 bytes) |
| TODO | 0x800D349C | `race_message_display` | race message display - wrong way/final lap etc (16 |
| TODO | 0x800D3B28 | `stunt_mode_setup` | configure stunt arena and scoring (5068 bytes) |
| TODO | 0x800D3B28 | `stunt_combo_display` | stunt combo display - current combo/multiplier (50 |
| TODO | 0x800D3E50 | `stunt_arena_preview` | render stunt arena preview (384 bytes) |
| TODO | 0x800D3E50 | `stunt_score_popup` | stunt score popup - +500 BARREL ROLL etc (384 byte |
| TODO | 0x800D4000 | `options_menu_handler` | game options menu |
| TODO | 0x800D4000 | `menu_options` | options menu handler |
| TODO | 0x800D4EF4 | `battle_mode_setup` | configure battle arena and rules (532 bytes) |
| TODO | 0x800D4EF4 | `stunt_total_display` | stunt total display - total stunt points (532 byte |
| TODO | 0x800D5000 | `menu_saveload` | save/load menu handler |
| TODO | 0x800D5050 | `players_race_update` | update player race progress |
| TODO | 0x800D5050 | `players_race_update` | players race update - per-frame player state (1116 |
| TODO | 0x800D510C | `ghost_race_setup` | race against saved ghosts setup (716 bytes) |
| TODO | 0x800D510C | `physics_init` | physics init - initialize physics state (716 bytes |
| TODO | 0x800D52D4 | `object_activate` | activate game object |
| TODO | 0x800D52D4 | `object_activate` | object activate - activate game object (436 bytes) |
| TODO | 0x800D54BC | `player_conditional_call` | conditional function call helper |
| TODO | 0x800D54E0 | `player_conditional_check` | conditional check based on parameter |
| TODO | 0x800D5798 | `players_frame_update` | per-frame player update |
| TODO | 0x800D5798 | `players_frame_update` | players frame update - all players tick (740 bytes |
| TODO | 0x800D5894 | `player_state_clear` | clear player state |
| TODO | 0x800D5894 | `player_state_clear` | player state clear - reset player state (224 bytes |
| TODO | 0x800D58CC | `records_screen` | displays best times and high scores (740 bytes) |
| TODO | 0x800D58CC | `stat_records_screen` | arcade:stats.c records display (same as records_sc |
| TODO | 0x800D5BB0 | `best_times_display` | shows best lap and race times (224 bytes) |
| TODO | 0x800D5BB0 | `player_finish_race` | player finish race - handle race finish (224 bytes |
| TODO | 0x800D5BB0 | `stat_best_times_show` | arcade:stats.c best times display (same as best_ti |
| TODO | 0x800D5C90 | `high_scores_display` | shows stunt high scores (220 bytes) |
| TODO | 0x800D5C90 | `player_respawn` | player respawn - respawn after crash (220 bytes) |
| TODO | 0x800D6000 | `menu_controller_config` | controller config menu handler |
| TODO | 0x800D60B4 | `players_finish_check` | check if players finished race |
| TODO | 0x800D60B4 | `players_finish_check` | players finish check - check all finished (676 byt |
| TODO | 0x800D6290 | `physics_init_mode0` | physics initialization mode 0 |
| TODO | 0x800D6290 | `physics_init_mode0` | physics init mode 0 - standard physics (92 bytes) |
| TODO | 0x800D62EC | `physics_init_mode1` | physics initialization mode 1 (passes t0=1) |
| TODO | 0x800D62EC | `physics_init_mode1` | physics init mode 1 - alternate physics (92 bytes) |
| TODO | 0x800D63C4 | `object_action_clear` | clear object action state |
| TODO | 0x800D63C4 | `object_action_clear` | object action clear - clear object actions (676 by |
| TODO | 0x800D63F4 | `achievements_screen` | shows unlocked achievements (676 bytes) |
| TODO | 0x800D63F4 | `object_action_process` | object action process - process pending actions (6 |
| TODO | 0x800D6698 | `credits_screen` | displays game credits (296 bytes) |
| TODO | 0x800D6698 | `drone_lap_update` | drone lap update - update drone lap counter (296 b |
| TODO | 0x800D67C0 | `credits_scroll` | scrolls credits text (348 bytes) |
| TODO | 0x800D67C0 | `drone_position_calc` | drone position calc - calculate drone track positi |
| TODO | 0x800D691C | `loading_screen` | displays loading progress bar (1376 bytes) |
| TODO | 0x800D691C | `drone_speed_adjust` | drone speed adjust - rubberbanding speed adjustmen |
| TODO | 0x800D6E7C | `loading_tips` | shows random gameplay tip during loading (852 byte |
| TODO | 0x800D6E7C | `drone_collision_avoid` | drone collision avoid - avoid other cars/obstacles |
| TODO | 0x800D7000 | `menu_prev` | navigate to previous menu item |
| TODO | 0x800D7100 | `menu_confirm` | confirm current menu selection |
| TODO | 0x800D71D0 | `pause_menu` | full pause menu with options (3256 bytes) |
| TODO | 0x800D71D0 | `drone_pathfind_main` | drone pathfind main - main pathfinding routine (32 |
| TODO | 0x800D7200 | `menu_toggle` | toggle menu option value |
| TODO | 0x800D7300 | `menu_audio_options` | audio options sub-menu |
| TODO | 0x800D7400 | `menu_credits` | credits screen handler |
| TODO | 0x800D7500 | `menu_load` | load menu data/resources |
| TODO | 0x800D7600 | `menu_stub_empty` | empty menu stub function |
| TODO | 0x800D7700 | `menu_main` | main menu entry point |
| TODO | 0x800D7800 | `menu_state_dispatch` | menu state machine dispatcher |
| TODO | 0x800D7D40 | `render_replay_ui` | render replay interface |
| TODO | 0x800D7DC4 | `render_results_screen` | render race results screen |
| TODO | 0x800D7E88 | `pause_resume` | resumes game from pause (788 bytes) |
| TODO | 0x800D7E88 | `drone_target_update` | drone target update - update target waypoint (788  |
| TODO | 0x800D8000 | `replay_ui_draw` | draw replay interface elements |
| TODO | 0x800D8184 | `pause_restart` | restarts current race (2772 bytes) |
| TODO | 0x800D8184 | `drone_steering_calc` | drone steering calc - calculate steering angle (27 |
| TODO | 0x800D8C58 | `pause_quit` | quits to main menu (1032 bytes) |
| TODO | 0x800D8C58 | `drone_throttle_calc` | drone throttle calc - calculate throttle/brake (10 |
| TODO | 0x800D9060 | `results_screen` | displays race results (4204 bytes) |
| TODO | 0x800D9060 | `results_screen` | results screen - race results display (4204 bytes) |
| TODO | 0x800D9060 | `stat_results_display` | arcade:stats.c:stat_endgame results screen (same a |
| TODO | 0x800DA0CC | `position_result_display` | displays finishing position (168 bytes) |
| TODO | 0x800DA0CC | `position_result_display` | position result display - 1ST/2ND/3RD text (168 by |
| TODO | 0x800DA0CC | `stat_finish_position` | arcade:stats.c:stat_endgame finish position record |
| TODO | 0x800DA174 | `time_result_display` | displays race time (348 bytes) |
| TODO | 0x800DA174 | `time_result_display` | time result display - race time mm:ss.xx (348 byte |
| TODO | 0x800DA2D0 | `points_award` | calculates and displays points earned (2316 bytes) |
| TODO | 0x800DA2D0 | `points_award` | points award - calculate and display points (2316  |
| TODO | 0x800DA2D0 | `tournament_points_calc` | arcade:tourney.c - calculate tournament points (23 |
| TODO | 0x800DABDC | `replay_save_prompt` | asks user to save replay (2940 bytes) |
| TODO | 0x800DABDC | `replay_save_prompt` | replay save prompt - save replay dialog (2940 byte |
| TODO | 0x800DB758 | `continue_prompt` | quick continue/quit choice (196 bytes) |
| TODO | 0x800DB758 | `continue_prompt` | continue prompt - continue/quit choice (196 bytes) |
| TODO | 0x800DB7B4 | `object_type7_create_alt` | create object type 7 alternative |
| TODO | 0x800DB7B4 | `object_type7_create_alt` | object type 7 create - alternate object create (92 |
| TODO | 0x800DB81C | `RaceStateMachine_Update` | size:2148 - jump-table state machine on 0x801170FC |
| TODO | 0x800DB81C | `attract_mode` | demo/title screen handler (arcade: game/game.c:att |
| TODO | 0x800DB81C | `race_state_update` | race progression/checkpoint/finalization controlle |
| TODO | 0x800DB81C | `attract_handler` | attract handler - attract mode state machine (196  |
| TODO | 0x800DBFC0 | `hiscore_check_handler` | arcade:CheckHighScore - check if score qualifies |
| TODO | 0x800DC000 | `hiscore_display_handler` | arcade:ShowHighScores - display high score table |
| TODO | 0x800DC100 | `hiscore_insert_handler` | arcade:InsertHighScore - insert new entry |
| TODO | 0x800DC100 | `tournament_init` | arcade:tourney.c:InitTourney - init tournament sta |
| TODO | 0x800DC200 | `tournament_is_active` | arcade:tourney.c:TourneyOn - check if tournament m |
| TODO | 0x800DC220 | `tournament_parse_flags` | arcade:tourney.c:ParseTourneyFlags - parse tournam |
| TODO | 0x800DC248 | `championship_standings` | shows championship points (432 bytes) |
| TODO | 0x800DC248 | `championship_standings` | championship standings - points leaderboard (432 b |
| TODO | 0x800DC248 | `tournament_standings_display` | arcade:tourney.c - championship standings display  |
| TODO | 0x800DC3F8 | `trophy_award_championship` | awards championship trophy (924 bytes) |
| TODO | 0x800DC3F8 | `trophy_award_championship` | trophy award - championship trophy display (924 by |
| TODO | 0x800DC3F8 | `tournament_trophy_award` | arcade:tourney.c - award championship trophy (924  |
| TODO | 0x800DC720 | `sound_handles_array_clear` | clear sound handle array |
| TODO | 0x800DC720 | `sound_handles_array_clear` | sound handles clear - reset sound handles (248 byt |
| TODO | 0x800DC794 | `unlock_notification` | shows unlock popup (248 bytes) |
| TODO | 0x800DC794 | `unlock_notification` | unlock notification - unlock popup display (248 by |
| TODO | 0x800DC794 | `tournament_unlock_check` | arcade:tourney.c unlock progression - show unlock  |
| TODO | 0x800DC88C | `attract_mode_handler` | arcade:attract() - attract mode state machine (127 |
| TODO | 0x800DC88C | `attract_mode_start` | initializes attract mode demo (1272 bytes) |
| TODO | 0x800DC88C | `unlock_fanfare` | unlock fanfare - unlock celebration effect (1272 b |
| TODO | 0x800DC99C | `attract_demo_handler` | arcade:play_demogame - demo race playback (1016 by |
| TODO | 0x800DC99C | `attract_demo_play` | plays back recorded demo race (1016 bytes) |
| TODO | 0x800DC99C | `stats_update` | stats update - update game statistics (1016 bytes) |
| TODO | 0x800DC99C | `stat_game_update` | arcade:stats.c:stat_* per-state update (same as st |
| TODO | 0x800DCD1C | `mode_enable_flagged` | enable mode with flag check |
| TODO | 0x800DCD1C | `mode_enable_flagged` | mode enable flagged - enable mode with flags (96 b |
| TODO | 0x800DCD94 | `attract_idle_handler` | arcade:TimeOut check (96 bytes) |
| TODO | 0x800DCD94 | `attract_idle_check` | returns 1 if no input for idle timeout (96 bytes) |
| TODO | 0x800DCD94 | `mode_enable_direct` | mode enable direct - direct mode enable (96 bytes) |
| TODO | 0x800DCD94 | `start_button_check` | arcade:money.c:chk_start N64 start/input check (at |
| TODO | 0x800DCDF4 | `attract_video_handler` | arcade:AttractMovie - video playback (732 bytes) |
| TODO | 0x800DCDF4 | `attract_video_play` | plays FMV or pre-rendered video (732 bytes) |
| TODO | 0x800DCDF4 | `options_menu` | options menu - game options screen (732 bytes) |
| TODO | 0x800DD0D0 | `attract_sequence_handler` | arcade:attract screen sequencing (988 bytes) |
| TODO | 0x800DD0D0 | `attract_sequence_update` | updates attract mode state machine (988 bytes) |
| TODO | 0x800DD0D0 | `audio_settings` | audio settings - music/sfx volume (988 bytes) |
| TODO | 0x800DD4AC | `title_screen_handler` | arcade:ShowLogo/ShowJoin (2816 bytes) |
| TODO | 0x800DD4AC | `title_screen` | main title screen display (2816 bytes) |
| TODO | 0x800DD4AC | `control_settings` | control settings - button mapping (2816 bytes) |
| TODO | 0x800DDFAC | `title_logo_handler` | arcade:AnimateLogo (608 bytes) |
| TODO | 0x800DDFAC | `title_logo_animate` | animates the game logo (608 bytes) |
| TODO | 0x800DDFAC | `display_settings` | display settings - video options (608 bytes) |
| TODO | 0x800DE20C | `title_prompt_handler` | arcade:AddJoinNow - PRESS START prompt (724 bytes) |
| TODO | 0x800DE20C | `title_button_prompt` | shows PRESS START with blinking (724 bytes) |
| TODO | 0x800DE20C | `save_settings` | save settings - save to controller pak (724 bytes) |
| TODO | 0x800DE20C | `tournament_save_settings` | arcade:tourney.c:SaveTourneySettings - save tourna |
| TODO | 0x800DE4DC | `title_background_handler` | arcade:title background effects (908 bytes) |
| TODO | 0x800DE4DC | `title_background` | renders animated title background (908 bytes) |
| TODO | 0x800DE4DC | `load_settings` | load settings - load from controller pak (908 byte |
| TODO | 0x800DE4DC | `tournament_load_settings` | arcade:tourney.c:LoadTourneySettings - load tourna |
| TODO | 0x800DE868 | `main_menu_handler` | main menu screen (836 bytes) |
| TODO | 0x800DE868 | `main_menu_screen` | handles main menu logic (836 bytes) |
| TODO | 0x800DE868 | `settings_confirm` | settings confirm - confirm changes (836 bytes) |
| TODO | 0x800DEBAC | `main_menu_input_handler` | main menu input processing (224 bytes) |
| TODO | 0x800DEBAC | `main_menu_input` | processes main menu input (224 bytes) |
| TODO | 0x800DEBAC | `settings_cancel` | settings cancel - discard changes (224 bytes) |
| TODO | 0x800DEC8C | `main_menu_render_handler` | main menu rendering (732 bytes) |
| TODO | 0x800DEC8C | `main_menu_render` | draws the main menu (732 bytes) |
| TODO | 0x800DEC8C | `settings_default` | settings default - reset to defaults (732 bytes) |
| TODO | 0x800DEF68 | `mode_select_handler` | race/battle/stunt mode select (2976 bytes) |
| TODO | 0x800DEF68 | `mode_select_screen` | race/battle/stunt mode configuration (2976 bytes) |
| TODO | 0x800DEF68 | `mode_select_screen` | mode select screen - race/stunt/battle mode (2976  |
| TODO | 0x800DFB08 | `mode_select_input_handler` | mode selection input (188 bytes) |
| TODO | 0x800DFB08 | `mode_select_input` | handles mode selection input (188 bytes) |
| TODO | 0x800DFB08 | `mode_select_update` | mode select update - mode selection handler (188 b |
| TODO | 0x800DFBC4 | `profile_select_handler` | profile selection (1868 bytes) |
| TODO | 0x800DFBC4 | `profile_select_screen` | profile select screen (1868 bytes) |
| TODO | 0x800DFBC4 | `profile_select_screen` | profile select screen - player profile (1868 bytes |
| TODO | 0x800E051C | `cursor_move` | keyboard cursor move (212 bytes) |
| TODO | 0x800E0A00 | `menu_render_background` | menu render background - draw menu background |
| TODO | 0x800E0C20 | `menu_render_title` | menu render title - draw menu title text |
| TODO | 0x800E0E80 | `menu_render_options` | menu render options - draw menu option list |
| TODO | 0x800E1180 | `menu_cursor_update` | menu cursor update - update cursor position |
| TODO | 0x800E1400 | `menu_input_process` | menu input process - process menu input |
| TODO | 0x800E1680 | `menu_transition_start` | menu transition start - start menu transition |
| TODO | 0x800E1880 | `menu_transition_update` | menu transition update - update transition anim |
| TODO | 0x800E1A00 | `menu_sound_play` | menu sound play - play menu sound effect |
| TODO | 0x800E2000 | `hud_speedometer_render` | hud speedometer render - draw speedometer |
| TODO | 0x800E2200 | `hud_tachometer_render` | hud tachometer render - draw tachometer |
| TODO | 0x800E2400 | `hud_lap_render` | hud lap render - draw lap counter |
| TODO | 0x800E2600 | `hud_position_render` | hud position render - draw race position |
| TODO | 0x800E2A00 | `hud_minimap_render` | hud minimap render - draw track minimap |
| TODO | 0x800E2A3C | `replay_update_dual` | update both replay UI and camera (40 bytes) - call |
| TODO | 0x800E2A64 | `object_update_full` | object update full |
| TODO | 0x800E2C00 | `hud_stunt_render` | hud stunt render - draw stunt score |
| TODO | 0x800E3000 | `camera_target_set` | camera target set - set camera follow target |
| TODO | 0x800E3200 | `camera_position_update` | camera position update - update camera position |
| TODO | 0x800E3400 | `camera_angle_update` | camera angle update - update camera angles |
| TODO | 0x800E3600 | `camera_shake_apply` | camera shake apply - apply screen shake |
| TODO | 0x800E3800 | `camera_mode_set` | camera mode set - set camera mode (chase/hood/etc) |
| TODO | 0x800E3A00 | `camera_replay_control` | camera replay control - replay camera control |
| TODO | 0x800E4000 | `race_lap_check` | check lap completion (arcade: checkpoint.c:1098 Pa |
| TODO | 0x800E4200 | `race_position_calc` | calculate race positions (arcade: checkpoint.c:940 |
| TODO | 0x800E4400 | `race_checkpoint_update` | update checkpoint state per frame (arcade: checkpo |
| TODO | 0x800E4600 | `race_finish_check` | check race finish condition (arcade: checkpoint.c: |
| TODO | 0x800E4800 | `race_time_update` | update race timer (arcade: checkpoint.c:last_check |
| TODO | 0x800E4A00 | `race_wrong_way_check` | detect wrong way driving (arcade: checkpoint.c:dir |
| TODO | 0x800E5000 | `stunt_detect` | stunt detect - detect stunt type from air state |
| TODO | 0x800E5200 | `stunt_score_calc` | stunt score calc - calculate stunt score |
| TODO | 0x800E5400 | `stunt_combo_update` | stunt combo update - update combo multiplier |
| TODO | 0x800E5600 | `stunt_landing_check` | stunt landing check - check safe landing |
| TODO | 0x800E5800 | `stunt_trick_register` | stunt trick register - register completed trick |
| TODO | 0x800E5A00 | `stunt_height_bonus` | stunt height bonus - calc height bonus |
| TODO | 0x800E6000 | `audio_engine_update` | audio engine update - update engine sound pitch |
| TODO | 0x800E6200 | `audio_skid_update` | audio skid update - update tire skid sound |
| TODO | 0x800E6400 | `audio_collision_play` | audio collision play - play collision sound |
| TODO | 0x800E6450 | `collision_rumble_trigger` | arcade:fswheel.c:crash_force N64 collision rumble  |
| TODO | 0x800E6480 | `landing_rumble_trigger` | arcade:fswheel.c:landing_force N64 landing rumble  |
| TODO | 0x800E6600 | `audio_ambient_update` | audio ambient update - update ambient sounds |
| TODO | 0x800E6800 | `audio_music_update` | audio music update - update music playback |
| TODO | 0x800E6A00 | `audio_position_update` | audio position update - update 3D sound positions |
| TODO | 0x800E7000 | `object_pool_init` | object pool init - initialize object pool |
| TODO | 0x800E7100 | `object_alloc_slot` | object alloc slot - allocate object slot |
| TODO | 0x800E7200 | `object_free_slot` | object free slot - free object slot |
| TODO | 0x800E7300 | `object_update_all` | object update all - update all active objects |
| TODO | 0x800E73E4 | `Input_PollPads` | calls osContGetReadData, reads raw buttons/stick,  |
| TODO | 0x800E73E4 | `input_controller_read` | input controller read - polls controller, processe |
| TODO | 0x800E7400 | `object_render_all` | object render all - render all visible objects |
| TODO | 0x800E7500 | `object_collision_all` | object collision all - check all collisions |
| TODO | 0x800E7580 | `input_button_check` | input button check - checks button state |
| TODO | 0x800E7690 | `input_analog_read` | input analog read - reads analog stick values |
| TODO | 0x800E7800 | `input_rumble_update` | input rumble update - updates controller rumble |
| TODO | 0x800E7900 | `input_deadzone_apply` | input deadzone apply - applies stick deadzone |
| TODO | 0x800E7914 | `object_counter_decrement` | decrement object reference counter (108 bytes) - s |
| TODO | 0x800E7980 | `object_counter_increment` | increment object reference counter (112 bytes) - s |
| TODO | 0x800E8000 | `world_load_segment` | world load segment - load world segment from ROM |
| TODO | 0x800E8200 | `world_unload_segment` | world unload segment - unload world segment |
| TODO | 0x800E8400 | `world_visibility_update` | world visibility update - update visible sectors |
| TODO | 0x800E8600 | `world_lod_update` | world lod update - update LOD levels |
| TODO | 0x800E8800 | `world_stream_update` | world stream update - update streaming state |
| TODO | 0x800E9000 | `collision_mesh_load` | collision mesh load - load collision mesh |
| TODO | 0x800E9200 | `collision_query_ray` | collision query ray - raycast against world |
| TODO | 0x800E9400 | `collision_query_sphere` | collision query sphere - sphere cast |
| TODO | 0x800E9600 | `collision_ground_height` | collision ground height - get ground height at poi |
| TODO | 0x800E9800 | `collision_surface_type` | collision surface type - get surface type at point |
| TODO | 0x800E9A00 | `collision_normal_get` | collision normal get - get surface normal |
| TODO | 0x800EA000 | `anim_play` | anim play - start animation |
| TODO | 0x800EA200 | `anim_stop` | anim stop - stop animation |
| TODO | 0x800EA400 | `anim_blend` | anim blend - blend between animations |
| TODO | 0x800EA600 | `anim_frame_update` | anim frame update - update animation frame |
| TODO | 0x800EA800 | `anim_event_check` | anim event check - check for anim events |
| TODO | 0x800EB000 | `model_load` | model load - load model from ROM |
| TODO | 0x800EB200 | `model_unload` | model unload - unload model |
| TODO | 0x800EB400 | `model_render` | model render - render model with transforms |
| TODO | 0x800EB500 | `model_lod_select` | model lod select - select model LOD level |
| TODO | 0x800EB690 | `world_object_destroy` | world object destroy (396 bytes) |
| TODO | 0x800EC0DC | `race_init_helper` | race initialization |
| TODO | 0x800EC2F8 | `world_physics_tick` | world physics tick - iterates cars and applies phy |
| TODO | 0x800EC928 | `world_collision_detect` | world collision detect - bounding sphere check (58 |
| TODO | 0x800ECB74 | `world_bounds_check` | world bounds check - position in bounds (164 bytes |
| TODO | 0x800ECC18 | `world_gravity_apply` | world gravity apply - applies gravity to velocity  |
| TODO | 0x800ED674 | `world_friction_apply` | world friction apply - applies drag to velocity (4 |
| TODO | 0x800ED764 | `mode_byte_set` | mode byte set |
| TODO | 0x800ED7B4 | `mode_byte2_set` | mode byte 2 set |
| TODO | 0x800ED804 | `world_velocity_integrate` | world velocity integrate - Euler integration (712  |
| TODO | 0x800EDACC | `world_collision_response` | world collision response - impulse-based elastic c |
| TODO | 0x800EDCE8 | `world_trigger_check` | check player in trigger volumes (2292 bytes) (arca |
| TODO | 0x800EDDC0 | `attract_or_transition` | attract mode/state transition - attract mode/state |
| TODO | 0x800EDDC0 | `game_mode_changed` | called on state transition |
| TODO | 0x800EDDC0 | `session_start_handler` | arcade:money.c:chk_start->true game start transiti |
| TODO | 0x800EE5DC | `world_trigger_activate` | activate checkpoint/shortcut/boost trigger (580 by |
| TODO | 0x800EE7C4 | `sync_init_conditional` | sync init conditional |
| TODO | 0x800EE820 | `effect_spawn` | effect spawn - create visual effect at position |
| TODO | 0x800EE88C | `collision_check_thunk` | collision check thunk (456 bytes) |
| TODO | 0x800EE8B4 | `world_effect_update` | world effect update - updates particle position/li |
| TODO | 0x800EEA50 | `emitter_create` | emitter create - create particle emitter |
| TODO | 0x800EEA7C | `game_late_init` | late-stage game init (called after decompression) |
| TODO | 0x800EEDB0 | `particle_update` | particle update - updates emitter particles (1240  |
| TODO | 0x800EEDB0 | `emitter_update` | emitter update - update emitter particles |
| TODO | 0x800EF200 | `smoke_effect` | smoke effect - smoke particle effect |
| TODO | 0x800EF3A0 | `spark_effect` | spark effect - spark particle effect |
| TODO | 0x800EF500 | `fire_effect` | fire effect - fire particle effect |
| TODO | 0x800EF8F4 | `explosion_effect` | explosion effect - creates fireball/smoke/sparks ( |
| TODO | 0x800EFEA8 | `dust_cloud_effect` | dust cloud effect - creates settling dust particle |
| TODO | 0x800F0100 | `skid_mark_render` | skid mark render - tire skid decals on track (1396 |
| TODO | 0x800F0698 | `net_message_recv` | arcade:pdu.c:ParseEntityStatePDU/ParseEventReportP |
| TODO | 0x800F0C18 | `net_state_sync` | arcade:pdu.c:PutEntityStatePDU - serialize car sta |
| TODO | 0x800F13F0 | `net_lobby_update` | network lobby update - handle lobby/player managem |
| TODO | 0x800F1944 | `net_player_update` | arcade:pdu.c:EntityStatePDU handling - update remo |
| TODO | 0x800F1C98 | `net_latency_check` | arcade:pdu.c timing - N64 frame sync (676 bytes) |
| TODO | 0x800F1F3C | `net_input_sync` | arcade:pdu.c:EntityStatePDU inputs - input state s |
| TODO | 0x800F20A0 | `net_game_tick` | arcade:pdu.c main loop - per-frame net update (166 |
| TODO | 0x800F2720 | `net_packet_send` | arcade:pdu.c:SendPDU - low-level packet send (376  |
| TODO | 0x800F27DC | `net_connection_update` | network connection update (1632 bytes) |
| TODO | 0x800F2890 | `net_packet_recv` | arcade:pdu.c:GetPDU - low-level packet receive (40 |
| TODO | 0x800F2A28 | `net_state_validate` | arcade:pdu.c:RTEST/LTEST - validate state data bou |
| TODO | 0x800F2E3C | `net_error_handle` | arcade:pdu.c:ReportPDUError - error handling (784  |
| TODO | 0x800F314C | `net_session_init` | network session init (1084 bytes) |
| TODO | 0x800F34D8 | `net_session_update` | network session update (3576 bytes) |
| TODO | 0x800F3588 | `net_player_join_handler` | arcade:pdu.c:AcknowledgePDU handling - player join |
| TODO | 0x800F38BC | `net_game_state_send` | arcade:pdu.c:PutEventReportPDU - send game events  |
| TODO | 0x800F4034 | `net_race_sync` | network race sync (1536 bytes) |
| TODO | 0x800F42D0 | `lod_select` | LOD level selection based on distance (260 bytes) |
| TODO | 0x800F43D4 | `lod_object_detail` | LOD object detail level (560 bytes) |
| TODO | 0x800F4604 | `physics_update_all` | large physics update routine |
| TODO | 0x800F4604 | `track_geometry_stream` | track geometry stream - streams from ROM to RSP (6 |
| TODO | 0x800F4634 | `track_vertex_load` | track vertex load (908 bytes) |
| TODO | 0x800F497C | `graphics_chunk` | graphics routine |
| TODO | 0x800F49C0 | `track_section_render` | track section render (876 bytes) |
| TODO | 0x800F4B8C | `graphics_chunk_b` | graphics routine |
| TODO | 0x800F4D2C | `track_surface_setup` | track surface setup (752 bytes) |
| TODO | 0x800F4D94 | `audio_update_a` | audio update routine |
| TODO | 0x800F4FEC | `assign_drones` | assign drone tasks and speed adjustment (arcade: d |
| TODO | 0x800F5000 | `track_render_main` | track render main - large rendering (3832 bytes) |
| TODO | 0x800F54C0 | `place_cars_in_order` | sort cars by race position (arcade: drones.c:place |
| TODO | 0x800F5EF8 | `track_lighting_setup` | track lighting setup (588 bytes) |
| TODO | 0x800F5F90 | `track_texture_load` | track texture load - loads texture to TMEM (1348 b |
| TODO | 0x800F6144 | `track_culling_update` | track culling update (1888 bytes) |
| TODO | 0x800F6310 | `track_render_alt` | track render alternate |
| TODO | 0x800F64D4 | `finish_state_normal` | normal finish handler |
| TODO | 0x800F64D4 | `billboard_render` | billboard render - camera-facing sprite (1120 byte |
| TODO | 0x800F6894 | `sign_render` | sign render - roadside signs (580 bytes) |
| TODO | 0x800F6934 | `sign_render_main` | sign render main (388 bytes) |
| TODO | 0x800F6AB8 | `props_render` | props render - track props (cones/barriers/trees)  |
| TODO | 0x800F6AD8 | `prop_instance_render` | prop instance render (740 bytes) |
| TODO | 0x800F6DBC | `prop_batch_render` | prop batch render (1168 bytes) |
| TODO | 0x800F724C | `prop_visibility_check` | prop visibility check (252 bytes) |
| TODO | 0x800F733C | `update_active_objects` | per-frame scene object dispatcher (accesses 0x8015 |
| TODO | 0x800F733C | `race_per_frame_update` | called in race loop |
| TODO | 0x800F733C | `UpdateActiveObjects` | - loads object count at 0x80159788, iterates array |
| TODO | 0x800F7344 | `graphics_chunk_c` | graphics routine |
| TODO | 0x800F73FC | `audio_update_b` | audio update routine |
| TODO | 0x800F7454 | `physics_post_update` | physics post-update routine |
| TODO | 0x800F7454 | `render_pipeline_main` | render pipeline main (1996 bytes) |
| TODO | 0x800F7C28 | `linear_interp` | linear interpolation for drone speeds (arcade: dro |
| TODO | 0x800F7E70 | `audio_update_c` | audio update routine |
| TODO | 0x800F7E70 | `memory_regions_clear` | memory regions clear |
| TODO | 0x800F7F44 | `graphics_chunk_d` | graphics routine |
| TODO | 0x800F8588 | `update_mpath_index` | update maxpath index for car (arcade: maxpath.c:up |
| TODO | 0x800F8754 | `audio_update_d` | audio update routine |
| TODO | 0x800F87AC | `find_maxpath_intervals` | find closest maxpath intervals (arcade: maxpath.c: |
| TODO | 0x800F8B70 | `finish_state_alt` | alternate finish handler |
| TODO | 0x800F8D9C | `camera_race_setup` | camera setup for race |
| TODO | 0x800F93A0 | `render_large_objects` | render large objects (5652 bytes) |
| TODO | 0x800F9428 | `render_car_body` | render car body (1604 bytes) |
| TODO | 0x800F9A74 | `render_car_wheels` | render car wheels (952 bytes) |
| TODO | 0x800F9E2C | `render_car_shadow` | render car shadow (684 bytes) |
| TODO | 0x800FA0D8 | `render_scene_setup` | render scene setup (2356 bytes) |
| TODO | 0x800FA9B4 | `render_viewport_init` | render viewport init (948 bytes) |
| TODO | 0x800FA9E4 | `loading_screen_handler` | arcade:ShowTransit (1296 bytes) |
| TODO | 0x800FA9E4 | `render_framebuffer` | render framebuffer (1296 bytes) |
| TODO | 0x800FAD58 | `render_display_update` | render display update (136 bytes) |
| TODO | 0x800FADE0 | `render_post_process` | render post process (1108 bytes) |
| TODO | 0x800FAEF4 | `pause_menu_handler` | N64-specific pause menu (1808 bytes) |
| TODO | 0x800FAEF4 | `render_flush` | render flush (1808 bytes) |
| TODO | 0x800FAF6C | `init_state_continue` | called after init_state_begin |
| TODO | 0x800FB234 | `vsync_wait` | vsync wait - waits for vertical blank (148 bytes) |
| TODO | 0x800FB2C8 | `setup_state_main` | called from GSTATE_SETUP |
| TODO | 0x800FB2C8 | `display_list_flush` | display list flush - submits DL to RSP/RDP (5944 b |
| TODO | 0x800FB5F4 | `pause_toggle_handler` | pause state toggle (820 bytes) |
| TODO | 0x800FB5F4 | `hud_render_main` | HUD render main (820 bytes) |
| TODO | 0x800FBB00 | `hiscore_input_analog` | arcade:fswheel.c:WheelSection N64 hiscore analog s |
| TODO | 0x800FBBFC | `game_state_check_handler` | check game state for menu transitions |
| TODO | 0x800FBBFC | `game_state_check_handler` | game state check handler |
| TODO | 0x800FBC30 | `countdown_handler` | arcade:CountDown() - race countdown timer |
| TODO | 0x800FBC30 | `game_mode_post` | post-processing after state match |
| TODO | 0x800FBC30 | `steady_state_handler` | called when state unchanged |
| TODO | 0x800FBC30 | `preplay_setup` | pre-race setup |
| TODO | 0x800FBC38 | `game_mode_post` | game mode post-processing |
| TODO | 0x800FBC60 | `countdown_display_handler` | countdown number display |
| TODO | 0x800FBF88 | `state_transition` | conditional state transition |
| TODO | 0x800FBF88 | `hiscore_entry_handler` | arcade:hiscore/EnterHighScore (state_transition) |
| TODO | 0x800FBF88 | `countdown` | race countdown handler |
| TODO | 0x800FBF90 | `state_init_handler` | GSTATE_INIT (0x00040000) |
| TODO | 0x800FBFE4 | `state_setup_handler` | GSTATE_SETUP (0x00080000) |
| TODO | 0x800FBFE4 | `track_car_selection_setup` | State_0x80000_Handler - track/car selection setup |
| TODO | 0x800FC038 | `state_camera_handler` | GSTATE_CAMERA (0x00200000) |
| TODO | 0x800FC0EC | `state_menu_handler` | GSTATE_MENU handler (0x00100000) - GSTATE_MENU (0x |
| TODO | 0x800FC0EC | `ui_input_handler_block` | State_0x100000_Handler - UI/input handling block |
| TODO | 0x800FC0EC | `state_menu_handler` | GSTATE_MENU handler (UI/input chain) |
| TODO | 0x800FC228 | `state_ready_handler` | GSTATE_READY (0x00400000) |
| TODO | 0x800FC25C | `state_play_handler` | GSTATE_PLAY (0x00800000) |
| TODO | 0x800FC25C | `gameplay_menu_handler` | State_0x800000_Handler - gameplay/menu block |
| TODO | 0x800FC3D8 | `state_handler_alt` | state handler alternate (1516 bytes) |
| TODO | 0x800FC89C | `state_cars_handler` | GSTATE_CARS (0x01000000) |
| TODO | 0x800FC960 | `state_finish_handler` | GSTATE_FINISH (0x02000000) |
| TODO | 0x800FC9B8 | `state_transition_check` | state transition check (1284 bytes) |
| TODO | 0x800FCA00 | `debug_overlay` | debug overlay - FPS/frame counter display (1016 by |
| TODO | 0x800FCDF8 | `debug_stats` | debug stats - memory/performance stats (556 bytes) |
| TODO | 0x800FCEB0 | `state_update_loop` | state update loop (948 bytes) |
| TODO | 0x800FD024 | `debug_collision` | debug collision - collision wireframes (540 bytes) |
| TODO | 0x800FD238 | `state_dispatch` | state machine bitmask ladder |
| TODO | 0x800FD238 | `input_aux_handler` | fast-path for bits 0x7C03FFFE - fast-path for bits |
| TODO | 0x800FD240 | `debug_ai_paths` | debug AI paths - waypoint visualization (552 bytes |
| TODO | 0x800FD264 | `state_exit_handler` | state exit handler (512 bytes) |
| TODO | 0x800FD464 | `game_loop` | main per-frame game state machine (704 bytes) |
| TODO | 0x800FD464 | `game_loop` | main game state machine (arcade: game/game.c:game) |
| TODO | 0x800FD7E8 | `random_seed_init` | random seed init - sets RNG seed (244 bytes) |
| TODO | 0x800FD8DC | `random_int` | random int - returns random 32-bit integer (284 by |
| TODO | 0x800FD9F8 | `random_float` | random float - returns random float [0,1) (1436 by |
| TODO | 0x800FDA90 | `large_update_routine` | large update routine (4560 bytes) |
| TODO | 0x800FDF94 | `random_range` | random range - returns random int in [min,max] (24 |
| TODO | 0x800FE08C | `stat_race_start` | arcade:stats.c:stat_preplay race timing init (same |
| TODO | 0x800FE4BC | `stat_race_end` | arcade:stats.c:stat_endgame race completion (same  |
| TODO | 0x800FE5B0 | `stat_race_update` | arcade:stats.c:stat_playgame per-frame time update |
| TODO | 0x800FE924 | `stat_lap_complete` | arcade:stats.c:lap_complete lap time recording (sa |
| TODO | 0x800FEA08 | `stat_lap_split` | arcade:stats.c:stat_playgame checkpoint split (sam |
| TODO | 0x800FEC60 | `resource_type_select_simple` | resource type select simple |
| TODO | 0x800FEC78 | `game_results_init` | game results init (1808 bytes) |
| TODO | 0x800FEE04 | `gameover_handler` | arcade:gameover() - game over screen |
| TODO | 0x800FEE04 | `game_results_update` | game results update (1172 bytes) |
| TODO | 0x800FF298 | `game_results_render` | game results render (1164 bytes) |
| TODO | 0x800FF724 | `game_results_input` | game results input (1748 bytes) |
| TODO | 0x800FFDF8 | `game_results_exit` | game results exit (1900 bytes) |
| TODO | 0x80100D30 | `callback_init` | initialize object callback (44 bytes) - stores cal |
| TODO | 0x80100EE4 | `countdown_start` | countdown timer start (776 bytes) - starts 3-2-1 c |
| TODO | 0x80101700 | `best_time_check` | best time check (588 bytes) - checks if time beats |
| TODO | 0x80101700 | `stat_best_lap_check` | arcade:stats.c:update_track_time check vs best lap |
| TODO | 0x80101700 | `stat_speed_record_check` | arcade:stats.c speed record comparison (reuses bes |
| TODO | 0x8010194C | `best_time_record` | record new best (812 bytes) - saves new best time  |
| TODO | 0x8010194C | `stat_best_lap_save` | arcade:stats.c:update_track_time save new best (sa |
| TODO | 0x80101C78 | `leaderboard_display` | leaderboard display (1472 bytes) - shows top 5 tim |
| TODO | 0x80102250 | `race_results_screen` | race results screen (2084 bytes) - displays finish |
| TODO | 0x80102A74 | `award_ceremony` | award ceremony (1524 bytes) - displays race result |
| TODO | 0x8010306C | `trophy_animation` | trophy animation (876 bytes) - renders animated tr |
| TODO | 0x801033D8 | `continue_prompt` | continue prompt (1616 bytes) - ask player to conti |
| TODO | 0x80103A08 | `game_over_screen` | game over screen (2328 bytes) - displays final sta |
| TODO | 0x80104320 | `name_entry_screen` | name entry screen (1844 bytes) - enter initials fo |
| TODO | 0x80104A58 | `highscore_entry_anim` | high score entry animation (1036 bytes) - animates |
| TODO | 0x80104E84 | `statistics_display` | statistics display (2464 bytes) - shows player gam |
| TODO | 0x80104E84 | `stat_display_screen` | arcade:stats.c:UpdateStats equivalent (same as sta |
| TODO | 0x80105858 | `achievements_check` | achievements check (1692 bytes) - checks and unloc |
| TODO | 0x80105858 | `mode_unlock_handler` | arcade:money.c:gFreeGame reward unlock check (achi |
| TODO | 0x80105EF4 | `achievement_unlock` | achievement unlock (876 bytes) - shows unlock noti |
| TODO | 0x80105EF4 | `unlock_reward_show` | arcade:money.c:gFreeGame display unlock notificati |
| TODO | 0x80106260 | `achievement_display` | achievement display (1484 bytes) - renders achieve |
| TODO | 0x801068F4 | `profile_stats_update` | profile stats update (2144 bytes) - updates player |
| TODO | 0x801068F4 | `stat_profile_update` | arcade:stats.c:update_total_game_time profile stat |
| TODO | 0x80107110 | `difficulty_scaling` | difficulty scaling (1248 bytes) - scales game diff |
| TODO | 0x801073D8 | `checkpoint_hit` | record checkpoint passage, validate sequence (580  |
| TODO | 0x80107600 | `ai_difficulty_adjust` | AI difficulty adjust (924 bytes) - adjusts AI base |
| TODO | 0x8010761C | `lap_complete` | handle lap completion (1240 bytes) (arcade: checkp |
| TODO | 0x801079AC | `rubber_banding` | rubber banding (1768 bytes) - adjusts AI speed bas |
| TODO | 0x80107AF4 | `race_finish` | handle car finishing race (1000 bytes) (arcade: ch |
| TODO | 0x80108098 | `dynamic_difficulty` | dynamic difficulty (2356 bytes) - adjusts difficul |
| TODO | 0x801089CC | `catchup_logic` | catch-up logic (1432 bytes) - implements catch-up  |
| TODO | 0x80108F6C | `skill_rating_update` | skill rating update (876 bytes) - updates player s |
| TODO | 0x801092D8 | `matchmaking` | matchmaking (1984 bytes) - player skill-based matc |
| TODO | 0x80109A98 | `session_host` | session host (1124 bytes) - initializes multiplaye |
| TODO | 0x80109EFC | `session_join` | session join (1476 bytes) - joins existing multipl |
| TODO | 0x8010A4C0 | `session_leave` | session leave (892 bytes) - removes player from se |
| TODO | 0x8010A83C | `network_sync_full` | network sync full (2648 bytes) - synchronizes game |
| TODO | 0x8010B284 | `ping_measurement` | ping measurement (676 bytes) - measures input lag/ |
| TODO | 0x8010B528 | `struct_callback_init` | initialize structure with callback (56 bytes) - se |
| TODO | 0x8010B560 | `state_update_global` | update state based on global flag (112 bytes) - ch |
| TODO | 0x8010B874 | `disconnect_handler` | disconnection handling (1548 bytes) - handles play |
| TODO | 0x8010BE7C | `reconnect_attempt` | reconnection attempt (1152 bytes) - attempts to re |
| TODO | 0x8010C6D0 | `steering_apply` | arcade:fswheel.c:wheelpos N64 steering angle from  |
| TODO | 0x8010FBB4 | `sync_acquire_menu` | acquire menu sync lock (44 bytes) - calls state_ge |
| TODO | 0x8010FBF4 | `save_context_stub` | stack frame setup helper |
| TODO | 0x8010FC1C | `call_memcpy` | helper calling memcpy with size 64 |
| TODO | 0x8010FC2C | `call_osjammsg` | helper calling osJamMesg with size 1 |
| TODO | 0x8010FC34 | `clear_region` | zeroes region at 0x8003E928 |
| TODO | 0x8010FC80 | `resource_alloc_init` | allocate and initialize resource (64 bytes) - call |
| TODO | 0x8010FCC0 | `synced_model_render` | render model with sync (92 bytes) - acquires model |
| TODO | 0x8010FD1C | `struct_init_and_call` | initialize struct and call sound_start_3d (68 byte |

---


## How to Claim Work

1. Find a `TODO` function you want to work on
2. Edit this file to change status to `WIP`  
3. Add your session ID or name in Notes
4. Commit the change
5. Do the work
6. Update status to `MATCHED` or `CLOSE` when done

## Quick Start Commands

```bash
# Find target assembly for a function
grep "function_name" symbol_addrs.us.txt
grep -r "0x80XXXXXX" asm/us/

# Compile test
ssh watchman
cd /home/cburnes/projects/rush2049-decomp
tools/ido-static-recomp/build/out/cc -g0 -O2 -mips2 -G 0 -non_shared -c -o /tmp/test.o /tmp/test.c
mips-linux-gnu-objdump -d /tmp/test.o
```

---
Last updated: 2026-01-02
