# Checkpoint and Race Timing System Analysis - Rush 2049 N64 Decompilation

This document maps the arcade checkpoint and race timing system to the N64 decompilation, providing key insights for accurate function matching.

## Overview

The Rush arcade checkpoint system handles:

1. **Checkpoint Detection** - Detecting when cars cross checkpoint trigger volumes
2. **Lap Counting** - Tracking lap completion and race progress
3. **Time Bonus** - Awarding bonus time at checkpoints (arcade timer mechanic)
4. **Race Positioning** - Calculating and updating race positions
5. **Split Times** - Recording and displaying checkpoint split times
6. **Best Lap Tracking** - Recording best lap times per track

## Arcade Source Files

| File | Lines | Purpose |
|------|-------|---------|
| `reference/repos/rushtherock/game/checkpoint.c` | 1,695 | Main checkpoint logic |
| `reference/repos/rushtherock/game/checkpoint.h` | 128 | Data structures |
| `reference/repos/rushtherock/game/cpinit.c` | ~930 | Checkpoint initial data per track |
| `reference/repos/rushtherock/game/select.c` | ~3,200 | Timer functions (GetElapsedTime, etc.) |
| `reference/repos/rushtherock/game/hud.c` | ~1,250 | HUD display (checkpoint time, lap counter) |

## Data Structures

### InitCheckPoint (checkpoint.h:74-80)

Initial checkpoint data used to define checkpoint positions per track:

```c
typedef struct {
    F32 pos[3];         // World position (x, y, z)
    S32 radius;         // Radius (not squared) of check circle
    S16 logical_cp;     // Checkpoint order (finish line is highest)
    F32 best_t[2];      // Par time for checkpoint [first_lap, other_laps]
} InitCheckPoint;
```

### CheckPoint (checkpoint.h:82-90)

Runtime checkpoint structure with additional computed fields:

```c
typedef struct {
    F32 pos[3];         // World position
    F32 uvs[3][3];      // Orientation vectors for crossing detection
    S32 radius;         // Radius of check circle
    S16 track_cent;     // Track center index over checkpoint center
    S16 logical_cp;     // Checkpoint order
    F32 bonus_t[2];     // Adjusted bonus time (scaled by difficulty)
} CheckPoint;
```

### Init_Track_Data (checkpoint.h:92-99)

Initial timing data per track:

```c
typedef struct {
    F32 start_time[8];  // Starting time (8 difficulty levels)
    F32 end_time[8];    // Target end time (8 difficulty levels)
    S16 loop_chkpnt;    // Checkpoint to loop to after passing last
    S16 finish_line;    // Finish line checkpoint index
    S16 before_finish;  // Checkpoint prior to finish line
} Init_Track_Data;
```

### Track_Data (checkpoint.h:101-109)

Runtime track data with lap count:

```c
typedef struct {
    F32 start_time[8];  // Starting time (8 difficulty levels)
    F32 end_time[8];    // Target end time (8 difficulty levels)
    S16 loop_chkpnt;    // Checkpoint loop index
    S16 finish_line;    // Finish line checkpoint index
    S16 before_finish;  // Checkpoint before finish
    S16 number_of_laps; // Actual laps for this race
} Track_Data;
```

### Track (checkpoint.h:111-116)

Track-level checkpoint container:

```c
typedef struct {
    CheckPoint *chk_point;  // Pointer to checkpoint array
    S16 num_checkpoints;    // Number of checkpoints on track
    S16 direction;          // Track direction (-1 or 1)
} Track;
```

### Per-Car Checkpoint Fields (modeldat.h)

Car state related to checkpoints:

```c
// In CAR_DATA (game_car[]):
S8  laps;           // Current lap (game version)
S8  checkpoint;     // Current checkpoint index
S8  place_locked;   // Race finish locked (-1=skip, 0=racing, 1=finished)
S32 score;          // Final race time (when finished)
F32 distance;       // Distance traveled (for position calculation)

// In MODELDAT (model[]):
S8  laps;               // Real laps
S16 last_cp_index;      // Last checkpoint passed
S16 next_cp_index;      // Next expected checkpoint
```

## Key Arcade Functions

### Initialization Functions

