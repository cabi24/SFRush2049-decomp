# Rush 2049 N64 Save/Load System

This document describes the save/load system used in San Francisco Rush 2049 for the Nintendo 64. The game uses the N64 Controller Pak (also known as the Memory Pak) to persist game data.

## Table of Contents

1. [Overview](#overview)
2. [Controller Pak Hardware](#controller-pak-hardware)
3. [PFS Filesystem Functions](#pfs-filesystem-functions)
4. [Save Data Structure](#save-data-structure)
5. [High Score Storage](#high-score-storage)
6. [Ghost/Replay Data Format](#ghostreplay-data-format)
7. [Settings Persistence](#settings-persistence)
8. [Memory Layout Diagrams](#memory-layout-diagrams)
9. [Key Functions and Addresses](#key-functions-and-addresses)
10. [Arcade Comparison](#arcade-comparison)
11. [Error Handling](#error-handling)

---

## Overview

Rush 2049 uses the N64 Controller Pak filesystem (PFS) API to save and load game data. The save system manages:

- **Game Options**: Sound settings, controller sensitivity, vibration
- **High Scores**: Per-track leaderboards with 10 entries each
- **Unlocks**: Cars, tracks, and extra content
- **Statistics**: Play time, races, wins, crashes, stunts
- **Ghost Replays**: Recorded lap data for time trial mode

The main save file uses 4 Controller Pak pages (1024 bytes), while ghost replay data uses a separate file with 8 pages (2048 bytes).

---

## Controller Pak Hardware

### Physical Specifications

```
Controller Pak (NUS-004)
+------------------------+
|  32KB SRAM             |
|  128 pages x 256 bytes |
|  Bank-switched access  |
|  Battery-backed        |
+------------------------+
```

| Property | Value |
|----------|-------|
| Total Capacity | 32,768 bytes (32KB) |
| Page Size | 256 bytes |
| Total Pages | 128 per bank |
| Blocks per Page | 8 |
| Block Size | 32 bytes |
| Max Files | 16 directory entries |

### Memory Organization

```
Controller Pak Memory Map
+------------------+--------+------------------+
| Region           | Pages  | Description      |
+------------------+--------+------------------+
| Page 0           | 1      | Status/ID page   |
| Page 1           | 1      | Label/metadata   |
| Pages 2-4        | 3      | Inode table      |
| Pages 5-7        | 3      | Directory table  |
| Pages 8-127      | 120    | User data        |
+------------------+--------+------------------+
```

---

## PFS Filesystem Functions

The N64 libultra SDK provides the osPfs* API for Controller Pak access. Rush 2049 uses these functions via decompiled implementations in `src/libultra/os_pfs*.c`.

### Core Functions

| Function | Source File | Purpose |
|----------|-------------|---------|
| `osPfsInitPak` | os_pfs.c | Initialize Controller Pak filesystem |
| `osPfsChecker` | os_pfs_check.c | Verify filesystem integrity |
| `osPfsFindFile` | os_pfs_find.c | Search for file by codes/names |
| `osPfsAllocateFile` | os_pfs_create.c | Create new file |
| `osPfsDeleteFile` | os_pfs_delete.c | Delete existing file |
| `osPfsReadWriteFile` | os_pfs.c | Read or write file data |
| `osPfsFreeBlocks` | os_pfs_free.c | Get available space |
| `osPfsFileState` | os_pfs_state.c | Get file information |

### OSPfs Structure

```c
typedef struct OSPfs {
    s32 status;              /* 0x00: Initialization status flags */
    OSMesgQueue *queue;      /* 0x04: Message queue for SI operations */
    s32 channel;             /* 0x08: Controller port (0-3) */
    u8 id[32];               /* 0x0C: Pack ID */
    u8 label[32];            /* 0x2C: User label */
    s32 version;             /* 0x4C: PFS version */
    s32 dir_size;            /* 0x50: Directory size */
    s32 inode_table;         /* 0x54: Inode table block location */
    s32 minode_table;        /* 0x58: Mirror inode table location */
    s32 dir_table;           /* 0x5C: Directory table block location */
    s32 inode_start_page;    /* 0x60: Starting page for inodes */
    u8 banks;                /* 0x64: Number of banks */
    u8 activebank;           /* 0x65: Currently active bank */
} OSPfs;  /* Size: 0x68 (104 bytes) */
```

### Directory Entry Structure

```c
typedef struct __OSDir {
    u32 game_code;           /* Game identifier */
    u16 company_code;        /* Company identifier */
    __OSInodeUnit start_page;/* First page of file */
    u8 status;               /* File status */
    u8 reserved;
    char game_name[16];      /* Game name string */
    char ext_name[4];        /* File extension */
    u16 data_sum;            /* Data checksum */
} __OSDir;  /* Size: 32 bytes */
```

### Inode Chain

Files are stored as linked lists of pages. Each inode entry points to the next page:

```
Inode Table Entry (2 bytes)
+--------+--------+
|  Bank  |  Page  |
+--------+--------+
  1 byte   1 byte

Special Values:
  0x0001 = End of file (EOF)
  0x0003 = Free page
```

---

## Save Data Structure

### File Identification

```c
#define SAVE_COMPANY_CODE       0x3031      /* "01" (Midway/Atari) */
#define SAVE_GAME_CODE          0x4E524855  /* "NRHU" (Rush 2049 US) */
#define SAVE_GAME_NAME          "RUSH2049"
#define SAVE_EXT_NAME           "SAVE"
#define GHOST_EXT_NAME          "GHOST"
```

### SaveData Structure (1024 bytes)

```c
typedef struct SaveData {
    /* Header - 8 bytes */
    u32             magic;              /* 0x52534856 "RSHV" */
    u16             version;            /* Version 1 */
    u16             checksum;           /* Adler-16 checksum */

    /* Game settings - 8 bytes */
    SaveOptions     options;

    /* Unlock state - 4 bytes */
    SaveUnlocks     unlocks;

    /* Statistics - 40 bytes */
    SaveStats       stats;

    /* High scores - 8 tracks x 116 bytes = 928 bytes */
    SaveTrackData   tracks[8];

    /* Ghost info headers - 4 x 12 bytes = 48 bytes */
    SaveGhostHeader ghosts[4];

} SaveData;  /* Total: ~1036 bytes, fits in 4 pages */
```

### Memory Layout Diagram

```
SaveData Memory Layout (4 pages = 1024 bytes)
+============================================+  Offset 0x000
|  Header (8 bytes)                          |
|  +------------------+--------------------+ |
|  | magic (4)        | version (2) | chk  | |
|  +------------------+--------------------+ |
+============================================+  Offset 0x008
|  SaveOptions (8 bytes)                     |
|  +-------+-------+-------+-------+         |
|  | sound | music | sfx   | sens  |         |
|  +-------+-------+-------+-------+         |
|  | vibra | auto  | cam   | hud   |         |
|  +-------+-------+-------+-------+         |
+============================================+  Offset 0x010
|  SaveUnlocks (4 bytes)                     |
|  +-------+-------+-------+-------+         |
|  | cars  | tracks| extras| diff  |         |
|  +-------+-------+-------+-------+         |
+============================================+  Offset 0x014
|  SaveStats (40 bytes)                      |
|  - total_play_time (4)                     |
|  - total_races (4)                         |
|  - total_wins (4)                          |
|  - total_crashes (4)                       |
|  - total_distance (4)                      |
|  - total_stunts (4)                        |
|  - best_stunt_score (4)                    |
|  - battle_kills (4)                        |
|  - battle_deaths (4)                       |
|  - battle_wins (4)                         |
+============================================+  Offset 0x03C
|  SaveTrackData[0] (116 bytes)              |
|  +--------------------------------------+  |
|  | SaveScore[0-9] (12 bytes each = 120) |  |
|  | - name[4]                            |  |
|  | - time (4)                           |  |
|  | - car_id, mirror, valid, pad         |  |
|  +--------------------------------------+  |
|  | best_lap (4)                         |  |
|  | track_id, num_valid, pad[2]          |  |
|  +--------------------------------------+  |
+============================================+
|  SaveTrackData[1-7]... (7 x 116 bytes)     |
+============================================+  Offset ~0x3C8
|  SaveGhostHeader[0-3] (48 bytes total)     |
|  - valid, track_id, car_id, mirror         |
|  - time (4)                                |
|  - data_size (2), checksum (2)             |
+============================================+
```

---

## High Score Storage

### Per-Track Score Table

Each track maintains a table of 10 high scores:

```c
typedef struct SaveScore {
    char        name[4];        /* 3-letter initials + null */
    u32         time;           /* Time in frames (60fps) */
    u8          car_id;         /* Car used (0-7) */
    u8          mirror;         /* Mirror mode flag */
    u8          valid;          /* Entry is valid */
    u8          pad;
} SaveScore;  /* 12 bytes */

typedef struct SaveTrackData {
    SaveScore   scores[10];     /* 120 bytes */
    u32         best_lap;       /* Best single lap time */
    u8          track_id;
    u8          num_valid;
    u8          pad[2];
} SaveTrackData;  /* 128 bytes */
```

### Time Format

Times are stored as frame counts at 60fps:

```
Frame Count to Time Conversion:
  total_hundredths = (frames * 100) / 60
  minutes = total_hundredths / 6000
  seconds = (total_hundredths / 100) % 60
  hundredths = total_hundredths % 100

Example:
  18000 frames = 5 minutes = 5:00.00
  3600 frames = 1 minute = 1:00.00
  60 frames = 1 second = 0:01.00
```

### Score Validation

```c
#define MIN_SCORE_TIME      (1 * 60 * 60)   /* 1 minute minimum */
#define MAX_SCORE_TIME      (10 * 60 * 60)  /* 10 minute maximum */
```

### Name Entry Character Set

```c
static const char sCharSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ _<";
                               // A-Z (0-25), Space (26), Delete (27), End (28)
```

---

## Ghost/Replay Data Format

Ghost replays record car position and orientation each frame for time trial comparison.

### Replay Frame Structure

```c
typedef struct ReplayFrame {
    /* Position - 6 bytes (16-bit fixed point) */
    s16     pos_x;          /* Position / 16.0 */
    s16     pos_y;
    s16     pos_z;

    /* Orientation - 8 bytes (compressed quaternion) */
    s16     quat_x;         /* Quaternion * 16384 */
    s16     quat_y;
    s16     quat_z;
    s16     quat_w;

    /* Velocity - 3 bytes (8-bit scaled) */
    s8      vel_x;          /* Velocity / 128.0 */
    s8      vel_y;
    s8      vel_z;

    /* Wheel state - 4 bytes */
    u8      wheel_rot[4];   /* Per-wheel rotation */

    /* Controls - 2 bytes */
    s8      steer_angle;
    u8      flags;

    /* Padding - 2 bytes */
    u8      pad[2];
} ReplayFrame;  /* 24 bytes per frame */
```

### Compression

- Recording rate: Every 2 frames (30fps effective)
- Position: 16-bit fixed point (scale: 16.0)
- Orientation: Compressed quaternion (scale: 16384.0)
- Velocity: 8-bit scaled (scale: 128.0)

### Ghost File Structure

```c
typedef struct SaveGhostHeader {
    u8          valid;          /* Ghost is valid */
    u8          track_id;       /* Track identifier */
    u8          car_id;         /* Car used */
    u8          mirror;         /* Mirror mode */
    u32         time;           /* Total lap time */
    u16         data_size;      /* Compressed frame data size */
    u16         checksum;       /* Data checksum */
} SaveGhostHeader;  /* 12 bytes */
```

### Ghost Memory Budget

```
Ghost Data Calculation:
  Max frames: 5 minutes * 60fps = 18000 frames
  Sample rate: Every 2 frames = 9000 samples
  Frame size: 24 bytes
  Uncompressed: 9000 * 24 = 216,000 bytes

  Compressed (target): ~2000 bytes per ghost
  Available pages: 8 pages = 2048 bytes
  Ghosts per file: 1 (detailed) or 4 (header only)
```

---

## Settings Persistence

### SaveOptions Structure

```c
typedef struct SaveOptions {
    u8  sound_mode;         /* 0=Mono, 1=Stereo, 2=Surround */
    u8  music_volume;       /* 0-100 */
    u8  sfx_volume;         /* 0-100 */
    u8  controller_sens;    /* Controller sensitivity 0-100 */
    u8  vibration;          /* Rumble Pak enabled */
    u8  auto_trans;         /* Automatic transmission */
    u8  camera_mode;        /* Default camera view */
    u8  hud_mode;           /* HUD display mode */
} SaveOptions;  /* 8 bytes */
```

### Default Values

```c
gSave.data.options.sound_mode = SOUND_STEREO;  /* 1 */
gSave.data.options.music_volume = 80;
gSave.data.options.sfx_volume = 80;
gSave.data.options.controller_sens = 50;
gSave.data.options.vibration = 1;
gSave.data.options.auto_trans = 1;
gSave.data.options.camera_mode = 0;
gSave.data.options.hud_mode = 0;
```

### Unlock State

```c
typedef struct SaveUnlocks {
    u8  cars;           /* Bitmask of unlocked cars (0-7) */
    u8  tracks;         /* Bitmask of unlocked tracks (0-7) */
    u8  extras;         /* Extra content flags */
    u8  difficulty;     /* Highest difficulty beaten */
} SaveUnlocks;  /* 4 bytes */

/* Default: First 4 cars and tracks unlocked */
gSave.data.unlocks.cars = 0x0F;    /* Cars 0-3 */
gSave.data.unlocks.tracks = 0x0F; /* Tracks 0-3 */
```

---

## Memory Layout Diagrams

### Controller Pak Page Allocation

```
Controller Pak (32KB = 128 pages)
+=========+=========+=========+
| System  | Files   | Free    |
| Pages   | (Rush)  | Space   |
+=========+=========+=========+
|  0-7    |  8-11   |  12-127 |
|  2KB    |  1KB    |  29KB   |
+---------+---------+---------+

Rush 2049 Files:
  RUSH2049.SAVE  = 4 pages (1024 bytes)
  RUSH2049.GHOST = 8 pages (2048 bytes) [optional]
```

### Save File Page Layout

```
Page 0 (256 bytes)               Page 1 (256 bytes)
+------------------------+       +------------------------+
| Header (8)             |       | SaveTrackData[2]       |
| SaveOptions (8)        |       | (continued)            |
| SaveUnlocks (4)        |       +------------------------+
| SaveStats (40)         |       | SaveTrackData[3]       |
+------------------------+       +------------------------+
| SaveTrackData[0]       |
| (116 bytes)            |       Page 2 (256 bytes)
+------------------------+       +------------------------+
| SaveTrackData[1]       |       | SaveTrackData[4-5]     |
| (partial)              |       +------------------------+
+------------------------+
                                 Page 3 (256 bytes)
                                 +------------------------+
                                 | SaveTrackData[6-7]     |
                                 +------------------------+
                                 | SaveGhostHeader[0-3]   |
                                 | (48 bytes)             |
                                 +------------------------+
                                 | Unused padding         |
                                 +------------------------+
```

---

## Key Functions and Addresses

### Save System Functions (src/game/save.c)

| Function | Description |
|----------|-------------|
| `save_init()` | Initialize save system, detect paks, load data |
| `save_reset()` | Reset to default values |
| `save_shutdown()` | Auto-save if enabled, cleanup |
| `save_detect_pak(controller)` | Detect and initialize Controller Pak |
| `save_check_all_paks()` | Scan all 4 controller ports |
| `save_write()` | Write save data to active pak |
| `save_read()` | Read save data from active pak |
| `save_calc_checksum(data, size)` | Calculate Adler-16 checksum |

### PFS Functions (src/libultra/os_pfs*.c)

| Function | ROM Address | Source File |
|----------|-------------|-------------|
| `osPfsInitPak` | 0x80009C10 | os_pfs.c |
| `__osPfsGetStatus` | 0x80009E18 | os_pfs.c |
| `__osPfsSelectBank` | 0x8000E850 | os_pfs.c |
| `osPfsReadWriteFile` | 0x8000A970 | os_pfs.c |
| `osPfsFreeBlocks` | 0x8000B240 | os_pfs_free.c |
| `osPfsChecker` | 0x8000B3E0 | os_pfs_check.c |
| `osPfsFindFile` | 0x8000A350 | os_pfs_find.c |
| `osPfsAllocateFile` | 0x8000ACA4 | os_pfs_create.c |
| `osPfsDeleteFile` | 0x8000A700 | os_pfs_delete.c |
| `osPfsFileState` | 0x8000A520 | os_pfs_state.c |
| `__osContRamRead` | 0x8000E8D0 | os_pfs_rw.c |
| `__osContRamWrite` | 0x8000F680 | os_pfs_rw.c |

### Key Global Variables

| Variable | Type | Description |
|----------|------|-------------|
| `gSave` | SaveState | Global save state and data |
| `gHiScore` | HiScoreState | High score tables and entry state |
| `gReplay` | ReplayState | Replay recording/playback state |
| `gGhost` | GhostState | Ghost lap comparison state |

---

## Arcade Comparison

The arcade version (Rush The Rock) uses NVRAM/EEPROM instead of removable media:

### Arcade NVRAM System

```c
/* From reference/repos/rushtherock/OS/eer_defs.h */
#define EEPROM_SIZE     2048        /* 2KB EEPROM */
#define STRIDE          2           /* Addressing shift */
#define MAX_STAT        70          /* Statistics entries */

/* High score table location */
#define TABLE_BASE      ((U32 *)(BRAM_BASE + 8*1024L))
#define TABLE_SIZE      (sizeof(HiScore) * NSCORES)
#define NSCORES         100         /* 100 scores per track (vs 10 on N64) */
```

### Key Differences

| Feature | N64 | Arcade |
|---------|-----|--------|
| Storage Medium | Controller Pak (32KB) | Battery-backed RAM |
| Scores per Track | 10 | 100 |
| Name Length | 3 characters | 8 characters |
| Ghost Replays | Stored on pak | N/A (no ghosts) |
| Unlock System | Yes | No (all unlocked) |
| Statistics | Comprehensive | Coin counter focused |

### Arcade HiScore Structure

```c
/* From reference/repos/rushtherock/game/attract.h */
typedef struct HiScore {
    U32     score;          /* Time in milliseconds */
    char    name[9];        /* 8 chars + null (NLENGTH=9) */
    S8      deaths;         /* Crash count */
    U8      mirror;         /* Mirror mode flag */
    S8      car;            /* Car type + node in high nibble */
} HiScore;
```

### Arcade NVRAM Functions

```c
/* From reference/repos/rushtherock/OS/eer_defs.h */
int eer_gets(unsigned int stat_num);        /* Read statistic */
int eer_puts(unsigned int stat_num, val);   /* Write statistic */
int eer_incs(unsigned int stat_num, val);   /* Increment statistic */
int eer_hstw(unsigned int rank, struct hst_ent *entptr, unsigned int table);
                                            /* Write high score entry */
void eer_write_byte(U8 *where, U8 *unlock, int what);  /* Low-level write */
```

---

## Error Handling

### PFS Error Codes

```c
#define PFS_ERR_SUCCESS         0   /* No error */
#define PFS_ERR_NOPACK          1   /* No Controller Pak inserted */
#define PFS_ERR_NEW_PACK        2   /* Controller Pak changed */
#define PFS_ERR_INCONSISTENT    3   /* Need to run checker */
#define PFS_ERR_CONTRFAIL       4   /* Controller overrun error */
#define PFS_ERR_INVALID         5   /* Invalid parameter */
#define PFS_ERR_BAD_DATA        6   /* Bad data read */
#define PFS_ERR_FULL            7   /* No free pages */
#define PFS_ERR_DIR_FULL        8   /* No free directory entries */
#define PFS_ERR_EXIST           9   /* File already exists */
#define PFS_ERR_ID_FATAL        10  /* Dead Controller Pak */
#define PFS_ERR_DEVICE          11  /* Wrong device type */
```

### Save System Error Codes

```c
#define SAVE_OK                 0   /* Success */
#define SAVE_ERR_NO_PAK         1   /* No Controller Pak */
#define SAVE_ERR_BAD_PAK        2   /* Controller Pak error */
#define SAVE_ERR_CORRUPT        3   /* Data corrupted */
#define SAVE_ERR_NO_SPACE       4   /* Not enough space */
#define SAVE_ERR_NO_FILE        5   /* File not found */
#define SAVE_ERR_WRITE_FAIL     6   /* Write failed */
#define SAVE_ERR_READ_FAIL      7   /* Read failed */
#define SAVE_ERR_INIT_FAIL      8   /* PFS init failed */
```

### Checksum Validation

The save system uses a modified Adler-16 checksum:

```c
u16 save_calc_checksum(void *data, s32 size) {
    u8 *bytes = (u8*)data;
    u32 a = 1;
    u32 b = 0;
    s32 i;

    for (i = 0; i < size; i++) {
        a = (a + bytes[i]) % 251;
        b = (b + a) % 251;
    }

    return (u16)((b << 8) | a);
}
```

### Recovery Procedure

1. Detect pak on each controller port
2. Call `osPfsChecker()` to verify filesystem
3. Attempt to find existing save file
4. Validate magic number and checksum
5. If invalid, reset to defaults

---

## References

- `src/game/save.c` - Main save system implementation
- `src/game/hiscore.c` - High score table management
- `src/game/replay.c` - Ghost/replay recording
- `src/libultra/os_pfs*.c` - Controller Pak filesystem
- `include/PR/os_pfs.h` - PFS structure definitions
- `include/game/save.h` - Save data structures
- `include/game/hiscore.h` - High score structures
- `include/game/replay.h` - Replay structures
- `reference/repos/rushtherock/game/hiscore.c` - Arcade reference
- `reference/repos/rushtherock/OS/eer_defs.h` - Arcade NVRAM definitions
