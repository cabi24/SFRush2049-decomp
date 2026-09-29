# Early function and source inventory

Archived from `CLAUDE.md` on 2026-09-28. These tables describe the December
2025–January 2026 inventory; counts, paths, and status labels are historical.
“Complete” included stubs and source scaffolding. Identification, C file counts,
and lines of source do not establish matched or cartridge-linked coverage.
The categories can overlap; do not sum them as a current population count.

For current coverage use `make progress`. For extracted targets use the
closure-derived population described in the [context skill](../../.claude/skills/refresh-game-context/SKILL.md).
See [milestones](project-milestones.md) for the chronology.

## Identified functions (reported 228/228 on 2025-12-07)

| Category | Count | Examples |
|----------|-------|----------|
| startup | 4 | entrypoint, main, idle_thread_entry, audio_thread_entry |
| libc | 8 | memchr, memset, strchr, strlen, memcpy, bzero, bcopy, bzero_alt |
| libm | 10 | modf, modff, __isinf, __isnan, sinf, cosf, sqrtf, fcvt, __ecvt_internal, __round_helper |
| libultra os | 70 | osCreateMesgQueue, osJamMesg, osPiStartDma, osSpTaskYielded, osDpWait, osAiSetFrequency, osContStartReadData, osSetTimer, osCreatePiManager, osCreateViManager, osInvalICache, osWritebackDCache, osSpTaskLoad, __osException, __osSetCompare, __osViSwapContext, etc. |
| libultra gu | 10 | guMtxIdentF, guMtxF2L, guMtxL2F, guMtxIdent, guOrthoF, guOrtho, guPerspectiveF, guPerspective, guLookAtF, guLookAt |
| libultra pfs | 25 | osPfsInitPak, osPfsChecker, osPfsReadWriteFile, osPfsFreeBlocks, osPfsFileState, osPfsAllocate, osPfsDeleteFile, osPfsRename, osPfsFindFile, osPfsGetFileStat, osPfsGetFileSize, osPfsReAllocate, __osPfsSelectBank, __osPfsCheckPages, etc. |
| controller | 8 | osContStartQuery, osContGetQuery, osContStartReadData, osContGetReadData, __osPackReadData, __osContBuildPacket, __osContGetStatus, __osContRamReset |
| libultra motor | 4 | osMotorInit, __osMotorAccess, osMotorStart, osMotorStop |
| libultra vi | 4 | osViModeTableGet, osViModeNtscLan1, osViModeNtscLpn1, vi_manager_main |
| libultra sp | 2 | osSpTaskLoad, __osPiReadDeviceType |
| libgcc FP | 8 | __fixdfdi, __floatdidf, etc. |
| libgcc 64-bit | 9 | __lshrdi3, __udivdi3, __muldi3, etc. |
| inflate/decomp | 16 | inflate_entry, inflate_loop, huft_build, lzss_decode, inflate_read_bits, inflate_needbits, inflate_getbits, inflate_io_wait, inflate_flush_window, inflate_free_window, etc. |
| timer queue | 14 | dll_remove, dll_init, dll_update, dll_reschedule, dll_insert, dll_get_priority, dll_get_data, dll_set_data, __osEnqueueThread, __osPopThread, __osDispatchThread, __osTimerInterrupt, __osGetTimerValue, __osInsertTimer |
| display/render | 8 | display_update, viewport_setup, get_viewport_pos, display_mode_tick, get_tv_offset, apply_display_mode |
| game init | 1 | game_init |
| utility | 3 | checksum8, checksum16_adler, comm_parse |
| scheduler | 13 | osCreateScheduler, osScAddClient, __scMain, __scSchedule, __scHandleRetrace, __scHandleRSP, __scHandleRDP, __scTaskReady, __scExecTask, __scAppendList, __scExec, __scHandlePreNMI, __scScheduleCore |
| VI timing | 9 | viTickStart, viEnableAccum, viDisableAccum, viUpdateTime, viScheduleTick, viAddTicks, viGetTimeToDeadline, viDeadlinePassed, viStub |

See [`symbol_addrs.us.txt`](../../symbol_addrs.us.txt) for the maintained symbol file.