| Function | File:Line | Purpose |
|----------|-----------|---------|
| `init_cp_data()` | checkpoint.c:318 | Initialize checkpoint structures, calculate UVs |
| `InitCPS()` | checkpoint.c:616 | Initialize checkpoints for race start |
| `init_cp_time()` | checkpoint.c:807 | Initialize checkpoint timing |

### Per-Frame Update Functions

| Function | File:Line | Purpose |
|----------|-----------|---------|
| `CheckCPs()` | checkpoint.c:866 | Main per-frame checkpoint detection |
| `PassedCP()` | checkpoint.c:989 | Handle checkpoint crossing event |
| `check_everyones_checkpoints()` | checkpoint.c:1172 | Award bonus time to leading human |

### Helper Functions

| Function | File:Line | Purpose |
|----------|-----------|---------|
| `get_next_checkpoint()` | checkpoint.c:847 | Get next checkpoint index (with wrap) |
| `get_next_center()` | checkpoint.c:541 | Get next path center index |
| `get_prev_center()` | checkpoint.c:580 | Get previous path center index |
| `FPutCheckpoints()` | checkpoint.c:1136 | Place checkpoint objects (debug) |

### Game Over Functions

| Function | File:Line | Purpose |
|----------|-----------|---------|
| `CarReportsGameOver()` | checkpoint.c:1316 | Handle game over for a car |
| `JumpToNextCP()` | checkpoint.c:819 | Skip to next checkpoint (debug) |
| `set_game_time_to_one_second()` | checkpoint.c:830 | End game quickly (debug) |

### Timing Functions (select.c)

| Function | File:Line | Purpose |
|----------|-----------|---------|
| `GetCountdownTime()` | select.c:3045 | Get remaining race time (ms) |
| `GetElapsedTime()` | select.c:3076 | Get elapsed race time (ms) |
| `SetCountdownTimer()` | select.c:3092 | Set countdown timer |
| `ResetCountdownTimer()` | select.c:3084 | Reset timer to IRQTIME |
| `TimeOut()` | select.c:3101 | Check if countdown expired |

### Display Functions (hud.c)

| Function | File:Line | Purpose |
|----------|-----------|---------|
| `ShowCheckpointTime()` | hud.c:1032 | Display checkpoint split time |
| `ShowFinishFlag()` | hud.c:1154 | Display finish flag at checkpoint |
| `update_lap_counter()` | checkpoint.c:1645 | Update lap counter display |

## Checkpoint Detection Algorithm

### High-Level Flow

```
CheckCPs() [called every frame]
    |
    v
For each car:
    1. Skip if not controlled or crashed/resurrecting
    2. Get car position (game_car[index].dr_pos)
    3. Calculate distance from next expected checkpoint
    4. If within radius:
        a. Calculate dot product with checkpoint normal (uvs[2])
        b. If crossed plane (dot > 0):
            -> Call PassedCP()
    |
    v
find_maxpath_intervals()  [update AI path positions]
    |
    v
Calculate race positions (sort by distance)
    |
    v
check_everyones_checkpoints()  [award bonus time]
```

### Crossing Detection (CheckCPs)

```c
// From checkpoint.c:866-904
void CheckCPs(void) {
    for (index = 0; index < MAX_LINKS; index++) {
        // Get car position
        for (i = 0; i < 3; i++)
            obs_posn[i] = game_car[index].dr_pos[i];

        // Calculate difference from checkpoint center
        for (i = 0; i < 3; i++) {
            diff[i] = obs_posn[i] - chkpnt[m->next_cp_index].pos[i];
            zvec[i] = chkpnt[m->next_cp_index].uvs[2][i];
        }

        // Distance from checkpoint center (2D)
        cent_dist = diff[0]*diff[0] + diff[2]*diff[2];

        // If within checkpoint radius
        if (cent_dist < chkpnt[m->next_cp_index].radius) {
            // Check if crossed the plane (dot product with normal)
            if (DotVector(diff, zvec) > 0.0) {
                PassedCP(m, false);  // Checkpoint passed!
            }
        }
    }
}
```

### Checkpoint Passing (PassedCP)

