# Checkpoint/Lap System Mapping

## Overview

This document maps the arcade Rush The Rock checkpoint and lap system to the N64 Rush 2049 decompilation. The checkpoint system handles:
- Checkpoint trigger detection
- Lap counting
- Bonus time awards
- Position/ranking calculation
- Split time tracking
- Finish line detection

## Arcade Source Files

| File | Purpose |
|------|---------|
| `game/checkpoint.c` | Main checkpoint detection and lap logic |
| `game/scheckpoint.c` | Alternate/simple checkpoint version |
| `game/cpinit.c` | Checkpoint data initialization per track |
| `game/checkpoint.h` | Data structures and constants |
| `game/sselect.c` | Timer functions (GetElapsedTime, GetCountdownTime) |
| `game/hud.c` | Lap counter display, elapsed time display |
| `game/globals.h` | Global variables (play_time, lap_flag, etc.) |

---

## Data Structures

### InitCheckPoint (Initial/Static Data)
```c
typedef struct {
    F32 pos[3];         // Checkpoint world position (x, y, z)
    S32 radius;         // Radius squared (in sq. feet) for trigger detection
    S16 logical_cp;     // Checkpoint order (finish line has highest value)
    F32 best_t[2];      // Bonus time: [0]=first lap, [1]=subsequent laps (seconds)
} InitCheckPoint;
```

### CheckPoint (Runtime Data)
```c
typedef struct {
    F32 pos[3];         // Position (copied from path at runtime)
    F32 uvs[3][3];      // Orientation matrix (Z-vec is checkpoint normal)
    S32 radius;         // Trigger radius squared
    S16 track_cent;     // Index into path array
    S16 logical_cp;     // Checkpoint order value
    F32 bonus_t[2];     // Calculated bonus time (adjusted by difficulty)
} CheckPoint;
```

### Track_Data (Per-Track Settings)
```c
typedef struct {
    F32 start_time[8];  // Initial time by difficulty (0-7)
    F32 end_time[8];    // Target remaining time at race end
    S16 loop_chkpnt;    // First checkpoint after finish line (for wrapping)
    S16 finish_line;    // Checkpoint index of finish line
    S16 before_finish;  // Checkpoint before finish (for final lap flag swap)
    S16 number_of_laps; // Total laps for this race
} Track_Data;
```

### Track (Per-Track Checkpoint Array)
```c
typedef struct {
    CheckPoint *chk_point;   // Pointer to checkpoint array
    S16 num_checkpoints;     // Number of checkpoints on track
    S16 direction;           // +1 or -1 (track path direction)
} Track;
```

---

## Key Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `MAX_CPS` | 20 | Maximum checkpoints per track |
| `MAX_TRACKS` | 8 | Number of tracks supported |
| `MAX_LINKS` | 8 | Maximum simultaneous cars |
| `ONE_SEC` | 1000 | Milliseconds per second |

---

## Arcade Function List

### Initialization Functions

| Function | Lines | Purpose |
|----------|-------|---------|
| `init_cp_data()` | 318-530 | Initialize checkpoint structures from static data |
| `InitCPS()` | 616-800 | Per-race checkpoint initialization, bonus time calculation |
| `init_cp_time()` | 807-814 | Reset checkpoint time tracking at race start |
| `FPutCheckpoints()` | 1136-1148 | Debug: Place checkpoint visual markers |

### Runtime Detection Functions

| Function | Lines | Purpose |
|----------|-------|---------|
| `CheckCPs()` | 866-984 | Main per-frame checkpoint check for all cars |
| `PassedCP()` | 989-1131 | Handle checkpoint crossing (awards time, updates lap) |
| `check_everyones_checkpoints()` | 1172-1275 | Award bonus time when lead car passes checkpoint |
| `CarPassedCheckpoint()` | 1281-1310 | Network: Handle remote car checkpoint messages |
| `CarReportsGameOver()` | 1316-1426 | Handle race finish, final placement sorting |

### Utility Functions

