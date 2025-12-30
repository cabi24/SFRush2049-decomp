# Rush 2049 N64 Memory Map

This document describes the memory layout of San Francisco Rush 2049 for Nintendo 64, based on analysis of the ROM, symbol files, and decompilation research.

## Overview

| Region | Start | End | Size | Description |
|--------|-------|-----|------|-------------|
| ROM Header | 0x00 | 0x40 | 64B | N64 ROM header |
| IPL3 Boot | 0x40 | 0x1000 | ~4KB | IPL3 boot code |
| Static Code | 0x80000400 | 0x8000F700 | ~61KB | Uncompressed boot/library code |
| Static Data | 0x80010000 | 0x80037DB0 | ~159KB | Static data/rodata |
| BSS (Static) | 0x80037DB0 | 0x80086A50 | ~318KB | Static BSS/uninitialized data |
| Decompressed Game | 0x80086A50 | 0x80124AF0 | ~632KB | Compressed game code (DEFLATE) |
| Game BSS | 0x801249F0 | 0x8017A640 | ~346KB | Game BSS/runtime data |
| Hardware Regs | 0xA4000000 | 0xA4800020 | - | N64 MMIO registers |

**Total RAM Used**: ~1.5MB of 4MB available (8MB with Expansion Pak)

---

## 1. Code Segments

### 1.1 Static ROM Code (0x80000400 - 0x8000F700)

This code is loaded directly from ROM at boot and remains resident. It contains libultra OS functions, decompression routines, and initialization code.

| ROM Offset | VRAM Address | Size | Description |
|------------|--------------|------|-------------|
| 0x1000 | 0x80000400 | 0x50 | Entry point (clears BSS, sets stack, jumps to main) |
| 0x1050 | 0x80000450 | 0x11A0 | Scheduler (osCreateScheduler, scMain, scHandleRetrace, etc.) |
| 0x21F0 | 0x800015F0 | 0xB00 | Display/rendering (display_update, viewport_setup) |
| 0x2CF0 | 0x80002238 | 0x450 | Game init (main, game_init, thread entries) |
| 0x3140 | 0x80002540 | 0x1F0 | DMA utilities (dma_queue_init, lzss_decompress, inflate_decompress) |
| 0x34A0 | 0x800028A0 | 0x2150 | libm (modf, modff, sinf, cosf, fcvt, __ecvt_internal) |
| 0x5610 | 0x80004A14 | 0x1FEC | Inflate/DEFLATE decompression |
| 0x7600 | 0x80006A00 | 0x630 | OS message queue (osCreateMesgQueue, osRecvMesg, osSendMesg) |
| 0x7630 | 0x80006BB0 | 0x310 | VI manager (vi_manager_main) |
| 0x8E10 | 0x80008210 | 0x270 | PI/VI managers (osCreatePiManager, osCreateViManager) |
| 0x9660 | 0x80008A60 | 0x780 | Graphics utilities (guOrtho, guPerspective, guLookAt) |
| 0x9DE0 | 0x800091E0 | 0x270 | Matrix functions (guMtxF2L, guMtxL2F, guMtxIdent) |
| 0xAB20 | 0x80009F20 | 0x3D0 | Motor Pak (osMotorInit, osMotorStart, osMotorStop) |
| 0xBFE0 | 0x8000B3E0 | 0x9B0 | Controller Pak (osPfs* functions) |
| 0xD580 | 0x8000C980 | 0x930 | Exception handler (__osException) - handwritten ASM |
| 0xE4F0 | 0x8000D8F0 | 0x2C0 | libgcc 64-bit arithmetic (__lshrdi3, __udivdi3, __muldi3) |

**Key Functions in Static Code:**

```
entrypoint        = 0x80000400  // ROM bootstrap
main              = 0x800020F0  // Real main function
game_init         = 0x80002238  // OS-level initialization
idle_thread_entry = 0x800021A4  // Idle thread
audio_thread_entry= 0x800024FC  // Audio thread
osCreateScheduler = 0x80000450  // Scheduler creation
__osException     = 0x8000C980  // CPU exception handler
inflate_entry     = 0x80006814  // DEFLATE decompression entry
```

### 1.2 Decompressed Game Code (0x80086A50 - 0x80124AF0)

Game logic is stored compressed in ROM at offset 0xB0CB10 using DEFLATE compression. At boot, it is decompressed to RAM starting at 0x80086A50.