```c
// From checkpoint.c:989-1131
void PassedCP(MODELDAT *m, S8 flag) {
    // Check for lap completion
    if (m->next_cp_index == trck->finish_line) {
        m->laps++;
        // Handle maxpath sync for AI
        check_mpath_save(cancel_mpath_lap);
    }

    // Update checkpoint indices
    m->last_cp_index = m->next_cp_index;
    m->next_cp_index = get_next_checkpoint(m->last_cp_index);

    // Play appropriate sound
    if (node == gThisNode) {
        if (m->last_cp_index == trck->finish_line) {
            i = trck->number_of_laps - m->laps;  // Laps remaining
            update_lap_counter(Do_it, i);

            if (i > 1) sound_code = laps_to_go[i-1];  // "X laps to go"
            else if (i == 1) {
                ShowFinishFlag(chkpnt[m->last_cp_index].pos, false);
                sound_code = S_FINALL;  // "Final lap!"
            }
            else if (i == 0) {
                sound_code = S_WINNER;  // "Winner!"
                ShowGameOver(true);
            }
        }
        SOUND(sound_code);
    }

    // Record checkpoint time
    if (node == this_node) {
        S32 current_time = IRQTIME;
        S32 delta_time = current_time - last_checkpoint_time;
        ShowCheckpointTime(delta_time);
        last_checkpoint_time = current_time;
    }

    // Check for race finish
    if (m->last_cp_index == trck->finish_line) {
        if (trck->number_of_laps == m->laps && game_car[node].place_locked == 0) {
            // Record finish time with sub-frame precision
            game_car[node].gameover_time = GetElapsedTime();
            // Interpolate exact crossing time
            dist = BodyZ(rpos, chkpnt[m->last_cp_index].uvs);
            vel = BodyZ(gc->dr_vel, chkpnt[m->last_cp_index].uvs);
            dtime = (vel > 1.0) ? (dist/vel)*1000.0 : 0;
            game_car[node].gameover_time -= dtime;
        }
    }
}
```

## Race Timing System

### Timer Architecture

The arcade uses a countdown timer system:

```c
// Global variables (select.c)
U32 play_time;    // Total countdown time (milliseconds)
U32 start_time;   // IRQTIME when timer started

// Get remaining time
S32 GetCountdownTime(void) {
    return (S32)play_time - (IRQTIME - (S32)start_time);
}

// Get elapsed time since race start
U32 GetElapsedTime(void) {
    return IRQTIME - start_time;
}

// Set countdown timer
void SetCountdownTimer(long msec) {
    play_time = msec;
    start_time = IRQTIME;  // Reset reference point
}
```

### Time Constants

```c
#define ONE_SEC     1000L   // 1000 milliseconds
#define NET_SEC     100000L // Network timer ticks per second

#define TRK_SELECT_TIME  (16 * ONE_SEC)  // Track select timeout
#define QCK_SELECT_TIME  ( 5 * ONE_SEC)  // Quick select timeout
#define CAR_SELECT_TIME  (16 * ONE_SEC)  // Car select timeout
```

### Bonus Time Calculation

Bonus time is calculated based on difficulty and distributed across checkpoints:

```c
// From InitCPS() - checkpoint.c:616-800
void InitCPS(void) {
    // Get start and target end times for difficulty level
    start_time = trck->start_time[difficulty[trackno]];
    end_time = trck->end_time[difficulty[trackno]];

    // Calculate total time to distribute
    time_to_be_given = (total_time + last_cp_time) - start_time + end_time;

    // Calculate scaling factor
    fudge_factor = (F32)time_to_be_given / total_time;

    // Scale all checkpoint bonus times
    for (j = 0; j < number_checkpoints; j++)
        for (k = 0; k < 2; k++)
            chkpnt[j].bonus_t[k] *= fudge_factor;
}
```

### Bonus Time Award

```c
// From check_everyones_checkpoints() - checkpoint.c:1172-1275
void check_everyones_checkpoints(void) {
    // For each active car
    for (i = 0; i < num_active_cars; i++) {
        // Only award time to leading human
        if (model[slot].drone_type == HUMAN) {
            // Check if passed next expected checkpoint
            if (cur_laps > next_lap ||
                (cur_laps == next_lap &&
                 chkpnt[cur_lcp].logical_cp >= chkpnt[next_lcp].logical_cp)) {

                // Get bonus time (first lap vs other laps)
                if (global_laps > 0)
                    bonus_t = chkpnt[next_lcp].bonus_t[1];
                else
                    bonus_t = chkpnt[next_lcp].bonus_t[0];

                // Add bonus time
                play_time += bonus_t - total_time_box_time;
                SOUND(S_BEEP1);
            }
        }
    }
}
```

