# Memory Management and Heap System Documentation

## Overview

This document maps the memory management systems between the N64 version of San Francisco Rush 2049 and the Rush The Rock arcade source code. While both platforms share similar game logic, their memory management differs significantly due to hardware constraints.

---

## Memory Map Comparison

### N64 Memory Layout

```
+------------------+ 0x80000000 (KSEG0 - Cached, Unmapped)
|     Header       | 0x40 bytes
+------------------+ 0x80000040
|      IPL3       | Boot code (0x1000 bytes)
+------------------+ 0x80000400 (entry_VRAM)
|   Entry Point   | entrypoint, BSS clear, stack setup
+------------------+ ~0x80001000
|                  |
|   Static Code   | Compiled C code (~57KB)
|  (libultra, OS) | libc, libm, libultra, scheduler
|                  |
+------------------+ ~0x80008000
|   Static Data   | Initialized data, rodata
+------------------+ ~0x80020000
|      BSS        | Uninitialized globals
+------------------+ ~0x80086A50 (D_80086A50)
|                  |
| Decompressed    | 647KB of game code decompressed
|   Game Code     | from ROM at boot (0xB0CB10)
|                  | Contains rendering, physics, game logic
|                  |
+------------------+ ~0x80124AF0
|   Game Data     | Player state, car data, track data
|   Structures    | Object pools, effect buffers
+------------------+ ~0x80160000
|                  |
|   Heap Space    | Dynamic allocations:
|                  | - Huffman tables (~16KB)
|                  | - Audio buffers
|                  | - Texture streaming
|                  |
+------------------+ ~0x80400000 (4MB RDRAM typical)
|   Thread Stacks | Each thread has dedicated stack
+------------------+
```

### Arcade Memory Layout (Rush The Rock)

```
+------------------+ BSS End
|                  |
|   Memory Pool   | Linear allocator via mem_pool
|   (AllocMem)    | All game allocations from here
|                  |
+------------------+ mem_pool_start (net_mem_start + NET_MEM_BYTES)
|   Network Mem   | 30KB for network subsystem
+------------------+ net_mem_start (prc_extend_bss)
|      BSS        | Uninitialized globals
+------------------+
|   Static Data   | Initialized data
+------------------+
|      Code       | Game code, GUTS library
+------------------+ Low memory
```

---

## Memory Regions Detail

### N64 Static Code Region (0x80000400 - 0x80086A50)

| Address Range | Size | Content |
|---------------|------|---------|
| 0x80000400-0x80001050 | 3KB | Entry point, scheduler |
| 0x80001050-0x800021F0 | 7KB | Scheduler, VI timing |
| 0x800021F0-0x80002CF0 | 3KB | Display, viewport |
| 0x80002CF0-0x80003140 | 1KB | Init, thread setup |
| 0x80003140-0x80003390 | 1KB | DMA wrappers |
| 0x800034A0-0x80005610 | 9KB | libm (math functions) |
| 0x80005610-0x80007600 | 8KB | DEFLATE inflate |
| 0x80006A00-0x80008000 | 6KB | libultra OS |
| 0x80008000-0x80086A50 | 500KB | More libultra, libgcc |

### N64 Decompressed Game Code Region (0x80086A50 - 0x80124AF0)

| Address | Size | Function | Description |
|---------|------|----------|-------------|
| 0x80087A08 | 10KB | render_large | Major rendering function |
| 0x80099BFC | 10KB | render_object | 3D model rendering (G_DL) |
| 0x800A04C4 | 3KB | render_scene | Viewport/camera setup |
| 0x800B358C | - | sound_stop | Audio stop |
| 0x800B37E8 | - | sound_control | Audio control |
| 0x800B4200 | - | slot_state_setup | Player state |
| 0x800C9AE0 | - | game_mode_handler | Mode handling |
| 0x800CA3B4 | 3KB | playgame_state_change | Game state changes |
| 0x800DB81C | 2KB | race_state_update | Race logic |
| 0x800FD464 | 704B | game_loop | Main per-frame loop |

### N64 Key Global Variables

| Address | Name | Size | Purpose |
|---------|------|------|---------|
| 0x801146EC | gstate | 1 byte | Game state machine (ATTRACT, PLAYGAME, etc.) |
| 0x80142AFC | frame_counter | 4 bytes | Frame counter |
| 0x801427A8 | state_object_ptr | 4 bytes | Current state object pointer |
| 0x80144D60 | array_ptr | - | Array management |
| 0x80152818 | car_array | ~2KB | Array of car data structures |
| 0x80156CF0 | player_status | ~256B | Player status bytes |
| 0x80159788 | object_list | - | Scene object array |
| 0x8015A118 | player_array | ~1KB | Player data array |
| 0x801461D0 | game_struct | ~4KB | Main game structure (160 references) |
| 0x801174B4 | state_flags | 4 bytes | Game state bitmask (110 references) |