## Decompiled Source Files (120 C files, ~98,517 lines)

| File | Functions | Status |
|------|-----------|--------|
| src/libc/string.c | memchr, memset, strchr, strlen, memcpy | Complete |
| src/libc/memory.c | bcopy (memmove with overlap handling) | Complete |
| src/libm/math.c | modf, modff, __isinf, __isnan, sinf, cosf | Complete |
| src/libgcc/ll.c | __lshrdi3, __ashldi3, __ashrdi3, __umoddi3, __udivdi3, __divdi3, __moddi3, __muldi3 | Complete (stubs) |
| src/libultra/os_message.c | osCreateMesgQueue, osSendMesg, osRecvMesg | Complete |
| src/libultra/os_vi.c | osViGetCurrentFramebuffer, osViSwapBuffer, osViSetMode, osViGetFramebuffer, osViSetSpecialFeatures, osViSetSwapBuffer | Complete |
| src/libultra/os_vi_mgr.c | osViInit, vi_manager_thread | Complete |
| src/libultra/os_event.c | osSetEventMesg | Complete |
| src/libultra/os_thread.c | osCreateThread, osStartThread, osSetThreadPri, osSetIntMask | Complete |
| src/libultra/os_cache.c | osInvalICache, osInvalDCache, bzero | Complete |
| src/libultra/os_timer.c | osSetTimer, osSetTimerIntr, osGetTime (64-bit) | Complete |
| src/libultra/os_int.c | __osDisableInt, __osRestoreInt | Complete |
| src/libultra/os_dp.c | osDpSetNextBuffer, osDpWait, osDpGetCounters | Complete |
| src/libultra/os_sp.c | __osSpSetStatus, __osSpSetPc, __osSpDeviceBusy | Complete |
| src/libultra/os_sp_task.c | osSpTaskYielded, osViGetFramebuffer | Complete |
| src/libultra/os_misc.c | osDpIsBusy, osVirtualToPhysical (full), osGetActiveQueue, osPhysicalToVirtual | Complete |
| src/libultra/os_cpu.c | __osSetSR, __osGetSR, __osSetFpcCsr, __osGetFpcCsr, __osGetCause (inline asm) | Complete |
| src/libultra/os_tlb.c | __osTlbInit, __osTLBLookup, osTLBMapTLB, osTLBUnmapTLB (asm-only stubs) | Complete |
| src/libultra/os_cont.c | osContStartQuery, osContGetQuery, osContStartReadData, osContGetReadData, __osContRamReset | Complete |
| src/libultra/os_pi.c | osPiInit, osPiGetAccess, osPiReleaseAccess, osPiReadWord, osPiWriteWord, osPiReadIo, osPiRawReadWord, osPiStartDma, osPiSetDeviceTiming | Complete |
| src/libultra/os_si.c | __osSiRawStartDma, osSiInit, __osSiGetAccess, __osSiRelAccess, osContStartReadData, __osContBuildRequest, __osContParseResponse | Complete |
| src/libultra/os_ai.c | osAiSetNextBuffer, osAiSetFrequency | Complete |
| src/libultra/os_sync.c | sync_init, sync_acquire, sync_release, sync_execute | Complete |
| src/libultra/os_queue.c | __osEnqueueThread, __osPopThread, __osDispatchThread | Complete |
| src/libultra/os_jam.c | osJamMesg | Complete |
| src/libultra/os_scheduler.c | osCreateScheduler, osScAddClient, __scMain, __scSchedule, __scHandleRetrace, osScResetTime, osScEnableTime, osScDisableTime, osScUpdateTime, osScSetDeadline, osScAddDeadline, osScGetTimeRemaining, osScDeadlinePassed | Complete |
| src/game/gfx.c | gfx_init_dl, gfx_alloc_dl | Complete |
| src/os/dll.c | dll_remove, dll_init, dll_update, dll_reschedule, dll_insert, dll_get_priority | Complete |
| src/inflate/inflate.c | inflate_entry, inflate_loop, inflate_block, huft_build, inflate_codes, inflate_stored, inflate_fixed, inflate_dynamic | Complete |
| src/game/init.c | main, game_init, thread entry points | Complete |
| src/game/display.c | display_update, viewport_setup, display_process, get_tv_offset, get_viewport_pos, get_viewport_offset, update_viewport | Complete |
| src/util/checksum.c | checksum8, checksum16_adler | Complete |
| src/util/dma.c | dma_queue_init, dma_wait, dma_signal, lzss_decompress, inflate_decompress | Complete |
| src/game/matrix.c | guMtxIdentF, guMtxF2L, guMtxL2F, guMtxIdent | Complete |
| src/libultra/gu.c | guOrthoF, guOrtho, guPerspectiveF, guPerspective, guLookAtF, guLookAt | Complete |
| include/types.h | Basic types, volatile types (vu32, etc.), vector/matrix types | Complete |
| include/inflate/inflate.h | struct huft, inflate function prototypes | Complete |
| include/PR/os_message.h | OSMesgQueue structure | Complete |
| include/game/game.h | GState enum, game constants | Complete |

