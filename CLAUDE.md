# Claude Code Project Memory

This file helps Claude maintain context across sessions for the Rush 2049 N64 decompilation project.

## Project Overview

**Goal**: Decompile San Francisco Rush 2049 (N64) into matching C source code.

**Key Advantage**: We have the Rush The Rock arcade source code at `reference/repos/rushtherock/` which shares significant code with the N64 version.

## Current Status

**Phase**: 2 - Decompilation Ready (MATCHING BUILD ACHIEVED!)
**Last Updated**: 2025-12-07

### Completed
- [x] Project planning via Spec Kit (constitution, spec, plan, tasks)
- [x] Cloned reference repos (rushtherock, sm64, mk64, perfect_dark, banjo-kazooie)
- [x] Created lessons-learned.md from decomp research
- [x] Set up agent definitions in .claude/agents/
- [x] Verified ROM (US, 12MB)
- [x] Created project structure and README
- [x] Converted ROM from V64 to Z64 format
- [x] Configured splat (splat.us.yaml)
- [x] Initial ROM disassembly (~18K lines MIPS assembly)
- [x] Splat found 85 file boundaries
- [x] Split code into 88 individual .s files
- [x] **BUILD SYSTEM WORKING - ROM MATCHES!**
- [x] Set up OptiPlex 3080 (20 cores, 31GB RAM) for fast builds
- [x] Created hardware_regs.ld with N64 MMIO addresses
- [x] Created tools/diff.py for function comparison

### In Progress
- [ ] Match functions to arcade source
- [ ] Begin function decompilation

### Identified Functions (228/228 = 100% coverage! updated 2025-12-07)
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

See `symbol_addrs.us.txt` for complete list.

### Decompiled Source Files (32 C files, ~6033 lines)
| File | Functions | Status |
|------|-----------|--------|
| src/libc/string.c | memchr, memset, strchr, strlen, memcpy | Complete |
| src/libc/memory.c | bcopy (memmove with overlap handling) | Complete |
| src/libm/math.c | modf, modff, __isinf, __isnan, sinf, cosf | Complete |
| src/libgcc/ll.c | __lshrdi3, __ashldi3, __ashrdi3, __umoddi3, __udivdi3, __divdi3, __moddi3, __muldi3 | Complete (stubs) |
| src/libultra/os_message.c | osCreateMesgQueue, osSendMesg, osRecvMesg | Complete |
| src/libultra/os_vi.c | osViGetCurrentFramebuffer, osViSwapBuffer, osViSetMode, osViGetFramebuffer, osViSetSpecialFeatures | Complete |
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
| src/libultra/os_cpu.c | __osSetSR, __osGetSR, __osSetFpcCsr, __osGetFpcCsr (stubs) | Complete |
| src/libultra/os_pi.c | osPiInit, osPiGetAccess, osPiReleaseAccess, osPiReadWord, osPiWriteWord, osPiReadIo, osPiRawReadWord, osPiStartDma, osPiSetDeviceTiming | Complete |
| src/libultra/os_si.c | __osSiRawStartDma, osSiInit, __osSiGetAccess, __osSiRelAccess, osContStartReadData, __osContBuildRequest, __osContParseResponse | Complete |
| src/libultra/os_ai.c | osAiSetNextBuffer, osAiSetFrequency | Complete |
| src/libultra/os_sync.c | sync_init, sync_acquire, sync_release, sync_execute | Complete |
| src/libultra/os_queue.c | __osEnqueueThread, __osPopThread, __osDispatchThread | Complete |
| src/libultra/os_jam.c | osJamMesg | Complete |
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

### Key Discoveries from Agent Analysis

**Decompression (5610.s)**: DEFLATE/zlib inflate implementation
- Uses 4KB double-buffered sliding window
- Async I/O for ROM access optimization
- Can reference Perfect Dark's inflate.c as template

