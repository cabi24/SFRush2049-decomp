# Rush 2049 N64 Memory Management and Heap System

This document provides comprehensive documentation of the memory management and heap systems used in San Francisco Rush 2049 for Nintendo 64, including comparisons to the Rush The Rock arcade source code.

---

## 1. N64 Memory Constraints

### Hardware Specifications

| Component | Size | Description |
|-----------|------|-------------|
| Base RDRAM | 4 MB | Standard N64 configuration |
| Expansion Pak | +4 MB | Optional expansion (8 MB total) |
| TMEM | 4 KB | Texture memory (on-chip RDP) |
| RSP DMEM | 4 KB | RSP data memory |
| RSP IMEM | 4 KB | RSP instruction memory |
| D-Cache | 8 KB | CPU data cache |
| I-Cache | 16 KB | CPU instruction cache |

### Rush 2049 Memory Usage

Based on analysis of the ROM and decompiled code:

| Region | Start | End | Size | Purpose |
|--------|-------|-----|------|---------|
| OS/System | 0x80000000 | 0x80000400 | 1 KB | OS constants, TV type, ROM base |
| Static Code | 0x80000400 | 0x8000F700 | ~61 KB | libultra, libc, libm, inflate |
| Static Data | 0x80010000 | 0x80037DB0 | ~159 KB | Initialized data, constants |
| Static BSS | 0x80037DB0 | 0x80086A50 | ~318 KB | Thread stacks, OS structures |
| Game Code | 0x80086A50 | 0x80124AF0 | ~632 KB | Decompressed game logic (DEFLATE) |
| Game Data | 0x80124AF0 | 0x8017A640 | ~346 KB | Runtime structures, pools |
| Heap/Stack | 0x8017A640 | 0x80400000 | ~2.4 MB | Dynamic allocations |

**Total Static Usage**: ~1.52 MB (without Expansion Pak support evident)

---

## 2. Heap and Pool Implementations

### 2.1 Huffman Table Allocator (Inflate)

Used during DEFLATE decompression for Huffman lookup tables.

**Source**: `/home/cburnes/projects/rush2049-decomp/src/inflate/inflate.c`

```c
/* Global state */
static void *g_huft_base;     /* 0x800354C4 - Pool base address */
static u32   g_huft_offset;   /* 0x800354C8 - Current offset */
static u32   g_huft_size;     /* 0x800354CC - Total pool size */

#define INFLATE_TABLE_SIZE 0x4000  /* 16 KB for Huffman tables */

/* Initialize allocator */
void huft_init(void *base, u32 size) {
    g_huft_base = base;
    g_huft_offset = 0;
    g_huft_size = size;
}

/* Bump allocator - no free, reset only */
void *huft_alloc(u32 size) {
    void *ptr;
    if (g_huft_offset + size > g_huft_size)
        return NULL;
    ptr = (u8 *)g_huft_base + g_huft_offset;
    g_huft_offset += size;
    return ptr;
}
```

**Usage Pattern**:
- Allocated at boot during ROM decompression
- Can use static address (0x803FD120) or heap allocation
- Reset between decompression operations
- ~16 KB required for full DEFLATE tables

### 2.2 Audio Sample Heap

Manages memory for sound samples and streaming buffers.

**Source**: `/home/cburnes/projects/rush2049-decomp/src/game/game.c`

```c
/* Externs from game.c */
extern s32 sample_heap_remaining;  /* Available heap space */
extern void *audio_alloc(s32 size); /* Audio memory allocator */

/* Audio buffer globals */
extern s16 *audio_output_buffer_left;   /* Left channel output */
extern s16 *audio_output_buffer_right;  /* Right channel output */
extern s32 audio_output_buffer_size;    /* Buffer size in samples */

/* Streaming double-buffers */
extern void *stream_buffers[2];   /* Two buffers for streaming */
extern s32 stream_active_buf;     /* Currently active buffer (0 or 1) */
extern u32 stream_buffer_size;    /* Size per buffer */
```

**Allocation Pattern** (from game.c lines 28930-28938):
```c
/* Check heap space before allocation */
if (sampleSize > sample_heap_remaining) {
    return NULL;  /* Out of memory */
}

/* Allocate with 16-byte alignment */
sample_heap_remaining -= (sampleSize + 15) & ~15;
```

### 2.3 Object Pool Allocator

