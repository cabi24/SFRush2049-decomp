# Rush 2049 N64 Save System Documentation

This document provides comprehensive documentation for the N64 Controller Pak save/load system used in San Francisco Rush 2049.

## Table of Contents

1. [N64 Controller Pak Overview](#n64-controller-pak-overview)
2. [Save Data Structure](#save-data-structure)
3. [Arcade Comparison](#arcade-comparison)
4. [N64 PFS Functions Used](#n64-pfs-functions-used)
5. [Save File Format](#save-file-format)
6. [Ghost Data](#ghost-data)

---

## N64 Controller Pak Overview

The N64 Controller Pak (Memory Pak) is a 32KB removable storage device that plugs into the back of an N64 controller. Rush 2049 uses Nintendo's libultra `osPfs*` (Pak File System) functions to manage save data.

### Controller Pak Specifications

| Property | Value |
|----------|-------|
| Total Size | 32,768 bytes (32KB) |
| Number of Pages | 128 pages |
| Page Size | 256 bytes |
| File System | FAT-style with inode tables |
| Max Files | 16 directory entries |
| Banks | 1 (some third-party paks have multiple) |

### File System Structure

The Controller Pak uses a simple FAT-like filesystem:

```
Page 0:        Status/Signature page
Page 1:        Label and metadata
Pages 2-7:     Inode tables (file allocation)
Pages 8-127:   Data pages (available for files)
```

**Directory Entry Format (32 bytes):**
| Offset | Size | Field |
|--------|------|-------|
| 0x00 | 4 | Game Code (e.g., "NRHU" for Rush 2049 US) |
| 0x04 | 2 | Company Code (e.g., "01" for Midway) |
| 0x06 | 2 | Start Page (inode reference) |
| 0x08 | 16 | Game Name ("RUSH2049") |
| 0x18 | 4 | Extension Name ("SAVE" or "GHOST") |
| 0x1C | 4 | Reserved |

### Source Files

The Controller Pak implementation is split across multiple source files:

| File | Purpose | ROM Functions |
|------|---------|---------------|
| `src/libultra/os_pfs.c` | Core initialization and read/write | osPfsInitPak, osPfsReadWriteFile |
| `src/libultra/os_pfs_alloc.c` | Page allocation | __osPfsGetNextPage, osPfsReSizeFile |
| `src/libultra/os_pfs_check.c` | Filesystem integrity | osPfsChecker, osPfsRepairId |
| `src/libultra/os_pfs_create.c` | File creation | osPfsAllocateFile, __osPfsCreateDirEntry |
| `src/libultra/os_pfs_delete.c` | File deletion | osPfsDeleteFile, __osPfsReleasePages |
| `src/libultra/os_pfs_find.c` | File lookup | osPfsFindFile |
| `src/libultra/os_pfs_free.c` | Free space | osPfsFreeBlocks |
| `src/libultra/os_pfs_rw.c` | Low-level I/O | __osContRamRead, __osContRamWrite |
| `src/libultra/os_pfs_state.c` | File info | osPfsFileState |
| `src/game/save.c` | Game-specific save logic | save_*, ghost_* functions |

---

## Save Data Structure

Rush 2049 stores the following data types on the Controller Pak:

### Main Save File

**File Identification:**
| Field | Value |
|-------|-------|
| Company Code | `0x3031` ("01" - Midway) |
| Game Code | `0x4E524855` ("NRHU" - Rush 2049 US) |
| Game Name | `"RUSH2049"` |
| Extension | `"SAVE"` |
| File Size | 1024 bytes (4 pages) |

### Save Data Contents

From `include/game/save.h` and `src/game/save.c`:

#### SaveData Structure (1024 bytes total)

```c
typedef struct SaveData {
    /* Header (8 bytes) */
    u32             magic;          /* 0x52534856 "RSHV" */
    u16             version;        /* Current: 1 */
    u16             checksum;       /* Adler-16 checksum */

    /* Game settings (8 bytes) */
    SaveOptions     options;

    /* Unlock state (4 bytes) */
    SaveUnlocks     unlocks;

    /* Statistics (40 bytes) */
    SaveStats       stats;

    /* High scores per track (variable) */
    SaveTrackData   tracks[8];

    /* Ghost info headers (48 bytes) */
    SaveGhostHeader ghosts[4];
} SaveData;
```

#### SaveOptions (8 bytes)
```c
typedef struct SaveOptions {
    u8  sound_mode;        /* 0=Mono, 1=Stereo, 2=Surround */
    u8  music_volume;      /* 0-100 */
    u8  sfx_volume;        /* 0-100 */
    u8  controller_sens;   /* Controller sensitivity */
    u8  vibration;         /* Rumble Pak enabled */
    u8  auto_trans;        /* Auto transmission */
    u8  camera_mode;       /* Default camera */
    u8  hud_mode;          /* HUD display mode */
} SaveOptions;
```

#### SaveUnlocks (4 bytes)
```c
typedef struct SaveUnlocks {
    u8  cars;              /* Bitmask: UNLOCK_CAR_1 through UNLOCK_CAR_8 */
    u8  tracks;            /* Bitmask: UNLOCK_TRACK_1 through UNLOCK_TRACK_8 */
    u8  extras;            /* UNLOCK_MIRROR_MODE, UNLOCK_STUNT_ARENAS, etc. */
    u8  difficulty;        /* Highest difficulty beaten */
} SaveUnlocks;
```

**Unlock Flags:**
| Flag | Value | Description |
|------|-------|-------------|
| UNLOCK_CAR_* | 1<<n | Car n unlocked (n=0-7) |
| UNLOCK_TRACK_* | 1<<n | Track n unlocked (n=0-7) |
| UNLOCK_MIRROR_MODE | 0x01 | Mirror mode available |
| UNLOCK_STUNT_ARENAS | 0x02 | Stunt arenas available |
| UNLOCK_BATTLE_ARENAS | 0x04 | Battle arenas available |
| UNLOCK_ALL_MUSIC | 0x08 | All music tracks available |

#### SaveStats (40 bytes)
```c
typedef struct SaveStats {
    u32 total_play_time;   /* Total time in seconds */
    u32 total_races;       /* Races completed */
    u32 total_wins;        /* First place finishes */
    u32 total_crashes;     /* Crash count */
    u32 total_distance;    /* Distance traveled */
    u32 total_stunts;      /* Stunts performed */
    u32 best_stunt_score;  /* Best stunt session */
    u32 battle_kills;      /* Battle mode kills */
    u32 battle_deaths;     /* Battle mode deaths */
    u32 battle_wins;       /* Battle mode wins */
} SaveStats;
```

#### SaveTrackData (per track)
```c
typedef struct SaveTrackData {
    SaveScore scores[10];  /* Top 10 scores */
    u32       best_lap;    /* Best single lap time */
    u8        track_id;
    u8        num_valid;
    u8        pad[2];
} SaveTrackData;
```

#### SaveScore (16 bytes per entry)
```c
typedef struct SaveScore {
    char name[4];          /* Player initials (3 chars + null) */
    u32  time;             /* Time in frames (60fps) */
    u8   car_id;           /* Car used */
    u8   mirror;           /* Mirror mode flag */
    u8   valid;            /* Entry valid */
    u8   pad;
} SaveScore;
```

---

## Arcade Comparison

The arcade version (Rush The Rock) uses a fundamentally different save approach based on NVRAM.

### Arcade NVRAM System

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Storage Type | NVRAM (battery-backed RAM) | Controller Pak |
| Location | Main PCB | Controller port |
| Persistence | Always available | Removable |
| Size | ~64KB+ | 32KB |
| Access Method | Direct memory map | Serial Interface (SI) |
| Multi-user | Single cabinet | Per-controller (4 players) |

### Arcade NVRAM Functions

From `reference/repos/rushtherock/game/`:

| Function | Purpose |
|----------|---------|
| `eer_gets(EER_*)` | Read NVRAM value |
| `eer_puts(EER_*, val)` | Write NVRAM value |
| `eer_incs(EER_*, delta)` | Increment NVRAM counter |
| `eer_write_byte()` | Low-level byte write |
| `eer_hstr()` / `eer_hstw()` | High score table read/write |
| `eer_tally_hist()` | Update histogram statistics |

### Arcade NVRAM Variables (from `eer_defs.h`)

| Variable | Purpose |
|----------|---------|
| `EER_GMOPT` | Game options (difficulty, timer, etc.) |
| `EER_TRKOPT` | Track options |
| `EER_LAPOPT` | Lap count options |
| `EER_CNOPT` | Coin/credit options |
| `EER_GAME_CNT` | Total games played |
| `EER_HI_CNT` | High score entry count |
| `EER_LINKED` | Linked games count |
| `EER_MIRROR` | Mirror mode plays |
| `EER_NEWCOINS` | Coin counter |

### Arcade High Score Storage

From `reference/repos/rushtherock/game/hiscore.c`:

```c
/* High score table location in NVRAM */
#define TABLE_BASE  (BRAM_BASE + 0x1000)
#define TABLE_SIZE  (sizeof(HiScore) * NSCORES)  /* 100 entries per track */

/* HiScore structure (arcade) */
typedef struct HiScore {
    char name[NLENGTH];    /* 7-character name */
    U32  score;            /* Time in milliseconds */
    U16  deaths;           /* Death count */
    U8   mirror;           /* Mirror mode */
    U8   car;              /* Car index | (node << 4) */
} HiScore;
```

**Key Differences in High Scores:**

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Names | 7 characters | 3 characters (initials) |
| Entries per track | 100 | 10 |
| Time format | Milliseconds | Frames (60fps) |
| Storage | Direct NVRAM | Controller Pak file |
| Node tracking | Cabinet ID in car field | N/A |

### Arcade Statistics System

From `reference/repos/rushtherock/game/stats.c`:

The arcade tracks extensive per-track statistics:
- Games played, finishes, high scores
- Car selections, transmission preferences
- Drone/force/catchup option usage
- Time-of-day histograms
- Play time averages

N64 version simplifies this to aggregate statistics stored in `SaveStats`.

---

## N64 PFS Functions Used

### Core Functions

#### osPfsInitPak
```c
s32 osPfsInitPak(OSMesgQueue *mq, OSPfs *pfs, s32 channel);
```
**Purpose:** Initialize Controller Pak for a specific controller.

**Parameters:**
- `mq` - Message queue for SI operations
- `pfs` - OSPfs structure to initialize (output)
- `channel` - Controller port (0-3)

**Returns:** 0 on success, error code on failure

**Error Codes:**
| Code | Meaning |
|------|---------|
| 0 | Success |
| PFS_ERR_NOPACK (1) | No Controller Pak inserted |
| PFS_ERR_CONTRFAIL | Controller communication failure |
| PFS_ERR_CORRUPTED | Pak filesystem corrupted |

#### osPfsAllocateFile
```c
s32 osPfsAllocateFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                      u8 *gameName, u8 *extName, s32 size, s32 *fileNo);
```
**Purpose:** Create a new file on the Controller Pak.

**Parameters:**
- `pfs` - Initialized OSPfs handle
- `companyCode` - 2-byte company ID
- `gameCode` - 4-byte game ID
- `gameName` - 16-byte game name string
- `extName` - 4-byte extension string
- `size` - File size in bytes (must be multiple of 256)
- `fileNo` - Output: assigned file number

**Returns:** 0 on success

**Error Codes:**
| Code | Meaning |
|------|---------|
| PFS_ERR_EXIST | File already exists |
| PFS_ERR_FULL | Not enough space |
| PFS_ERR_DIR_FULL | No free directory slots |

#### osPfsReadWriteFile
```c
s32 osPfsReadWriteFile(OSPfs *pfs, s32 fileNo, u8 flag,
                       s32 offset, s32 size, u8 *data);
```
**Purpose:** Read or write data to a file.

**Parameters:**
- `pfs` - Initialized OSPfs handle
- `fileNo` - File number (0-15)
- `flag` - 0=read, 1=write
- `offset` - Byte offset within file
- `size` - Number of bytes
- `data` - Data buffer

**Low-Level Operation:**
1. Calculate starting page from offset
2. Select appropriate bank
3. For each page:
   - Read page to buffer
   - Copy data to/from buffer
   - Write page back (if writing)

#### osPfsFreeBlocks
```c
s32 osPfsFreeBlocks(OSPfs *pfs, s32 *freeBytes);
```
**Purpose:** Get available space on Controller Pak.

**Implementation:**
```c
/* Free pages are marked with ipage value of 3 */
for (bank = 0; bank < pfs->banks; bank++) {
    for (page = start; page < 128; page++) {
        if (inode.inode_page[page].ipage == 3)
            freeCount++;
    }
}
*freeBytes = freeCount * 256;  /* pages to bytes */
```

#### osPfsFindFile
```c
s32 osPfsFindFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                  u8 *gameName, u8 *extName, s32 *fileNo);
```
**Purpose:** Search for an existing file.

**Returns:** 0 if found (fileNo set), PFS_ERR_INVALID if not found

#### osPfsDeleteFile
```c
s32 osPfsDeleteFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                    u8 *gameName, u8 *extName);
```
**Purpose:** Delete a file and free its pages.

**Operation:**
1. Find file in directory
2. Follow inode chain marking pages as free (value 3)
3. Clear directory entry

### Error Handling

From `src/game/save.c`:

```c
static const char *sErrorStrings[] = {
    "OK",
    "No Controller Pak",
    "Controller Pak Error",
    "Data Corrupted",
    "Not Enough Space",
    "File Not Found",
    "Write Failed",
    "Read Failed",
    "Init Failed"
};
```

### Save Operation Flow

```
save_write()
    |
    +-> save_file_exists() -> osPfsFindFile()
    |       |
    |       +-> (not found) -> save_create_file() -> osPfsAllocateFile()
    |
    +-> Update checksum
    |
    +-> osPfsReadWriteFile(flag=PFS_WRITE)
```

---

## Save File Format

### File Layout (1024 bytes)

```
Offset  Size    Field
------  ----    -----
0x0000  4       Magic number (0x52534856 = "RSHV")
0x0004  2       Version (currently 1)
0x0006  2       Checksum (Adler-16)
0x0008  8       SaveOptions
0x0010  4       SaveUnlocks
0x0014  40      SaveStats
0x003C  ~880    SaveTrackData[8] (8 tracks x ~110 bytes)
0x03AC  48      SaveGhostHeader[4] (4 slots x 12 bytes)
0x03DC  36      Padding to 1024 bytes
```

### Checksum Algorithm

From `src/game/save.c`:

```c
/* Modified Adler-16 checksum */
u16 save_calc_checksum(void *data, s32 size) {
    u8 *bytes = (u8*)data;
    u32 a = 1;
    u32 b = 0;

    for (s32 i = 0; i < size; i++) {
        a = (a + bytes[i]) % 251;
        b = (b + a) % 251;
    }

    return (u16)((b << 8) | a);
}
```

### Default Values

On first run or corrupted data:

| Setting | Default |
|---------|---------|
| Sound Mode | Stereo |
| Music Volume | 80 |
| SFX Volume | 80 |
| Controller Sens | 50 |
| Vibration | On |
| Auto Trans | On |
| Camera Mode | 0 |
| HUD Mode | 0 |
| Cars Unlocked | First 4 (0x0F) |
| Tracks Unlocked | First 4 (0x0F) |

---

## Ghost Data

Ghost replays record car position/rotation each frame for time trial comparisons.

### Ghost File Structure

**File Identification:**
| Field | Value |
|-------|-------|
| Game Name | `"RUSH2049"` |
| Extension | `"GHOST"` |
| File Size | 2048 bytes (8 pages) |
| Max Slots | 4 ghosts |

### Ghost Header (12 bytes per slot)

```c
typedef struct SaveGhostHeader {
    u8   valid;            /* Slot in use */
    u8   track_id;         /* Track index */
    u8   car_id;           /* Car used */
    u8   mirror;           /* Mirror mode */
    u32  time;             /* Total lap time */
    u16  data_size;        /* Compressed data size */
    u16  checksum;         /* Data checksum */
} SaveGhostHeader;
```

### Ghost Frame Format

From `src/game/replay.c`:

```c
/* Compressed frame data (20 bytes) */
typedef struct ReplayFrame {
    s16 pos_x, pos_y, pos_z;       /* Position (16-bit fixed) */
    s16 quat_x, quat_y, quat_z, quat_w;  /* Orientation (quaternion) */
    s8  vel_x, vel_y, vel_z;       /* Velocity (8-bit) */
    u8  wheel_rot[4];              /* Wheel rotations */
    s8  steer_angle;               /* Steering */
    u8  flags;                     /* State flags */
} ReplayFrame;  /* 20 bytes */
```

**Compression Factors:**
| Value | Scale | Range |
|-------|-------|-------|
| Position | 16.0 | +/- 2048 units |
| Velocity | 128.0 | +/- 1 unit/frame |
| Quaternion | 16384.0 | -1.0 to 1.0 |

### Ghost Recording

```c
#define REPLAY_SAMPLE_RATE  2    /* Record every 2nd frame */
#define MAX_REPLAY_FRAMES   7200 /* 2 minutes at 60fps */
```

**Maximum Ghost Size:**
- Frames: 7200 / 2 = 3600 samples
- Size: 3600 * 20 bytes = 72,000 bytes (uncompressed)
- Actual: Uses delta compression + RLE for ~2KB per lap

### Ghost Comparison

During time trial:
1. Best lap ghost loaded from Controller Pak
2. Current lap recorded in parallel
3. Time delta displayed (ahead/behind ghost)
4. If new best, current lap becomes ghost

```c
void ghost_end_lap(u32 lap_time) {
    if (!gGhost.best_lap.data.valid ||
        lap_time < gGhost.best_lap.lap_time) {
        /* New best - save to pak */
        ghost_save_to_pak(track_id);
    }
}
```

---

## Implementation Notes

### Controller Detection

Rush 2049 checks all 4 controller ports for Controller Paks:

```c
void save_init(void) {
    for (i = 0; i < 4; i++) {
        if (save_detect_pak(i) == SAVE_OK) {
            if (save_file_exists(i)) {
                save_read_from(i);
                gSave.active_controller = i;
                break;
            }
        }
    }
}
```

### Auto-Save

The game supports optional auto-save:

```c
void save_shutdown(void) {
    if (gSave.auto_save && gSave.modified) {
        save_write();
    }
}
```

### Pak Hot-Swap

The game handles Controller Pak removal/insertion:
- Periodically calls `save_check_all_paks()`
- Updates `PakState` for each controller
- UI indicates pak status (None, Bad, Ready)

### Memory Considerations

- `OSPfs` structure: ~60 bytes per controller
- Save buffers: 1024 bytes for main save
- Ghost buffers: 2048 bytes per ghost slot
- Total static allocation: ~10KB for full save system

---

## Related Files

| Path | Description |
|------|-------------|
| `src/game/save.c` | Main save system implementation |
| `include/game/save.h` | Save data structures and constants |
| `src/game/replay.c` | Ghost recording and playback |
| `src/libultra/os_pfs*.c` | Controller Pak filesystem |
| `include/PR/os_pfs.h` | PFS structures and constants |

---

## References

- Nintendo 64 Programming Manual (libultra documentation)
- `reference/repos/rushtherock/game/hiscore.c` - Arcade high score system
- `reference/repos/rushtherock/game/stats.c` - Arcade statistics
- `reference/repos/rushtherock/game/globals.c` - Arcade NVRAM access