---

## Allocation Functions

### Arcade Memory Allocator (LIB/mem.c)

The arcade uses a simple linear (bump) allocator:

```c
// Global state
U32   *mem_pool;        // Base of memory pool
int   mem_count;        // Current allocation offset
int   mem_lock_count;   // Locked allocation point
int   MEM_BYTES;        // Total pool size

// Allocate memory from pool
void *AllocMem(int amount) {
    void *base;
    if (amount & 7) amount += 8 - (amount & 7);  // 8-byte align
    base = (void *)&mem_pool[mem_count / 4];
    mem_count += amount;
    if (mem_count >= MEM_BYTES)
        FatalMsg("AllocMem: failed (%d) bytes", amount);
    return base;
}

// Get current allocation base
void *GetMemBase() {
    return (void *)&mem_pool[mem_count / 4];
}

// Bytes available
int BytesFree() {
    return MEM_BYTES - mem_count;
}

// Reset all allocations
void FreeAllMem() {
    mem_count = 0;
}

// Free to last lock point
void FreeUnlockedMem() {
    mem_count = mem_lock_count;
}

// Mark current point as lock
void LockMem() {
    mem_lock_count = mem_count;
}
```

### N64 Memory Allocation

The N64 version uses several allocation strategies:

#### 1. Huffman Table Allocator (inflate.c)

```c
static void *g_huft_base;    // 0x800354C4
static u32  g_huft_offset;   // 0x800354C8
static u32  g_huft_size;     // 0x800354CC

void huft_init(void *base, u32 size) {
    g_huft_base = base;
    g_huft_offset = 0;
    g_huft_size = size;
}

void *huft_alloc(u32 size) {
    void *ptr;
    if (g_huft_offset + size > g_huft_size)
        return NULL;
    ptr = (u8 *)g_huft_base + g_huft_offset;
    g_huft_offset += size;
    return ptr;
}
```

#### 2. Heap Allocator (referenced in inflate.c)

```c
extern void *heap_alloc(s32 unused, u32 size);  // Dynamic allocation
extern void heap_free(void *ptr);               // Free allocation
```

#### 3. Object Pool Allocator (game.c)

```c
extern void *object_pool[];          // 0x80150B70
extern void *pool_secondary[];       // Secondary pool
extern void *effect_pool_1;          // Effect pool 1
extern void *effect_pool_2;          // Effect pool 2
extern void *effect_pool_3;          // Effect pool 3

// Object allocation (called from game code)
void *object_alloc(s32 type) {
    // Allocates from object_pool based on type
    // Returns pointer to allocated object
}

void *object_create(s32 type) {
    // Higher-level wrapper
    // 1. Validates state
    // 2. Calls object_alloc
    // 3. Initializes object fields
    return object_alloc(type);
}
```

---

## Function Mapping Table

### Memory Allocation Functions

| N64 Address | N64 Function | Arcade Equivalent | Confidence |
|-------------|--------------|-------------------|------------|
| 0x80004D6C | huft_alloc | - (N64-specific) | N/A |
| 0x80004D98 | huft_build | zlib huft_build | High |
| inline | heap_alloc | AllocMem | Medium |
| inline | heap_free | FreeUnlockedMem | Medium |
| 0x800B???? | object_alloc | MBOX_NewObject | Medium |
| 0x800B42F0 | object_create | - | Low |

### Initialization Functions

| N64 Address | N64 Function | Arcade Equivalent | Confidence |
|-------------|--------------|-------------------|------------|
| 0x800020F0 | main | start() (init.c) | Medium |
| 0x800021A4 | idle_thread_entry | - (N64-specific) | N/A |
| 0x80002238 | game_init | init() (init.c:330) | Low-Medium |
| 0x800024FC | audio_thread_entry | - (N64-specific) | N/A |

**Note**: N64 `game_init` (0x80002238) is OS-level bootstrap (message queues, ROM decompression, thread spawning), NOT the same as arcade `game_init()` (game.c:494-601) which is gameplay state setup (NVRAM options, difficulty, HUD).

### Game Loop Functions