Fixed-size object pool for game entities.

**Source**: `/home/cburnes/projects/rush2049-decomp/src/game/game.c`

**Pool Structure** (lines 4870-4900):
```c
/* Pool control structure layout:
 *   offset 0x00: type/flags
 *   offset 0x04: count (number of entries)
 *   offset 0x08: stride (bytes per entry)
 *   offset 0x0C: base (pointer to pool array)
 *   offset 0x10: active list head
 *   offset 0x14: free list head
 */

extern void *object_pool[];        /* @ 0x80159788 */
extern void *pool_secondary[];     /* Secondary pool area */
extern u8   pool_storage_buffer[]; /* Actual storage */

/* Initialize pool with linked free list */
void pool_linked_list_init(void *a0) {
    s32 *base;
    s32 count, stride;
    s32 i;

    count = *(s32*)((u8*)a0 + 0x04);
    stride = *(s32*)((u8*)a0 + 0x08);
    base = *(s32**)((u8*)a0 + 0x0C);

    /* Link all entries into free list */
    for (i = 0; i < count - 1; i++) {
        base[0] = (s32)&base[stride / 4];
        base = (s32*)((u8*)base + stride);
    }
    base[0] = 0;  /* Last entry points to NULL */

    *(s32**)((u8*)a0 + 0x14) = *(s32**)((u8*)a0 + 0x0C);  /* free = base */
}
```

**Pool Reset** (lines 4918-4949):
```c
void pool_system_reset(s32 slot, void *data, s32 size) {
    void *node;

    /* Remove all active objects */
    node = *(void**)(&object_pool[0x10]);
    while (node != NULL) {
        audio_update(*(s32*)((u8*)node + 0x0C));
        path_follow(&object_pool[0], node);
        node = *(void**)(&object_pool[0x10]);
    }

    /* Reinitialize pool */
    object_pool[0] = 1;                           /* type = 1 */
    *(s32*)(&object_pool[0x08]) = 36;            /* stride = 36 bytes */
    *(s32*)(&object_pool[0x04]) = 100;           /* count = 100 entries */
    *(void**)(&object_pool[0x0C]) = &pool_storage_buffer[0];

    pool_linked_list_init(&object_pool[0]);

    /* Clear secondary pool (2208 bytes) */
    memset_custom(&pool_secondary[0], 0, 2208);
}
```

### 2.4 Particle/Effect Pools

Pre-allocated arrays for particle effects.

```c
/* From game.c externs */
extern void *effect_pool_1;      /* Effect pool 1 */
extern void *effect_pool_2;      /* Effect pool 2 */
extern void *effect_pool_3;      /* Effect pool 3 */
extern u8 effect_state_buffer[]; /* Effect state (44 bytes) */
extern s32 particle_pool_index;  /* Current particle index */

/* Particle pool at 0x80162000 */
s32 *particlePool = (s32 *)0x80162000;
/* 64 entries, 16 s32s (64 bytes) per particle */

/* Circular buffer allocation */
s32 *particle = &particlePool[particleIndex * 16];
particle_pool_index = (particleIndex + 1) % 64;
```

### 2.5 Entity Slot Allocator

Table-based allocation for game entities.

```c
/* From game.c line 6990-7015 */
void entity_slot_alloc(s16 typeId, void *data, s32 param, void *callback) {
    s32 *entityPool = (s32 *)0x80157000;
    s32 *poolIndex = (s32 *)0x80157400;
    s32 i;

    /* Search for free slot (entry[5] == 0xFFFF) */
    for (i = 0; i < 64; i++) {
        s32 *slot = (s32 *)(entityPool[i]);
        if (slot == NULL || slot[5] == 0xFFFF) {
            /* Found free slot */
            entityPool[i] = (s32)entity;
            break;
        }
    }

    if (i >= 64) {
        return;  /* Pool full */
    }

    /* Clear entity memory and initialize */
    /* ... */
    (*poolIndex)++;
}
```

---

## 3. Display List Allocation

### 3.1 Display List Buffer Management

**Source**: `/home/cburnes/projects/rush2049-decomp/src/game/gfx.c`

