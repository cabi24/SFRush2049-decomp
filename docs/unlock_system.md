# Rush 2049 N64 Unlock and Progress System

This document describes the unlock system, progress tracking, and save data format for San Francisco Rush 2049 on N64.

## Overview

Rush 2049 uses a comprehensive unlock system that rewards players for completing tracks, achieving specific milestones, and collecting hidden items. Progress is saved to the Controller Pak.

## Unlockable Content

### Tracks (Unlock IDs 0-11)

| ID | Track Name | Unlock Condition |
|----|------------|------------------|
| 0 | Marina | Always unlocked |
| 1 | Haight | Always unlocked |
| 2 | Soma | Always unlocked |
| 3 | Mission | Always unlocked |
| 4 | Nob Hill | Complete Marina (finish in 1st place) |
| 5 | Chinatown | Complete previous track (1st place) |
| 6 | Fisherman's Wharf | Complete previous track (1st place) |
| 7 | Alcatraz | Complete previous track (1st place) |
| 8 | Bonus 1 | Complete all 8 main tracks |
| 9 | Bonus 2 | Complete all 8 main tracks |
| 10 | Bonus 3 | Complete all 8 main tracks |
| 11 | Stunt Arena | Complete any 5 tracks |

### Cars (Unlock IDs 12-19)

| ID | Car Name | Unlock Condition |
|----|----------|------------------|
| 12 | Crusher | Always unlocked |
| 13 | Dominator | Always unlocked |
| 14 | Velocity | Always unlocked |
| 15 | Rocket | Always unlocked |
| 16 | Panther | Complete 2 tracks |
| 17 | Bruiser | Complete 2 tracks |
| 18 | Speedster | Complete 4 tracks |
| 19 | Rampage | Complete 4 tracks |
| 20 | Nitro | Complete 6 tracks |
| 21 | Blaze | Complete 6 tracks |
| 22 | Phantom | Complete 8 tracks |
| 23 | Prototype | Achieve gold time on all 8 main tracks |

**Gold Time Requirement**: The Prototype car requires beating the gold (par) time on all 8 main tracks.

### Track Bonuses / Hidden Coins (Unlock IDs 20-31)

Each track has hidden coins to collect. Collecting all coins on a track unlocks a bonus:

| ID | Bonus | Unlock Condition |
|----|-------|------------------|
| 20-27 | Track [N] Bonus | Collect all hidden coins on track N |

Coin collection provides:
- 100 points per coin collected
- Bonus unlock when all coins on a track are collected
- Contributes to "CAR COLLECTOR" achievement

### Stunt Arenas (Unlock IDs 32-39)

| ID | Arena Name | Unlock Condition |
|----|------------|------------------|
| 32 | Alcatraz | Complete 5+ tracks |
| 33 | Metro | Available by default (with stunt mode) |
| 34 | Pier 39 | Complete stunt challenges |
| 35 | Golden Gate | Complete stunt challenges |
| 36 | Twin Peaks | Complete stunt challenges |
| 37 | Downtown | Complete stunt challenges |
| 38 | Obstacle | Complete stunt challenges |
| 39 | Freeform | Complete all other arenas |

### Paint Jobs (Unlock IDs 40-47)

| ID | Paint | Unlock Condition |
|----|-------|------------------|
| 0-7 | Standard Colors | Always unlocked (Red, Blue, Green, Yellow, Orange, Purple, White, Black) |
| 40 | Chrome | Special achievement (metallic finish) |
| 41 | Gold | Win with gold times |
| 42 | Rainbow | Complete all cars and tracks |

### Cheats (Unlock IDs 48-55)

Cheats are unlocked through special achievements or hidden conditions:

| ID | Cheat | Description |
|----|-------|-------------|
| 48-55 | Various | Unlocked through gameplay milestones |

### Misc Unlocks (Unlock IDs 56+)

| ID | Unlock | Description |
|----|--------|-------------|
| 56+ | Secrets | Various secret content |

## Progress Tracking

### Statistics Tracked

The game tracks the following player statistics:

- **Total Races**: Number of races completed
- **Total Wins**: Number of 1st place finishes
- **Best Race Times**: Per-track best race times (12 tracks)
- **Best Lap Times**: Per-track best lap times (12 tracks)
- **Stunt Points**: Cumulative stunt score
- **Total Distance**: Cumulative distance traveled
- **Crash Count**: Number of crashes