| N64 Address | N64 Function | Arcade Equivalent | Confidence |
|-------------|--------------|-------------------|------------|
| 0x800FD464 | game_loop | game() (game.c:714+) | High |
| 0x800C9AE0 | game_mode_handler | ProcessPDUs? | Medium |
| 0x800CA3B4 | playgame_state_change | playgame() | Medium |
| 0x800EDDC0 | attract_or_transition | attract() | Medium |
| 0x800DB81C | race_state_update | CheckCPs(), race logic | Medium |
| 0x800B37E8 | sound_control | audio control functions | High |

---

## Object Pools and Entity Management

### N64 Object Pools

```
Object Pool Layout (estimated from game.c analysis):

+------------------+ 0x80150B70 (object_pool)
| Pool Entries     | Array of object pointers
| [0] - [N]        | Each ~1780 bytes (0x6F4)
+------------------+
| Pool Secondary   | 0x80150BA0
+------------------+

Effect Pools:
+------------------+ effect_pool_1
| Particle FX      |
+------------------+ effect_pool_2
| Trail FX         |
+------------------+ effect_pool_3
| Misc FX          |
+------------------+

Effect State Buffer: effect_state_buffer[44]
- Cleared on state transitions
- Indexed by effect type
```

### Arcade Object Management

The arcade uses MBOX (Mathbox) for object management:

```c
// From visuals.c
static Skid gSkid[SKID_OBJS];           // Skid mark pool
static CarParts gCarParts[NCARS];       // Car part objects

// Object operations
gObjList[k] = MBOX_FindObject("partName");
obj = MBOX_NewObject(objIndex, pos, uvs, -1, 0);
MBOX_RemoveObject(obj);
ZOID_UpdateObject(objnum, pos, mat);
```

### Object Allocation Flow

```
N64 Object Allocation:
1. sync_acquire(object_sync)     // Thread-safe
2. object_alloc(type)            // Get from pool
3. Initialize fields:
   - *(u8*)(obj+2) = type       // Object type
   - *(u32*)(obj+0x6F4) = data  // Data block
4. state_set(state_ptr, obj, 0)  // Register
5. sync_release(object_sync)

Arcade Object Allocation:
1. MBOX_FindObject(name)         // Get object index
2. MBOX_NewObject(idx, pos, ...)  // Create instance
3. No explicit sync (single-threaded)
```

---

## DMA and Decompression Buffers

### N64 DMA System

```c
// DMA Message Queue (src/util/dma.c)
OSMesgQueue D_8002F190;         // DMA sync queue
OSMesg      D_8002F1A8[1];      // Message buffer
u8          D_8002B030;          // Initialized flag

void dma_queue_init(void) {
    D_8002B030 = 1;
    osCreateMesgQueue(&D_8002F190, D_8002F1A8, 1);
    osJamMesg(&D_8002F190, NULL, 0);  // Signal ready
}

s32 dma_wait(s32 blocking) {
    if (!D_8002B030) dma_queue_init();
    // Wait or poll for DMA completion
}

void dma_signal(void) {
    osJamMesg(&D_8002F190, NULL, 0);
}
```

### Inflate Double-Buffer System

```c
// From src/inflate/inflate.c
#define INFLATE_WINDOW_SIZE 4096      // 4KB per buffer

// Double buffers for async ROM reads
u8 g_buffer_a[4096];   // 0x80084A50
u8 g_buffer_b[4096];   // 0x80085A50

u32 g_buffer_toggle;   // Which buffer is active

// Async read while processing other buffer
void refill_buffer(void) {
    // Wait for pending I/O
    while (osRecvMesg(&g_mq, &msg, OS_MESG_NOBLOCK) == -1);

    // Toggle buffer
    buffer = (g_buffer_toggle == 1) ? g_buffer_b : g_buffer_a;
    g_buffer_toggle = !g_buffer_toggle;

    // Start async DMA for next chunk
    osInvalDCache(buffer, INFLATE_WINDOW_SIZE);
    dma_read_async(&g_dma_state, 0, 0, g_src, buffer,
                   INFLATE_WINDOW_SIZE, &g_mq);
    g_src += INFLATE_WINDOW_SIZE;

    // Update input pointers
    g_inptr = buffer;
    g_inend = buffer + INFLATE_WINDOW_SIZE;
}
```

### Decompression at Boot

```c
// From src/game/init.c - game_init (0x80002238)
#define ROM_COMPRESSED_DATA 0xB0CB10

void game_init(void *arg) {
    // Finalize DMA for first region
    dma_finalize(gDecompressedDataStart, size1);

    // Invalidate cache for decompression target
    osInvalDCache(gDecompressedDataEnd1, size2);

    // Decompress 647KB of game code from ROM
    inflate_entry((void *)ROM_COMPRESSED_DATA,
                  gDecompressedDataStart, 0);

    // Clear BSS
    bzero(gBssStart, bss_size);

    // Continue initialization...
}
```