| Function | Lines | Purpose |
|----------|-------|---------|
| `get_next_checkpoint()` | 847-855 | Get next checkpoint index (with wrapping) |
| `get_next_center()` | 541-570 | Get next path point (respects track direction) |
| `get_prev_center()` | 580-612 | Get previous path point |
| `update_lap_counter()` | 1645-1693 | Update lap display on HUD |
| `set_game_time_to_one_second()` | 830-839 | Debug: Set time to 1 second remaining |
| `JumpToNextCP()` | 819-824 | Debug: Teleport to next checkpoint |

### Timer Functions (from sselect.c)

| Function | Purpose |
|----------|---------|
| `GetElapsedTime()` | Returns `IRQTIME - start_time` (race elapsed time in ms) |
| `GetCountdownTime()` | Returns `play_time - GetElapsedTime()` (remaining time) |
| `SetCountdownTimer(msec)` | Sets `play_time = msec`, `start_time = IRQTIME` |
| `TimeOut()` | Returns true if countdown reached zero |

---

## Checkpoint Detection Algorithm

From `CheckCPs()` (lines 866-905):

```c
// For each car:
for (index = 0; index < MAX_LINKS; index++) {
    // Get car position
    obs_posn = game_car[index].dr_pos;

    // Calculate offset from checkpoint center
    diff = obs_posn - chkpnt[next_cp_index].pos;

    // Get checkpoint normal (Z-axis of orientation matrix)
    zvec = chkpnt[next_cp_index].uvs[2];

    // Distance check (XZ plane only, ignores Y)
    cent_dist = diff[0]*diff[0] + diff[2]*diff[2];

    if (cent_dist < chkpnt[next_cp_index].radius) {
        // Inside radius - check if crossed plane
        if (DotVector(diff, zvec) > 0.0) {
            // Crossed checkpoint - car is on "forward" side of plane
            PassedCP(m, false);
        }
    }
}
```

**Key insight**: The checkpoint is a cylindrical trigger volume with infinite height. Detection uses:
1. XZ distance to checkpoint center (ignores Y/height)
2. Dot product with normal to detect crossing direction

---

## Lap Counting Logic

From `PassedCP()` (lines 989-1076):

```c
void PassedCP(MODELDAT *m, S8 flag) {
    // Check if crossing finish line
    if (m->next_cp_index == trck->finish_line) {
        m->laps++;

        // Record lap time if this is player's car
        if (node == this_node) {
            check_mpath_save(cancel_mpath_lap);
        }
    }

    // Update checkpoint indices
    m->last_cp_index = m->next_cp_index;
    m->next_cp_index = get_next_checkpoint(m->last_cp_index);

    // Handle lap counter display
    if (m->last_cp_index == trck->finish_line) {
        laps_remaining = trck->number_of_laps - m->laps;
        update_lap_counter(Do_it, laps_remaining);

        // Play appropriate sound
        if (laps_remaining == 1) {
            sound_code = S_FINALL;  // "Final lap!"
        } else if (laps_remaining == 0) {
            sound_code = S_WINNER;  // Race complete
            ShowGameOver(true);
        }
    }
}
```

---

## Bonus Time Calculation

From `InitCPS()` (lines 656-704):

```c
// Calculate total ideal lap time
while (1) {
    n_index = get_next_checkpoint(n_index);
    last_cp_time = chkpnt[n_index].bonus_t[0];  // First lap times
    if (n_index == trck->finish_line) break;
    total_time += last_cp_time;
}

// Calculate fudge factor to scale times by difficulty
start_time = trck->start_time[difficulty[trackno]];  // e.g., 60 seconds
end_time = trck->end_time[difficulty[trackno]];      // e.g., 5 seconds
time_to_be_given = (total_time + last_cp_time) - start_time + end_time;
fudge_factor = time_to_be_given / total_time;

// Apply fudge factor to all checkpoint bonus times
for (j = 0; j < number_checkpoints; j++) {
    chkpnt[j].bonus_t[0] *= fudge_factor;
    chkpnt[j].bonus_t[1] *= fudge_factor;
}
```

**Time flow**:
- Start: Player gets `start_time` seconds (e.g., 60s on easy)
- Each checkpoint: Adds `bonus_t` seconds (scaled by difficulty)
- Goal: End with `end_time` seconds remaining (e.g., 5s on hard)

---

## Position/Ranking System