```c
/* Global display list pointers */
extern Gfx *gDisplayListHead;  /* 0x800354C4 - Buffer start */
extern Gfx *gDisplayListEnd;   /* 0x800354CC - Buffer end */
extern u32 gDisplayListSize;   /* 0x800354C8 - Current offset */

/* Initialize display list buffer */
void gfx_init_dl(Gfx *start, Gfx *end) {
    gDisplayListHead = start;
    gDisplayListEnd = end;
    gDisplayListSize = 0;
}

/* Allocate space in display list (bump allocator) */
Gfx *gfx_alloc_dl(u32 size) {
    Gfx *result;
    gDisplayListSize += size;
    result = gDisplayListHead + (gDisplayListSize - size);
    return result;
}
```

**Usage Pattern**:
- Display list buffer is pre-allocated in BSS
- Reset each frame before building new command list
- Simple bump allocation (no fragmentation)
- Pointer wraps or resets at frame end

### 3.2 RDP Command Building

Display lists contain RDP graphics commands built inline:

```c
/* From game.c - macro definitions */
#define gDPSetTile(gfx, fmt, size, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts)

/* Texture loading to TMEM */
void texture_load_tmem(s16 texId, s32 param, s32 flags) {
    s32 tmemAddr = (param & 0x1FF) * 8;

    /* G_SETTIMG - set texture image address */
    /* G_LOADBLOCK - load texture into TMEM */
    dlPtr[1] = tmemAddr;
}
```

---

## 4. Texture Memory (TMEM) Management

### 4.1 TMEM Overview

- Total TMEM: 4 KB (4096 bytes)
- Shared between color and Z-buffer data
- Managed by RDP hardware, software must partition

### 4.2 Texture Loading Pattern

```c
/* From game.c texture functions */
extern void texture_load(void *sndObj);
extern void texture_set(void);
extern void texture_bind(s32 slot, ...);
extern void texture_unbind(s32 slot, ...);
extern void texture_filter(s32 slot, s32 mode);
extern void texture_wrap(s32 slot, s32 clamp);

/* Texture parameter setup */
void texture_params_setup(s16 texId, s32 mode, s32 flags) {
    /* Configure texture format, size, wrap modes */
}

/* Load texture to TMEM */
void texture_load_tmem(s16 texId, s32 param, s32 flags) {
    s32 tmemAddr = (param & 0x1FF) * 8;  /* 8-byte aligned */
    /* Issue G_SETTIMG and G_LOADBLOCK commands */
}
```

---

## 5. DMA and Streaming Buffers

### 5.1 Inflate Double-Buffer System

**Source**: `/home/cburnes/projects/rush2049-decomp/src/inflate/inflate.c`

```c
#define INFLATE_WINDOW_SIZE 4096  /* 4 KB per buffer */

/* Double buffers for async ROM reads */
extern u8 g_buffer_a[INFLATE_WINDOW_SIZE];  /* 0x80084A50 */
extern u8 g_buffer_b[INFLATE_WINDOW_SIZE];  /* 0x80085A50 */

static u8 *g_inptr;        /* Current read position */
static u8 *g_inend;        /* Buffer end */
static u32 g_buffer_toggle; /* Active buffer (0 or 1) */
static void *g_src;        /* ROM source address */

/* Async read with double-buffering */
void refill_buffer(void) {
    OSMesg msg;
    u8 *buffer;

    /* Wait for pending I/O */
    while (osRecvMesg(&g_mq, &msg, OS_MESG_NOBLOCK) == -1);

    /* Toggle buffer */
    if (g_buffer_toggle == 1) {
        buffer = g_buffer_b;
        g_buffer_toggle = 0;
    } else {
        buffer = g_buffer_a;
        g_buffer_toggle = 1;
    }

    /* Invalidate cache and start async DMA */
    osInvalDCache(buffer, INFLATE_WINDOW_SIZE);
    dma_read_async(&g_dma_state, 0, 0, g_src, buffer,
                   INFLATE_WINDOW_SIZE, &g_mq);
    g_src = (u8 *)g_src + INFLATE_WINDOW_SIZE;

    /* Update input pointers */
    g_inptr = buffer;
    g_inend = buffer + INFLATE_WINDOW_SIZE;
}
```

### 5.2 Audio Streaming Buffers