**Arcade Source Matches**:
| N64 Function | Arcade Equivalent | Confidence |
|--------------|-------------------|------------|
| func_8000C050 | GUTS/os/dll.c:dll_remove | High |
| func_8000C090 | GUTS/os/dll.c:dll_init | High |
| func_800020F0 (main) | game/init.c:start() | Medium |
| func_80002238 (game_init) | game/init.c:init() | Low-Medium |
| func_800024FC (audio_thread) | game/init.c:game_loop() | Low-Medium |
| func_800015F0 | game/visuals.c | Medium |
| func_80001B44 | game/camera.c | Medium |

**Key Insight from Codex Analysis (2025-12-07)**:
- N64 `game_init` (0x80002238) is **NOT** the same as arcade `game_init()`
- N64 game_init is OS bootstrap (message queues, ROM decompression, thread spawning)
- Arcade game_init (game.c:494-601) is gameplay setup (NVRAM options, difficulty, HUD)
- The actual game loop target is `func_800FD464` (called from main thread in infinite loop)
- Game logic is likely in compressed ROM data decompressed at runtime to 0x8010FD80+

### Assembly File Analysis
| File | Size | Content |
|------|------|---------|
| 5610.s | 125KB | Decompression code (LZSS, Huffman), inflate-style |
| 34A0.s | 122KB | libm functions, float-to-string conversion |
| 1050.s | 67KB | OS initialization, thread/message queue setup |
| D580.s | 40KB | Exception handler (__osException) |

### Not Started
- [ ] IDO compiler setup (for matching C builds)
- [ ] Progress tracking script

## Key Files

| File | Purpose |
|------|---------|
| `baserom.us.z64` | Original ROM in Z64 format (not in git) |
| `us.sha1` | ROM hash for verification |
| `splat.us.yaml` | Splat configuration for ROM extraction |
| `asm/us/*.s` | 88 disassembled code files |
| `asm/us/5610.s` | Largest code file (125KB) - likely game logic |
| `asm/us/34A0.s` | Second largest (124KB) - math/physics? |
| `rush2049.us.ld` | Generated linker script |
| `symbol_addrs.us.txt` | Discovered symbols |
| `.specify/specs/plan.md` | Implementation plan with phases |
| `.specify/specs/tasks.md` | Detailed task breakdown (144 tasks) |
| `reference/lessons-learned.md` | Best practices from other decomps |
| `reference/repos/rushtherock/` | **ARCADE SOURCE** - primary reference |
| `tools/mips_to_c/` | mips_to_c decompiler (auto-generates C) |
| `tools/m2c.py` | Wrapper script for mips_to_c |
| `tools/diff.py` | Function comparison tool |

## Architecture Notes

### Arcade Source Structure (rushtherock/)
The arcade source is organized as:
- `game/` - Main game code (~97K lines)
  - `game.c` - Game loop and state machine
  - `cars.c`, `tires.c`, `collision.c` - Physics
  - `drones.c`, `maxpath.c` - AI
  - `checkpoint.c` - Race logic
  - `camera.c` - Camera system
- `GUTS/` - Arcade system libraries (3dfx)
- `LIB/` - Utility libraries
- `MB/` - Mathbox (physics coprocessor)
- `OS/` - Operating system

### Three-Tier Portability
1. **Portable** (use as-is): game.c, checkpoint.c, drones.c, maxpath.c
2. **Adaptation** (modify): cars.c, tires.c, math functions
3. **Rewrite** (N64-specific): rendering, audio, I/O

### ROM Info
- Size: 12,582,912 bytes (12 MB)
- Format: Z64 (big-endian, converted from V64)
- SHA-1: 3f99351d7bb61656614bdb2aa1a90cfe55d1922c
- Internal name: "uRhs2 40 9"
- Cartridge ID: UR
- Graphics microcode: F3DEX2 (assumed)

## Build Commands (When Ready)