### Achievement System

The game has an internal achievement system:

| ID | Achievement | Requirement |
|----|-------------|-------------|
| 0 | FIRST VICTORY | Win 1 race |
| 1 | ROAD WARRIOR (10 WINS) | Win 10 races |
| 2 | CHAMPION (50 WINS) | Win 50 races |
| 3 | STUNT BEGINNER | Score 1,000 stunt points |
| 4 | STUNT MASTER (100K) | Score 100,000 stunt points |
| 5 | PERFECT RACE | Complete a race without crashing |
| 6 | TRACK COLLECTOR | Unlock all tracks (trackFlags == 0xFFFF) |
| 7 | CAR COLLECTOR | Unlock all cars (carFlags == 0xFFFF) |
| 8 | SPEEDSTER | Best lap under 30 seconds (1800 frames) |
| 9 | VETERAN RACER | Complete 100 races |

## Unlock Mechanism

### Core Functions

**unlock_check(unlockId)** - Check if content is unlocked
```c
s32 unlock_check(s32 unlockId) {
    // First 4 tracks always unlocked (IDs 0-3)
    // First 4 cars always unlocked (IDs 12-15)
    // Otherwise check unlock_bits array
    u32 unlockWord = unlock_bits[unlockId / 32];
    s32 bitIndex = unlockId % 32;
    return (unlockWord & (1 << bitIndex)) ? 1 : 0;
}
```

**unlock_trigger(unlockId)** - Unlock new content
```c
void unlock_trigger(s32 unlockId) {
    // Set unlock bit
    unlock_bits[unlockId / 32] |= (1 << unlockId % 32);

    // Show notification for 5 seconds
    game_unlock_id = unlockId;
    unlock_display_timer = 300;  // 60fps * 5 seconds

    // Queue save
    save_queue_flag = 1;

    // Set message based on type:
    // 0-11: "NEW TRACK UNLOCKED!"
    // 12-19: "NEW CAR UNLOCKED!"
    // 20-31: "BONUS UNLOCKED!"
    // 32-39: "NEW ARENA UNLOCKED!"
    // 40-47: "NEW PAINT UNLOCKED!"
    // 48-55: "CHEAT UNLOCKED!"
    // 56+: "SECRET UNLOCKED!"
}
```

### Track Unlock Conditions

```c
s32 track_unlock_check(s32 trackId) {
    // First 4 tracks always unlocked
    if (trackId < 4) return 1;

    // Check if previous track completed
    if (trackId > 0 && best_race_times[trackId - 1] > 0)
        return 1;

    // Bonus tracks (8-10): All 8 main tracks completed
    if (trackId >= 8 && trackId <= 10) {
        for (i = 0; i < 8; i++)
            if (best_race_times[i] == 0) return 0;
        return 1;
    }

    // Stunt arena (11): 5+ tracks completed
    if (trackId == 11) {
        s32 completed = 0;
        for (i = 0; i < 8; i++)
            if (best_race_times[i] > 0) completed++;
        return (completed >= 5) ? 1 : 0;
    }

    return 0;
}
```

### Car Unlock Conditions

```c
s32 car_unlock_check(s32 carId) {
    // First 4 cars always unlocked
    if (carId < 4) return 1;

    // Count completed tracks
    s32 tracksCompleted = 0;
    for (i = 0; i < 12; i++)
        if (best_race_times[i] > 0) tracksCompleted++;

    // Cars 4-5: 2 tracks
    if (carId >= 4 && carId <= 5 && tracksCompleted >= 2) return 1;

    // Cars 6-7: 4 tracks
    if (carId >= 6 && carId <= 7 && tracksCompleted >= 4) return 1;

    // Cars 8-9: 6 tracks
    if (carId >= 8 && carId <= 9 && tracksCompleted >= 6) return 1;

    // Car 10: 8 tracks
    if (carId == 10 && tracksCompleted >= 8) return 1;

    // Car 11 (Prototype): Gold time on all 8 main tracks
    if (carId == 11) {
        s32 goldCount = 0;
        for (i = 0; i < 8; i++)
            if (best_race_times[i] > 0 && best_race_times[i] <= gold_times_array[i])
                goldCount++;
        return (goldCount >= 8) ? 1 : 0;
    }

    return 0;
}
```