```c
/* Audio stream double-buffers */
extern void *stream_buffers[2];
extern s32 stream_active_buf;
extern u32 stream_buffer_size;

/* Buffer management */
void audio_buffer_manage(void *buffer, s32 size) {
    s32 *bufferInfo = (s32 *)0x80161500;

    if (buffer == NULL || size <= 0) return;

    bufferInfo[0] = (s32)buffer;  /* Base address */
    bufferInfo[1] = size;          /* Total size */
    bufferInfo[2] = 0;             /* Read position */
    bufferInfo[3] = 0;             /* Write position */
    bufferInfo[4] = 0;             /* Underrun count */

    audio_buffer_registered = 1;
}
```

### 5.3 Network Buffers (4-player mode)

```c
/* Send/receive circular buffers */
u8 *sendBuffer = (u8 *)0x80164000;  /* 4 KB send */
u8 *recvBuffer = (u8 *)0x80165000;  /* 4 KB receive */
s32 bufferSize = 4096;

/* Message queuing */
void net_message_send(void *msg, s32 size) {
    if (*sendSize + size + 4 > bufferSize) {
        return;  /* Buffer full - drop message */
    }
    /* Write length-prefixed message */
}
```

---

## 6. Comparison to Arcade Memory Model

### 6.1 Arcade Memory Allocator

**Source**: `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/LIB/mem.c`

```c
U32  *mem_pool;           /* Base of memory pool */
int   mem_count;          /* Current allocation offset */
int   mem_lock_count;     /* Locked allocation point */
int   MEM_BYTES;          /* Total pool size */

/* Simple bump allocator */
void *AllocMem(int amount) {
    void *base;
    if (amount & 7) amount += 8 - (amount & 7);  /* 8-byte align */
    base = (void *)&mem_pool[mem_count / 4];
    mem_count += amount;
    if (mem_count >= MEM_BYTES)
        FatalMsg("AllocMem: failed (%d) bytes", amount);
    return base;
}

void *GetMemBase() {
    return (void *)&mem_pool[mem_count / 4];
}

int BytesFree() {
    return MEM_BYTES - mem_count;
}

void FreeAllMem() {
    mem_count = 0;
}

void FreeUnlockedMem() {
    mem_count = mem_lock_count;
}

void LockMem() {
    mem_lock_count = mem_count;
}

/* Pool initialization */
void InitMemHandler2(U32 first_avail_mem, U32 max_stack_bytes) {
    extern U32 INIT_SP[];
    MEM_BYTES = (U32)&INIT_SP - max_stack_bytes - first_avail_mem;
    mem_pool = (U32 *)first_avail_mem;
    mem_count = 0;
}
```

### 6.2 Key Differences

| Aspect | N64 (Rush 2049) | Arcade (Rush The Rock) |
|--------|-----------------|------------------------|
| **Primary Allocator** | Multiple specialized pools | Single linear pool |
| **Deallocation** | Pool-specific free lists | Lock/unlock points only |
| **Thread Safety** | Message queue sync | Single-threaded |
| **Alignment** | 16-byte (DMA) or 8-byte | 8-byte |
| **Fragmentation** | Pool-based avoids it | Linear avoids it |
| **Memory Pressure** | ~4 MB constraint | Arcade has more RAM |
| **Asset Loading** | ROM DMA + decompress | Hard drive direct load |

### 6.3 Arcade Memory Layout

```c
/* From init.c */
#define NET_MEM_BYTES    (30*1024)   /* 30 KB for network */
#define GUTS_STACK_BYTES (32*1024)   /* 32 KB for stack */

void start(void) {
    /* Extend BSS to get memory pool start */
    net_mem_start = (U32)prc_extend_bss(0);
    mem_pool_start = net_mem_start + NET_MEM_BYTES;
}

void init(void) {
    InitMemHandler2(mem_pool_start, GUTS_STACK_BYTES);
    /* Now AllocMem() works from mem_pool_start upward */
}
```

---

## 7. Memory Region Summary

### 7.1 Visual Memory Map