From `CheckCPs()` (lines 927-960):

```c
// Collect cars that haven't finished
for (i = 0; i < num_active_cars; i++) {
    if (game_car[model[i].slot].place_locked != 1)
        place[num_left++] = model[i].slot;
}

// Sort by distance traveled (descending)
for (i = 0; i < num_left; i++) {
    dist_tab[place[i]] = game_car[place[i]].distance;
}

// Selection sort by distance
for (i = 0; i < num_left; i++) {
    // Find highest distance
    high_index = i;
    for (j = i+1; j < num_left; j++) {
        if (dist_tab[place[j]] > dist_tab[place[high_index]]) {
            high_index = j;
        }
    }
    // Assign place (accounting for already-finished cars)
    game_car[place[high_index]].place = i + num_active_cars - num_left;
}
```

**Distance calculation**: `distance = lap * lap_distance + checkpoint_distance`

---

## Split Time Display

From `PassedCP()` (lines 1078-1095):

```c
if (node == this_node) {
    // Calculate split time since last checkpoint
    S32 current_time = IRQTIME;
    S32 delta_time = current_time - last_checkpoint_time;

    // Display the split (e.g., "+5.23" or "-2.10")
    ShowCheckpointTime(delta_time);

    // Reset for next segment
    last_checkpoint_time = current_time;
}
```

---

## N64 Function Mappings

Based on analysis of `src/game/game.c`:

| Arcade Function | N64 Equivalent | Address | Notes |
|-----------------|----------------|---------|-------|
| `CheckCPs()` | `checkpoint_check_collision()` | ~0x8011XXXX | 1076 bytes |
| `PassedCP()` | (inline in above) | - | Combined |
| `lap_record_time()` | `lap_record_time()` | ~0x8011XXXX | 380 bytes |
| `respawn_position()` | (calculates from checkpoint) | ~0x8011XXXX | Uses checkpoint for spawn |
| `update_lap_counter()` | (in HUD code) | - | Part of render system |

### N64 Car Structure Offsets

From analysis of `checkpoint_check_collision()`:

| Offset | Type | Purpose |
|--------|------|---------|
| 0x00-0x0B | f32[3] | Car position (x, y, z) |
| 0x54 | s32 | Current checkpoint index |
| 0x58 | s32 | Lap count |
| 0x64+ | u32[16] | Checkpoint times array |

### N64 Checkpoint Structure Offsets

| Offset | Type | Purpose |
|--------|------|---------|
| 0x00-0x0B | f32[3] | Checkpoint position |
| 0x0C | f32 | Radius squared |
| 0x10-0x1B | f32[3] | Normal vector |
| 0x1C | s32 | Checkpoint index |
| 0x20 | u32 | Bonus time (ms) |
| 0x24 | s32 | Is finish line flag |

### N64 Global Variables

| Address | Purpose |
|---------|---------|
| `D_801582B4` | Total checkpoints |
| `D_801582B8` | Finish line checkpoint index |
| `D_8015A298` | Total laps for race |
| `D_80158FD0` | Checkpoint data array |

---

## Key Differences: Arcade vs N64

1. **Coordinate Scale**: Arcade uses feet/40, N64 uses direct world units
2. **Network**: Arcade has extensive multiplayer cabinet linking code
3. **Mirror Mode**: Arcade supports mirrored tracks (negates X coords)
4. **Sound Codes**: Different sound indices between platforms
5. **Display**: Arcade uses 3Dfx/Glide, N64 uses RDP display lists

---

## Implementation Notes for N64 Decompilation

1. The checkpoint system is largely portable - primarily math operations
2. Timer functions need N64 equivalents for `IRQTIME`
3. Bonus time tables are in `cpinit.c` - may need extraction from ROM
4. Checkpoint positions are derived from path data at runtime
5. The `logical_cp` ordering allows non-sequential checkpoint layouts

---

## Related Files in N64 Source

| File | Purpose |
|------|---------|
| `src/game/game.c` | Contains checkpoint_check_collision and related |
| `src/game/checkpoint.c` | Should contain checkpoint system (needs creation) |
| `include/game/game.h` | Car and checkpoint structure definitions needed |