```bash
# Non-matching build (faster iteration)
make VERSION=us NON_MATCHING=1 -j$(nproc)

# Matching build
make VERSION=us -j$(nproc)

# Check progress
make progress

# Verify ROM hash
make test

# Diff a function
./tools/diff.py function_name -m -w

# Auto-decompile a function (generates initial C code)
./tools/m2c.py func_80006A00           # By function name
./tools/m2c.py asm/us/5610.s -f 0      # First function in file
./tools/m2c.py asm/us/7600.s           # All functions in file
```

## Agent Roles

See `.claude/agents/` for specialized agents:
- **asm-analyzer** - MIPS assembly analysis
- **c-writer** - Write matching C code
- **arcade-comparator** - Find arcade equivalents
- **project-r-analyst** - Study Project R insights
- **n64-decomp-expert** - Apply lessons from other decomps
- **build-runner** - Build and verify

## Development Environment

- **Primary Build Machine**: OptiPlex 3080 WSL (20 cores, 31GB RAM)
  - SSH: `ssh -p 2222 user@optiplex3080`
  - Sudo without password enabled
  - 10x faster builds than Raspberry Pi
- **Light Work**: Raspberry Pi (current terminal)
- **Spec Kit**: Installed via `uv tool install specify-cli`

## Next Steps (When Resuming)

1. Continue decompiling library functions (remaining libm, libultra)
2. Set up IDO compiler for matching C builds
3. Match N64 game functions to arcade source code
4. Identify key game structures (car state, track data, etc.)
5. Begin systematic game function decompilation

## Quick Reference

### Arcade Game States (game.h)
```c
enum GState {
    ATTRACT, TRKSEL, CARSEL, PLAYGAME,
    ENDGAME, GAMEOVER, HISCORE, PREPLAY,
    PREPLAY2, COUNTDOWN
};
```

### Key Arcade Files to Reference
| N64 Module | Arcade Equivalent | LOC |
|------------|-------------------|-----|
| src/game/game.c | game/game.c | 1,687 |
| src/racing/checkpoint.c | game/checkpoint.c | 1,694 |
| src/racing/cars.c | game/cars.c | 1,892 |
| src/racing/drones.c | game/drones.c | ~24K |
| src/racing/maxpath.c | game/maxpath.c | 3,730 |
| src/camera/camera.c | game/camera.c | 2,043 |

## Session Continuity Tips

When starting a new session:
1. Read this file first
2. Check `symbol_addrs.us.txt` for identified functions (228 total, 100% coverage)
3. Check git log for recent changes
4. Run `make progress` (when available) to see status

## Next Session TODO

**Priority**: Analyze extracted game code and match to arcade source
1. **Analyze extracted game code** (build/game_code.bin, build/game_code_disasm.txt)
2. **Match RDP/graphics functions** to arcade source (game contains render commands)
3. **Continue matching game functions** to arcade source in `reference/repos/rushtherock/`
4. **Decompile a simple game function** to test the workflow

### Compressed Game Code (EXTRACTED 2025-12-07!)
- **ROM offset**: 0xB0CB10 (compressed DEFLATE data)
- **RAM destination**: 0x8010FD80 - 0x801AE080
- **Decompressed size**: 647,072 bytes (~632 KB)
- **Functions found**: 752 functions, 1480 return instructions
- **Content**: RDP graphics commands, game rendering, actual game logic!

**Tools created**:
- `tools/extract_game_code.py` - Extracts and decompresses game code from ROM
- `tools/simple_mips_disasm.py` - Basic MIPS disassembler (for Pi without toolchain)
- `build/game_code.bin` - Extracted raw binary
- `build/game_code_disasm.txt` - Full disassembly (163K lines)

**Useful Codex command for function ID**:
```bash
codex exec --dangerously-bypass-approvals-and-sandbox "Analyze asm/us/XXXX.s and identify functions..."
```

**Current stats**:
- Static ROM: 228 functions identified (100% coverage), 32 C files, ~6033 lines
- Dynamic game code: 752 functions extracted, ready for analysis