```
N64 RDRAM Layout (4 MB / 8 MB with Expansion Pak)
=================================================

0x80000000  +------------------------+
            | OS Constants (1 KB)    |
0x80000400  +------------------------+  <- entry_VRAM
            | Static Code (61 KB)    |
            | - Entry point          |
            | - libultra OS          |
            | - libc/libm            |
            | - inflate decompress   |
0x8000F700  +------------------------+
            | Static Data (159 KB)   |
            | - Constants            |
            | - Lookup tables        |
0x80037DB0  +------------------------+
            | Static BSS (318 KB)    |
            | - Thread structures    |
            | - Message queues       |
            | - Timer state          |
            | - Inflate buffers      |
0x80086A50  +------------------------+  <- D_80086A50 (game code dest)
            |                        |
            | Decompressed Game Code |
            |      (632 KB)          |
            | - render_object 10 KB  |
            | - game_loop 704 B      |
            | - physics, AI, etc.    |
            |                        |
0x80124AF0  +------------------------+
            |                        |
            | Game Data/BSS (346 KB) |
            | - Object pools         |
            | - Car arrays           |
            | - Effect pools         |
            | - Audio buffers        |
            | - Network buffers      |
            |                        |
0x8017A640  +------------------------+
            |                        |
            | Dynamic Heap (~2.4 MB) |
            | - Runtime allocations  |
            | - Thread stacks        |
            |                        |
0x80400000  +------------------------+  <- 4 MB limit
            | (Expansion Pak area)   |
            |    0x80400000 -        |
            |    0x80800000          |
0x80800000  +------------------------+  <- 8 MB limit
```

### 7.2 Key Memory Addresses

| Address | Name | Size | Purpose |
|---------|------|------|---------|
| 0x80084A50 | g_buffer_a | 4 KB | Inflate double-buffer A |
| 0x80085A50 | g_buffer_b | 4 KB | Inflate double-buffer B |
| 0x80086A50 | D_80086A50 | 632 KB | Decompressed game code destination |
| 0x801146EC | gstate | 1 byte | Game state (ATTRACT, PLAYGAME, etc.) |
| 0x80142AFC | frame_counter | 4 bytes | Global frame counter |
| 0x80149438 | gDisplayListPtr | 4 bytes | Current display list write pointer |
| 0x80152818 | car_array | 5.7 KB | 6 cars x 952 bytes |
| 0x80157000 | entityPool | - | Entity slot table |
| 0x801461D0 | game_struct | ~4 KB | Main game structure (160 refs) |
| 0x80161500 | audio_buffer_info | 20 bytes | Audio buffer control |
| 0x80162000 | particlePool | 4 KB | 64 particles x 64 bytes |
| 0x80164000 | net_send_buffer | 4 KB | Network send circular buffer |
| 0x80165000 | net_recv_buffer | 4 KB | Network receive circular buffer |
| 0x80170000 | audio_voice_table | - | Voice allocation table |

---

## 8. Thread Stack Allocation

| Thread | ID | Priority | Stack Size | Stack Address | Entry Point |
|--------|-----|----------|------------|---------------|-------------|
| Idle | 1 | 2 | 400 B | gIdleThreadStack | 0x800021A4 |
| Scheduler | 4 | - | - | (internal) | __scMain |
| Render | 5 | 7 | 2.4 KB | gRenderThreadStack | 0x800E7808 |
| Init | 6 | 4 | 2.4 KB | gInitThreadStack | game_init |
| Game | 7 | 5 | 4.8 KB | gGameThreadStack | game_thread_entry |
| Audio | 8 | 3 | 2.4 KB | gAudioThreadStack | 0x800AC75C |

---

## 9. Best Practices Observed

1. **Pool-Based Allocation**: Fixed-size pools avoid fragmentation
2. **Double-Buffering**: Used for DMA transfers (inflate, audio)
3. **Message Queue Sync**: Thread-safe buffer access via OS primitives
4. **Static Pre-allocation**: Most large buffers are BSS, not heap
5. **Circular Buffers**: Particle and network buffers wrap, no allocations
6. **Alignment**: 16-byte for DMA, 8-byte for general data
7. **Lock Points**: Arcade pattern of lock/unlock for bulk free

---

## 10. References

- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - Main game code
- `/home/cburnes/projects/rush2049-decomp/src/game/gfx.c` - Display list allocation
- `/home/cburnes/projects/rush2049-decomp/src/game/init.c` - Boot initialization
- `/home/cburnes/projects/rush2049-decomp/src/inflate/inflate.c` - DEFLATE with double-buffering
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/LIB/mem.c` - Arcade allocator
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/init.c` - Arcade init
- `/home/cburnes/projects/rush2049-decomp/symbol_addrs.us.txt` - Symbol addresses
- `/home/cburnes/projects/rush2049-decomp/docs/memory_map.md` - Detailed memory map

---

*Generated from Rush 2049 N64 decompilation project analysis*
*Last updated: 2025-12-30*