## Race Position Calculation

### Position Algorithm

Positions are calculated by sorting cars by total distance traveled:

```c
// From CheckCPs() - checkpoint.c:927-960
// Sort unlocked cars by distance
for (i = 0; i < num_active_cars; i++) {
    if (game_car[model[i].slot].place_locked != 1)
        place[num_left++] = model[i].slot;
}

for (i = 0; i < num_active_cars; i++) {
    dist_tab[place[i]] = game_car[place[i]].distance;
}

// Selection sort by distance (descending)
for (i = 0; i < num_left; i++) {
    high_dist = dist_tab[place[i]];
    high_index = i;
    for (j = i+1; j < num_left; j++) {
        if (dist_tab[place[j]] > dist_tab[place[high_index]]) {
            high_dist = dist_tab[place[j]];
            high_index = j;
        }
    }
    game_car[place[high_index]].place = i + num_active_cars - num_left;
    // Swap
    temp = place[high_index];
    place[high_index] = place[i];
    place[i] = temp;
}
```

### Game Over Sorting

When cars finish, they are sorted by completion time:

```c
// From CarReportsGameOver() - checkpoint.c:1316-1426
void CarReportsGameOver(S32 slot, U32 score, U32 flag) {
    game_car[slot].score = score;
    game_car[slot].place_locked = 1;

    // Re-sort all locked players by score (race time)
    for (i = 0; i < num_locked; i++)
        for (j = 0; j < num_locked-i; j++) {
            if (game_car[place[j]].score > game_car[place[j+1]].score) {
                // Swap (lower time = better place)
                temp = place[j];
                place[j] = place[j+1];
                place[j+1] = temp;
            }
        }

    // Handle ties - lower node number wins
    for (i = 1; i <= num_locked; i++) {
        if (game_car[place[i-1]].score == game_car[place[i]].score &&
            place[i-1] > place[i]) {
            // Swap
            temp = place[i];
            place[i] = place[i-1];
            place[i-1] = temp;
            i = 0;  // Restart check
        }
    }

    // Assign places
    for (i = 0; i < num_locked+1; i++)
        game_car[place[i]].place = i;
}
```

## N64 Function Mapping

Based on analysis of `src/game/game.c`:

### Direct Function Equivalents

| Arcade Function | N64 Function | Line | Confidence |
|-----------------|--------------|------|------------|
| `init_cp_data()` | `race_init_checkpoints()` | - | High |
| `InitCPS()` | `checkpoints_enabled = 1` | 34259 | Medium |
| `CheckCPs()` | `checkpoint_check_collision()` | 13249 | High |
| `PassedCP()` | Within `checkpoint_check_collision()` | 13331 | High |
| `check_everyones_checkpoints()` | `race_position_update()` | 13432 | Medium |
| `CarReportsGameOver()` | `race_finish_check()` | 13543 | High |
| `GetElapsedTime()` | `timer_get_elapsed()` | 26593 | High |
| `ShowCheckpointTime()` | `display_centered_message(0)` | 31179 | Medium |
| `update_lap_counter()` | `draw_lap_counter()` | 31005 | High |
| `lap_record_time()` | `lap_record_time()` | 13378 | High |

### N64 Checkpoint Variables

```c
// From game.c externs (lines 282-325)
extern s32 current_race_time;      // Current race time (hundredths)
extern s32 current_lap_time;       // Current lap time
extern s32 last_lap_total;         // Previous lap end time
extern s32 best_lap_time;          // Best lap this race
extern s32 race_num_laps;          // Total laps in race
extern s32 total_lap_count;        // Another lap count
extern s32 checkpoints_enabled;    // Checkpoint system enabled
extern u32 best_race_times[12];    // Best times per track
extern u32 best_lap_times[12];     // Best laps per track
extern s32 new_best_lap_flag;      // New best lap indicator

// Player state arrays
extern void *player_lap_count[];   // Per-player lap count
extern void *player_checkpoint[];  // Per-player checkpoint
extern void *player_race_time[];   // Per-player race time

// Split time display
extern void *split_display_active[];
extern s32 split_time_frames[4];
```