| Address Range | Approx Size | Description |
|---------------|-------------|-------------|
| 0x80087068 - 0x8008A000 | ~12KB | Entity/render state processing |
| 0x8008A000 - 0x8009F000 | ~84KB | Audio, sync, resource management |
| 0x8009F000 - 0x800B0000 | ~68KB | Physics, AI, pathfinding (maxpath) |
| 0x800B0000 - 0x800C0000 | ~64KB | Vehicle physics, effects, UI helpers |
| 0x800C0000 - 0x800D0000 | ~64KB | Camera, HUD, menu systems |
| 0x800D0000 - 0x800E0000 | ~64KB | Track/car selection, race setup |
| 0x800E0000 - 0x800F0000 | ~64KB | Race state, audio, video timing |
| 0x800F0000 - 0x80100000 | ~64KB | Game loop, state machine, object dispatch |
| 0x80100000 - 0x80124AF0 | ~148KB | Additional game functions |

**Key Game Functions:**

```
game_loop              = 0x800FD464  // Main per-frame state machine (704 bytes)
PlayGame_HandleStateChange = 0x800CA3B4  // State change handler (2544 bytes)
render_object          = 0x80099BFC  // 3D model rendering (10KB)
render_large           = 0x80087A08  // Large rendering function (10KB)
render_scene           = 0x800A04C4  // Scene/viewport setup (2720 bytes)
PhysicsObjectList_Update = 0x800B0868  // Physics update
effects_update_emitters = 0x800B811C  // Particle/FX system
race_state_update      = 0x800DB81C  // Race progression (2148 bytes)
attract_state_machine  = 0x800D349C  // Attract mode (1676 bytes)
controller_poll        = 0x800C95DC  // Controller input (928 bytes)
```

**Game State Machine (at game_loop 0x800FD464):**

The game uses a bitmask ladder for state dispatch:

| State Bit | Handler Address | State Name |
|-----------|----------------|------------|
| 0x00040000 | 0x800FBF90 | GSTATE_INIT |
| 0x00080000 | 0x800FBFE4 | GSTATE_SETUP |
| 0x00100000 | 0x800FC0EC | GSTATE_MENU |
| 0x00200000 | 0x800FC038 | GSTATE_CAMERA |
| 0x00400000 | 0x800FC228 | GSTATE_READY |
| 0x00800000 | 0x800FC25C | GSTATE_PLAY |
| 0x01000000 | 0x800FC89C | GSTATE_CARS |
| 0x02000000 | 0x800FC960 | GSTATE_FINISH |

---

## 2. Global Variables (D_80* Addresses)

### 2.1 OS/System Variables (0x80000300 - 0x80000320)

```
osTvType       = 0x80000300  // TV type (PAL/NTSC/MPAL)
osRomBase      = 0x80000308  // ROM base address
osResetType    = 0x8000030C  // Reset type
osMemSize      = 0x80000318  // Total RAM size
osAppNMIBuffer = 0x8000031C  // NMI buffer
```

### 2.2 Thread Structures (0x80034000 - 0x80035000)

| Address | Name | Description |
|---------|------|-------------|
| 0x800344E0 | gGameThread | OSThread for main game (thread 7) |
| 0x80034690 | gAudioThread | OSThread for audio (thread 8) |
| 0x80034840 | gRenderThread | OSThread for render (thread 5) |
| 0x80034BA0 | gIdleThread | OSThread for idle (thread 1) |
| 0x80034D50 | gInitThread | OSThread for init (thread 6) |

### 2.3 Thread Stacks (0x8002F660 - 0x80032000)

| Address | Name | Approximate Size |
|---------|------|------------------|
| 0x8002F660 | gIdleThreadStack | ~400 bytes |
| 0x8002F7F0 | gAudioThreadStack | ~2KB |
| 0x80030150 | gInitThreadStack | ~2KB |
| 0x80030AB0 | gGameThreadStack | ~5KB |
| 0x80031D70 | gRenderThreadStack | ~6KB |

### 2.4 Message Queues (0x8002ECC0 - 0x80035460)

| Address | Name | Description |
|---------|------|-------------|
| 0x8002ECC0 | gEventMesgQueue | Event message queue |
| 0x8002ECF8 | gRetraceMesgQueue | VBlank/retrace messages |
| 0x80035410 | gDmaMesgQueue | DMA completion messages |
| 0x80035428 | gInflateMesgQueue | Decompression I/O queue |
| 0x80035440 | gSyncMesgQueue | Sync signal queue |

### 2.5 OS Timer State (0x80037C50 - 0x80037C64)