### Decompiled ROM Functions (Game Code)

| File | ROM Functions | Lines |
|------|---------------|-------|
| src/game/game.c | game_loop, state machines, render pipeline, input, physics, player state (137 funcs) | 4309 |
| src/game/sound.c | func_800B358C (sound_stop) | 818 |
| src/libultra/os_pfs.c | osPfsInitPak, __osPfsGetStatus, osPfsFreeBlocks, __osPfsSelectBank, osPfsReadWriteFile | 351 |

### Game Code Files (Arcade-Style Stubs)

| File | Lines | Description |
|------|-------|-------------|
| src/game/menu.c | 904 | Menu system and UI |
| src/game/hiscore.c | 793 | High score entry and display |
| src/game/physics.c | 767 | Car physics (Milliken model) |
| src/game/car.c | 705 | Car state and control |
| src/game/effects.c | 710 | Particle and visual effects |
| src/game/race.c | 675 | Race logic and timing |
| src/game/replay.c | 649 | Replay recording/playback |
| src/game/attract.c | 633 | Attract mode handler |
| src/game/carsel.c | 622 | Car selection UI |
| src/game/select.c | 620 | Track selection |
| src/game/drivetrain.c | 595 | Engine/transmission |
| src/game/tire.c | 506 | Tire physics model |
| src/game/sound.c | 482 | Sound system |
| src/game/state.c | 443 | Game state machine |
| src/game/vecmath.c | 442 | Vector/matrix math |
| src/game/drone.c | 432 | AI drone control |
| src/game/collision.c | 430 | Collision detection |
| src/game/road.c | 425 | Road/track geometry |
| src/game/camera.c | 423 | Camera system |
| src/game/checkpoint.c | ~350 | Checkpoint logic |

## Library files added on 2025-12-27

- Controller Pak (PFS): os_pfs_alloc.c, os_pfs_check.c, os_pfs_create.c, os_pfs_delete.c, os_pfs_find.c, os_pfs_free.c, os_pfs_rw.c, os_pfs_state.c, os_pfs_write.c
- Video Interface: os_vi_init.c, os_vi_intr.c, os_video.c
- Peripheral Interface: os_pi_dma.c, os_pi_write.c, os_pif.c
- Serial Interface: os_si_ext.c
- Signal Processor: os_sp_dma.c
- Controller: os_cont_query.c, os_cont_status.c
- Thread/Timer: os_thread_ext.c, os_thread_pri.c, os_timer_set.c, os_yield.c
- CPU/FPU: os_fpcsr.c, os_phys.c, os_dp_counters.c
- Other: os_debug.c, os_mesg_jam.c, boot/boot.c

## Early assembly text inventory

These are the original text-file size estimates and descriptions, not binary
function sizes or the current splat layout. For maintained address mappings,
see [memory_map.md](../memory_map.md).

| File | Size | Content |
|------|------|---------|
| 5610.s | 125KB | Decompression code (LZSS, Huffman), inflate-style |
| 34A0.s | 122KB | libm functions, float-to-string conversion |
| 1050.s | 67KB | OS initialization, thread/message queue setup |
| D580.s | 40KB | Exception handler (__osException) |