### N64 Timer Update (game.c:21134)

```c
// Game timer update - updates race clock and lap times
void timer_update(void) {
    s32 elapsedTime;

    elapsedTime = currentFrame - race_start_frame;
    elapsedTime = elapsedTime - pause_accumulator;

    // Convert frames to hundredths of second
    raceTime = (elapsedTime * 100) / 60;
    current_race_time = raceTime;

    // Update lap time
    current_lap_time = raceTime - last_lap_total;

    // Update split time display lifetime
    for (i = 0; i < 4; i++) {
        if (split_display_active[i] > 0) {
            s32 splitAge = currentFrame - split_time_frames[i];
            if (splitAge > 180) {  // 3 seconds at 60fps
                split_display_active[i] = 0;
            }
        }
    }

    // Best lap tracking
    if (current_lap_time > 0 && current_lap_time < best_lap_time) {
        best_lap_time = current_lap_time;
    }
}
```

### N64 Checkpoint Collision (game.c:13249)

```c
// checkpoint_check_collision (1076 bytes)
// Checks if car crossed checkpoint trigger plane
void checkpoint_check_collision(void *car, void *checkpoint) {
    // Get checkpoint data
    cpPos = (f32 *)checkpoint;
    radiusSq = *(f32 *)((u8 *)checkpoint + 0x0C);
    cpNormal = (f32 *)((u8 *)checkpoint + 0x10);
    cpIndex = *(s32 *)((u8 *)checkpoint + 0x1C);

    // Only check if this is the next expected checkpoint
    if (cpIndex != *carCpIndex) return;

    // Calculate distance from car to checkpoint center
    dx = carPos[0] - cpPos[0];
    dy = carPos[1] - cpPos[1];
    dz = carPos[2] - cpPos[2];
    distSq = dx*dx + dy*dy + dz*dz;

    // Check if within checkpoint radius
    if (distSq >= radiusSq) return;

    // Check if car crossed the checkpoint plane
    dotProduct = dx * cpNormal[0] + dy * cpNormal[1] + dz * cpNormal[2];
    if (dotProduct <= 0.0f) return;

    // Checkpoint passed! Record time
    cpTimes[cpIndex] = current_race_time;

    // Advance to next checkpoint
    (*carCpIndex)++;

    // Check for lap completion
    if (*carCpIndex >= D_801582B4) {  // Total checkpoints
        *carCpIndex = 0;
        (*carLaps)++;
        lap_record_time(car, *carLaps);
        voice_play(6);  // "Final lap" or similar
    } else {
        voice_play(8);  // "Checkpoint!"
    }
}
```

### N64 Lap Recording (game.c:13378)

```c
// lap_record_time (380 bytes)
// Records lap completion time and updates best lap
void lap_record_time(void *player, s32 lapNum) {
    u32 *lapTimes, *bestLap, *lastLap;
    u32 currentTime, lapTime, prevLapEnd;

    if (lapNum < 1 || lapNum > 8) return;

    // Get player lap data
    lapTimes = (u32 *)((u8 *)player + 0xA4);
    bestLap = (u32 *)((u8 *)player + 0xC4);
    lastLap = (u32 *)((u8 *)player + 0xC8);

    currentTime = current_race_time;

    // Calculate lap time
    if (lapNum == 1)
        prevLapEnd = 0;
    else
        prevLapEnd = lapTimes[lapNum - 2];

    lapTime = currentTime - prevLapEnd;

    // Record this lap time
    lapTimes[lapNum - 1] = currentTime;
    *lastLap = lapTime;

    // Update best lap if faster
    if (*bestLap == 0 || lapTime < *bestLap) {
        *bestLap = lapTime;
        // Check global track record
        if (track_best_lap[0] == 0 || lapTime < track_best_lap[0]) {
            // New track record
        }
    }
}
```

## Timing Differences: Arcade vs N64

### Time Units

| Platform | Base Unit | Conversion |
|----------|-----------|------------|
| Arcade | Milliseconds (ms) | `ONE_SEC = 1000` |
| N64 | Frames (60fps) | `(frames * 100) / 60 = hundredths` |

### Timer Implementation