```
__osTimerList    = 0x8002C3F0  // Timer queue head (TimerQueueHead*)
__osTimerCounter = 0x80037C60  // Last timer timestamp (osGetCount value)
__osRunningThread = 0x8002C3E0  // Current running thread (OSThread*)
```

### 2.6 Display List Pointers (0x800354C4 - 0x800354D0)

```
gDisplayListHead = 0x800354C4  // Display list buffer start
gDisplayListSize = 0x800354C8  // Current offset in display list
gDisplayListEnd  = 0x800354CC  // Display list buffer end
```

---

## 3. Key Game Structures

### 3.1 Game State (0x801146EC - 0x80117500)

| Address | Name | Type | Description |
|---------|------|------|-------------|
| 0x801146EC | gstate_init_flag | u8 | Set once on first game_loop call |
| 0x801170FC | race_active | u32 | Non-zero enables race loop |
| 0x801174B4 | gstate | u32 | Current game state bitmask (110 refs) |
| 0x801174B8 | gstate_prev | u32 | Previous game state (transition detection) |

### 3.2 Main Game Structure (0x801461D0)

This is the most referenced global (160 references). Estimated size ~0x300 bytes.

```c
typedef struct GameStruct {
    u8      state;          // 0x00: Current game state
    u8      prev_state;     // 0x01: Previous game state
    u8      num_players;    // 0x02: Number of players
    u8      num_cars;       // 0x03: Total cars in race
    s32     track_id;       // Track selection
    s32     difficulty;     // Game difficulty
    s32     num_laps;       // Number of laps
    u32     frame_count;    // Frame counter (0x80142AFC)
    f32     game_time;      // Elapsed game time
    f32     countdown_time; // Countdown timer
    // ... additional fields
} GameStruct;  // @ 0x801461D0
```

### 3.3 Car State Array (0x80152818)

Array of 6 car entries, each 952 bytes (0x3B8 stride).

```c
// game_car[6] @ 0x80152818
// Per-car layout:
//   +0x000: handle/pointer
//   +0x008: float (velocity X)
//   +0x00C: float (velocity Y)
//   +0x04C: float (position X)
//   +0x050: float (position Y)
//   +0x054: float (position Z)
//   +0x0EF: flag byte
//   +0x359: flag byte
//   +0x7E8: flag byte
```

Based on arcade CarData struct (~0x11C bytes core fields):

```c
typedef struct CarData {
    f32     dr_pos[3];       // 0x00: Dead reckoned position
    f32     dr_vel[3];       // 0x0C: Dead reckoned velocity
    f32     dr_uvs[3][3];    // 0x18: Dead reckoned orientation
    f32     dr_tirepos[4][3]; // 0x3C: Tire positions
    f32     dr_acc[3];       // 0x6C: Acceleration
    f32     RWV[3];          // Real world velocity
    f32     RWR[3];          // Real world position
    f32     mph;             // Speedometer reading
    s16     crashflag;       // Crashed flag
    s16     rpm;             // Engine RPM
    s8      place;           // Race position (1st, 2nd, etc)
    s8      laps;            // Current lap
    s8      checkpoint;      // Current checkpoint
    // ... additional fields
} CarData;
```

### 3.4 Track Data Array (0x8014A250)

Array of 6 track entries, each 2056 bytes (0x808 stride).

```c
// track_data[6] @ 0x8014A250
// Per-track layout:
//   +0x710: float (timing data)
//   +0x714: float (timing data)
//   +0x718: float (timing data)
//   +0x71C: flags
//   +0x720: flags
```

### 3.5 Player Slots (0x80144030)

Player state slots, stride 772 bytes (0x304).

### 3.6 Input Buffer (0x80140BF0)

Per-player input state, 4 players x 32 bytes (0x20 stride).

```c
// input_buffer[4] @ 0x80140BF0
// Per-player layout:
//   +0x00: button states (current)
//   +0x08: button states (previous)
//   +0x0A: 16-bit axis (steering)
//   +0x0C: 16-bit axis (throttle)
//   +0x0E: 16-bit axis (brake)
//   +0x18-0x1E: additional analog values
//   +0x1A: "new data" flag
```

### 3.7 Additional Data Arrays