## Controller Pak Save Data Format

### Save File Structure

The game uses two slightly different save formats:

#### Format 1 (save_game_to_pak) - 256 bytes

| Offset | Size | Field |
|--------|------|-------|
| 0x00 | 5 | Magic "R2049" |
| 0x05 | 1 | Version (0x01) |
| 0x06 | 2 | Reserved (0x00, 0x00) |
| 0x08 | 4 | Checksum (16-bit sum of data after header) |
| 0x0C | 4 | Unlock flags (32 bits) |
| 0x10 | 64 | Best lap times (16 tracks * 4 bytes) |
| 0x50 | 8 | Settings |

#### Format 2 (progress_save) - 140 bytes

| Offset | Size | Field |
|--------|------|-------|
| 0x00 | 4 | Magic "RUSH" |
| 0x04 | 2 | Version (1, 0) |
| 0x06 | 8 | Unlock bits (2 * 4 bytes = 64 unlock flags) |
| 0x0E | 48 | Best race times (12 tracks * 4 bytes) |
| 0x3E | 48 | Best lap times (12 tracks * 4 bytes) |
| 0x6E | 8 | Settings (sfx_volume, music_volume, controller_config, vibration, display_mode, misc_flags, reserved[2]) |
| 0x76 | 2 | Checksum (16-bit sum of preceding data) |

### Save Validation

```c
s32 save_slot_valid(s32 slot) {
    // Check magic header "RUSH2049"
    if (header[0] != 'R' || header[1] != 'U' ||
        header[2] != 'S' || header[3] != 'H' ||
        header[4] != '2' || header[5] != '0' ||
        header[6] != '4' || header[7] != '9')
        return 0;
    return 1;
}
```

### Checksum Algorithm

Simple 16-bit additive checksum:
```c
u16 checksum = 0;
for (i = 0; i < offset; i++) {
    checksum += saveData[i];
}
```

### Save Slots

The game supports 4 save slots (0-3) on the Controller Pak.

### Save Operations

- **osPfsInitPak()** - Initialize Controller Pak access
- **osPfsReadWriteFile()** - Read/write save data
- **osPfsDeleteFile()** - Delete save file (filename: "RUSH2049")

## Ghost Data Format

Ghost data is saved separately for time trial replays:

| Offset | Size | Field |
|--------|------|-------|
| 0x00 | 4 | Magic "GHST" |
| 0x04 | 4 | Track ID |
| 0x08 | 4 | Lap time (frames) |
| 0x0C | 4 | Frame count |
| 0x10 | 4 | Version (0x20490001) |
| 0x14+ | var | Frame data (position/rotation per frame) |
| end-4 | 4 | Checksum |

## Cheat Codes

**Note**: The decompiled code does not reveal any traditional button sequence cheat codes. The arcade source shows cheats were disabled for production builds:

```c
/* Disable cheat mode (regardless of BRAM setting) for production. */
cheats = 0;
```

Cheats in Rush 2049 N64 appear to be unlock-based rather than code-based, tied to the achievement system.

## Key Memory Addresses

| Address | Purpose |
|---------|---------|
| 0x801146EC | gstate - Game state byte |
| 0x80159000 | Unlock flags (32-bit word) |
| 0x80159100 | Best times array |
| 0x80159200 | Settings array |
| 0x80159204 | Paint unlock flags |
| 0x8015A100 | Extended unlock flags |

## Related Source Files

- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - Main unlock system implementation
- `/home/cburnes/projects/rush2049-decomp/src/libultra/os_pfs.c` - Controller Pak file system

## Arcade Comparison

The N64 version differs from the arcade Rush The Rock in several ways:

1. **No NVRAM**: N64 uses Controller Pak instead of arcade EEPROM
2. **Simplified Cheats**: Production builds disable cheat modes
3. **Achievement System**: N64 has an achievement system not present in arcade
4. **Save Slots**: Multiple save slots vs. single arcade machine state

The arcade source uses `eer_gets()` / `eer_puts()` for EEPROM access, while N64 uses `osPfs*()` functions for Controller Pak.