**Arcade:**
- Uses `IRQTIME` (1ms resolution hardware timer)
- Countdown timer for race (adds bonus time at checkpoints)
- `GetElapsedTime() = IRQTIME - start_time`

**N64:**
- Uses frame counter (60fps)
- Race time = elapsed frames since start
- `elapsed_cs = (frames * 100) / 60` for centiseconds

### Time Display Formats

```c
// Arcade time conversion (hud.c:1041)
cvt_time(delta, CheckpointStr, 'h');  // Format: MM:SS.hh

// N64 time format (game.c:21154)
// raceTime in hundredths: SS * 100 + hh
minutes = raceTime / 6000;
seconds = (raceTime / 100) % 60;
hundredths = raceTime % 100;
```

## Checkpoint Layout Per Track

The arcade defines checkpoints in `cpinit.c` for each track:

```c
// Example: Track 0 checkpoints (cpinit.c:212)
const InitCheckPoint InitTrack0CPs[] = {
    // pos[3],           radius, logical_cp, best_t[2]
    {{  500, 0, -1200}, 40000,  0, {15.0, 12.0}},  // CP 0
    {{ 1800, 0,   600}, 40000,  1, {10.0,  8.0}},  // CP 1
    {{ -200, 0,  2000}, 40000,  2, {12.0, 10.0}},  // CP 2 (Finish)
    {{ -999, 0, -9999}, 0,     -1, {0.0,   0.0}},  // Terminator
};
```

Track data defines which checkpoint is the finish line:

```c
// Track 0 data (cpinit.c:932)
const Init_Track_Data init_track_data[] = {
    // start_time[8], end_time[8], loop, finish, before
    {{60,55,50,45,40,38,36,35}, {10,8,6,4,2,1,0,0}, 0, 2, 1},
    // ...
};
```

## Voice/Sound Triggers

### Checkpoint Sounds (arcade)

| Sound ID | Trigger | Description |
|----------|---------|-------------|
| `S_CHKPNTSTATIC` | Any checkpoint | Static/radio sound |
| `S_FINALL` | Last lap | "Final lap!" |
| `S_2L` - `S_9L` | Lap counter | "X laps to go" |
| `S_WINNER` | Race finish | "Winner!" |
| `S_BEEP1` | Time bonus | Bonus time awarded |
| `S_LEADERLIGHT` | First place | Crowd cheer |
| `S_KLEADERLIGHT` | Lost first | Cheer fades |

### N64 Voice Calls (game.c:23345)

```c
// Voice IDs for checkpoint/lap events
case 6:  // "Final Lap!"
case 8:  // "Checkpoint!"
case 11-13:  // Lap calls ("2 laps", "3 laps")
```

## Implementation Notes

### Key Differences: Arcade vs N64

1. **Countdown Timer**: Arcade uses countdown with bonus time at checkpoints. N64 may use simple elapsed time.

2. **Network Sync**: Arcade has extensive network synchronization code for linked cabinets. N64 uses local split-screen only.

3. **Difficulty Levels**: Arcade has 8 difficulty levels affecting start/bonus times. N64 may have simplified difficulty.

4. **Finish Time Precision**: Arcade interpolates exact crossing time using velocity. N64 may use frame-level precision.

5. **Checkpoint Count**: Arcade uses `MAX_CPS = 20` per track. N64 appears to use up to 16.

### Recommended Decompilation Order

1. Start with `timer_get_elapsed()` - Basic timing
2. Then `checkpoint_check_collision()` - Core detection
3. Next `lap_record_time()` - Lap completion
4. Then `race_position_update()` - Position calculation
5. Finally `race_finish_check()` - Game over handling

### Debug Features

The arcade had checkpoint debugging:
- `SW_DEBUG6`: Show delta time vs elapsed
- `SW_DEBUG11`: Show path point errors
- `SW_DEBUG14`: Display checkpoint objects

Look for similar debug remnants in N64 code.

## References

- Arcade source: `reference/repos/rushtherock/game/checkpoint.c`
- Arcade headers: `reference/repos/rushtherock/game/checkpoint.h`
- Arcade timing: `reference/repos/rushtherock/game/select.c`
- Arcade HUD: `reference/repos/rushtherock/game/hud.c`
- N64 decompiled: `src/game/game.c`
- AI/Drone mapping: `docs/ai_drone_mapping.md`