| Address | Name | Description |
|---------|------|-------------|
| 0x80142AFC | frame_counter | Incremented each frame |
| 0x80142728 | D_80142728 | Unknown (77 refs) |
| 0x8014A110 | D_8014A110 | Paired with A250 (74 refs) |
| 0x80150B70 | effect_emitters_array | Particle effects (152-byte stride) |
| 0x80159450 | active_sound_list | Active objects/sounds array |
| 0x80159788 | active_sound_count | Active object/sound count |
| 0x801491F0 | physics_object_list_head | Physics linked list |

---

## 4. BSS/Heap Regions

### 4.1 Static BSS (0x80037DB0 - 0x80086A50)

| Address Range | Size | Content |
|---------------|------|---------|
| 0x80037DB0 | ~318KB | Exception context, OS structures |
| 0x8002C3E0 | 8 bytes | __osRunningThread |
| 0x8002C3F0 | 8 bytes | __osTimerList |

### 4.2 Game BSS (0x801249F0 - 0x8017A640)

| Address Range | Approx Size | Content |
|---------------|-------------|---------|
| 0x801249F0 - 0x80140000 | ~108KB | Early game data |
| 0x80140000 - 0x80150000 | ~64KB | Input, frame data, race state |
| 0x80150000 - 0x80160000 | ~64KB | Car arrays, effects |
| 0x80160000 - 0x8017A640 | ~105KB | Additional runtime data |

### 4.3 Memory Layout Summary

```
0x80000000  +------------------+
            | OS Constants     |  (0x300-0x320)
0x80000400  +------------------+
            | Static Code      |  ~61KB (entrypoint, libultra, inflate)
0x8000F700  +------------------+
            | Static Data      |  ~159KB
0x80037DB0  +------------------+
            | Static BSS       |  ~318KB (OS structures)
0x80086A50  +------------------+
            | Decompressed     |  ~632KB (game code from ROM)
            | Game Code        |
0x80124AF0  +------------------+
            | Game Data/BSS    |  ~346KB (runtime structures)
0x8017A640  +------------------+
            | Heap/Stack       |  Remaining RAM
0x80400000  +------------------+  End of 4MB RAM
            | (Expansion Pak)  |  0x80400000-0x80800000 (if present)
0x80800000  +------------------+
```

---

## 5. Hardware Register Regions

The N64 hardware registers are memory-mapped in the KSEG1 uncached region (0xA4000000+).

### 5.1 MIPS Interface (MI) - 0xA4300000

```
MI_INIT_MODE_REG  = 0xA4300000  // MI mode register
MI_VERSION_REG    = 0xA4300004  // MI version
MI_INTR_REG       = 0xA4300008  // Interrupt status
MI_INTR_MASK_REG  = 0xA430000C  // Interrupt mask
```

### 5.2 Video Interface (VI) - 0xA4400000

```
VI_STATUS_REG     = 0xA4400000  // VI status/control
VI_DRAM_ADDR_REG  = 0xA4400004  // Framebuffer address
VI_WIDTH_REG      = 0xA4400008  // Framebuffer width
VI_INTR_REG       = 0xA440000C  // Interrupt line
VI_CURRENT_REG    = 0xA4400010  // Current scanline
VI_BURST_REG      = 0xA4400014  // Color burst
VI_V_SYNC_REG     = 0xA4400018  // Vertical sync
VI_H_SYNC_REG     = 0xA440001C  // Horizontal sync
VI_LEAP_REG       = 0xA4400020  // Leap pattern
VI_H_START_REG    = 0xA4400024  // Horizontal start
VI_V_START_REG    = 0xA4400028  // Vertical start
VI_V_BURST_REG    = 0xA440002C  // Vertical burst
VI_X_SCALE_REG    = 0xA4400030  // X scale factor
VI_Y_SCALE_REG    = 0xA4400034  // Y scale factor
```

### 5.3 Audio Interface (AI) - 0xA4500000

```
AI_DRAM_ADDR_REG  = 0xA4500000  // Audio DMA address
AI_LEN_REG        = 0xA4500004  // Audio DMA length
AI_CONTROL_REG    = 0xA4500008  // Control register
AI_STATUS_REG     = 0xA450000C  // Status register
AI_DACRATE_REG    = 0xA4500010  // DAC sample rate
AI_BITRATE_REG    = 0xA4500014  // Audio bitrate
```

### 5.4 Peripheral Interface (PI) - 0xA4600000

```
PI_DRAM_ADDR_REG  = 0xA4600000  // RDRAM address for DMA
PI_CART_ADDR_REG  = 0xA4600004  // Cartridge address
PI_RD_LEN_REG     = 0xA4600008  // Read length
PI_WR_LEN_REG     = 0xA460000C  // Write length
PI_STATUS_REG     = 0xA4600010  // Status register
PI_BSD_DOM1_*     = 0xA4600014-0x20  // Domain 1 timing
PI_BSD_DOM2_*     = 0xA4600024-0x30  // Domain 2 timing
```