---

## Thread Stack Usage

### N64 Thread Configuration

| Thread | ID | Priority | Stack Size | Stack Address | Purpose |
|--------|------|----------|------------|---------------|---------|
| Idle | 1 | 2 | 0x190 (400B) | gIdleThreadStack | Boot, then idle |
| Init | 6 | 4 | 0x960 (2400B) | gInitThreadStack | Game init, then frame update |
| Game | 7 | 5 | 0x12C0 (4800B) | gGameThreadStack | Main game loop |
| Audio | 8 | 3 | 0x960 (2400B) | gAudioThreadStack | Audio processing |
| Render | 5 | 7 | 0x960 (2400B) | gRenderThreadStack | Graphics rendering |

### Arcade Stack Configuration

```c
// From init.c
#define NET_MEM_BYTES   (30*1024)    // 30KB for network
#define GUTS_STACK_BYTES (32*1024)   // 32KB for GUTS/OS

// Memory layout at startup
net_mem_start = prc_extend_bss(0);
mem_pool_start = net_mem_start + NET_MEM_BYTES;

// Initialize allocator
InitMemHandler2(mem_pool_start, GUTS_STACK_BYTES);
```

---

## Audio Buffers

### N64 Audio System

```c
// From game.c externs
extern s16 *audio_output_buffer_left;   // Left channel
extern s16 *audio_output_buffer_right;  // Right channel
extern s32 audio_output_buffer_size;    // Buffer size
extern s32 audio_buffer_size_2;         // Secondary size

extern void *stream_buffers[2];         // Double buffers
extern s32 stream_active_buf;           // Active index
extern u32 stream_buffer_size;          // Size per buffer

// Audio allocation
extern void *audio_alloc(s32 size);
extern s32 sample_heap_remaining;
```

### Arcade Audio (CAGE system)

```c
// From moobs.c
U32 buffer_average;  // Rolling average of buffer state

// Buffer fullness check (0x8000 = 1/2 full)
buffer_average += (U32)aud_qlen();

// Dynamic adjustment based on buffer state
if (buffer_average > 0x30000) {
    // Reduce sample rate
} else if (buffer_average < 0x10000) {
    // Increase sample rate
}
```

---

## Key Differences Summary

| Aspect | N64 | Arcade |
|--------|-----|--------|
| **Allocator** | Multiple pools + heap | Single linear pool |
| **Threading** | 5 threads, preemptive | Single thread, cooperative |
| **Sync** | Message queues, mutexes | None needed |
| **Decompression** | DEFLATE + async DMA | Files loaded from HD |
| **Audio** | RSP/AI double-buffer | CAGE system |
| **Graphics** | RDP microcode | 3Dfx Glide API |
| **Object Mgmt** | Pool allocator | MBOX system |
| **State Machine** | Byte at 0x801146EC | Enum `gstate` |

---

## Memory Map Diagram

```
N64 RDRAM (4MB)                         Arcade Memory
================                        =============

0x80000000 +-------+                    Low Mem +-------+
           | IPL3  |                            | Code  |
0x80000400 +-------+                            |       |
           | Entry |                            +-------+
           | Code  |                            | Data  |
0x80001000 +-------+                            +-------+
           |       |                            | BSS   |
           | OS    |                            +-------+ <-- prc_extend_bss
           | Libs  |                            | Net   |
           |       |                            | 30KB  |
0x80086A50 +-------+ <-- D_80086A50             +-------+ <-- mem_pool_start
           |       |                            |       |
           | Game  |  <-- DEFLATE               | Heap  |
           | Code  |      from ROM              | Pool  |
           | 647KB |                            |       |
           |       |                            | (grow)|
0x80124AF0 +-------+                            |   v   |
           |       |                            |       |
           | Game  |                            +-------+
           | Data  |                            | Stack |
           |       |                            | 32KB  |
0x80160000 +-------+                    High    +-------+
           | Heap  |
           +-------+
           |Thread |
           |Stacks |
0x80400000 +-------+ <-- 4MB limit
```

---

## References

- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - N64 game code
- `/home/cburnes/projects/rush2049-decomp/src/game/init.c` - N64 initialization
- `/home/cburnes/projects/rush2049-decomp/src/inflate/inflate.c` - Decompression
- `/home/cburnes/projects/rush2049-decomp/src/util/dma.c` - DMA wrappers
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/LIB/mem.c` - Arcade allocator
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/init.c` - Arcade init
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/game.c` - Arcade game loop
- `/home/cburnes/projects/rush2049-decomp/symbol_addrs.us.txt` - Symbol addresses