### 5.5 RDRAM Interface (RI) - 0xA4700000

```
RI_MODE_REG       = 0xA4700000  // RDRAM mode
RI_CONFIG_REG     = 0xA4700004  // RDRAM config
RI_CURRENT_LOAD   = 0xA4700008  // Current load
RI_SELECT_REG     = 0xA470000C  // RDRAM select
RI_REFRESH_REG    = 0xA4700010  // Refresh rate
```

### 5.6 Serial Interface (SI) - 0xA4800000

```
SI_DRAM_ADDR_REG  = 0xA4800000  // RDRAM address
SI_PIF_ADDR_RD64B = 0xA4800004  // PIF read address
SI_PIF_ADDR_WR64B = 0xA4800010  // PIF write address
SI_STATUS_REG     = 0xA4800018  // Status register
```

### 5.7 Signal Processor (SP) - 0xA4040000

```
SP_MEM_ADDR_REG   = 0xA4040000  // IMEM/DMEM address
SP_DRAM_ADDR_REG  = 0xA4040004  // RDRAM address
SP_RD_LEN_REG     = 0xA4040008  // Read length
SP_WR_LEN_REG     = 0xA404000C  // Write length
SP_STATUS_REG     = 0xA4040010  // Status register
SP_DMA_FULL_REG   = 0xA4040014  // DMA full flag
SP_DMA_BUSY_REG   = 0xA4040018  // DMA busy flag
SP_SEMAPHORE_REG  = 0xA404001C  // Semaphore
SP_PC_REG         = 0xA4080000  // RSP program counter
```

### 5.8 Display Processor (DPC) - 0xA4100000

```
DPC_START_REG     = 0xA4100000  // Display list start
DPC_END_REG       = 0xA4100004  // Display list end
DPC_CURRENT_REG   = 0xA4100008  // Current address
DPC_STATUS_REG    = 0xA410000C  // Status register
DPC_CLOCK_REG     = 0xA4100010  // Clock counter
DPC_BUFBUSY_REG   = 0xA4100014  // Buffer busy
DPC_PIPEBUSY_REG  = 0xA4100018  // Pipe busy
DPC_TMEM_REG      = 0xA410001C  // TMEM counter
```

---

## 6. ROM Layout

| Offset | Size | Description |
|--------|------|-------------|
| 0x00000000 | 0x40 | ROM header |
| 0x00000040 | 0xFC0 | IPL3 boot code |
| 0x00001000 | 0xEF00 | Static code (uncompressed) |
| 0x00010000 | ~0xA0CB10 | Data segment (uncompressed) |
| 0x00B0CB10 | ~0x4F34F0 | Compressed game code (DEFLATE) |
| ... | ... | Additional assets |
| 0x00C00000 | - | End of 12MB ROM |

**ROM Details:**
- Size: 12,582,912 bytes (12 MB)
- Format: Z64 (big-endian)
- SHA-1: 3f99351d7bb61656614bdb2aa1a90cfe55d1922c
- Internal name: "uRhs2 40 9"
- Graphics microcode: F3DEX2

---

## 7. Thread Layout

| Thread ID | Priority | Entry Point | Stack | Description |
|-----------|----------|-------------|-------|-------------|
| 1 | 1 | idle_thread_entry (0x800021A4) | 0x8002F660 | Idle thread |
| 4 | 4 | __scMain (0x8000063C) | - | Scheduler thread |
| 5 | 5 | (render) | 0x80031D70 | Render thread |
| 6 | 6 | (init) | 0x80030150 | Init thread |
| 7 | 7 | (game) | 0x80030AB0 | Main game thread |
| 8 | 5 | audio_thread_entry (0x800024FC) | 0x8002F7F0 | Audio thread |

---

## 8. References

- `symbol_addrs.us.txt` - 1165+ symbol definitions
- `hardware_regs.ld` - N64 MMIO addresses
- `include/game/structs.h` - Structure definitions
- `src/game/game.c` - Global variable usage
- `splat.us.yaml` - ROM segment configuration
- Arcade source: `reference/repos/rushtherock/game/game.c`

---

*Document generated from Rush 2049 N64 decompilation project analysis*
*Last updated: 2025-12-29*
