# Checkpoint and Race Timing System

This document describes the race checkpoint and timing system used in San Francisco Rush 2049 for the N64, with comparisons to the original arcade source code.

## Overview

The checkpoint system handles:
- Detection of checkpoint crossings
- Lap counting and validation
- Race timer management and time extensions
- Race position calculation and ranking
- Finish line detection

The implementation is based heavily on the arcade source code in `reference/repos/rushtherock/game/checkpoint.c`.

---

## Key Data Structures

### CheckPoint

Defines a single checkpoint trigger zone on the track.

```c
typedef struct CheckPoint {
    f32     pos[3];             /* World position (x, y, z) */
    f32     uvs[3][3];          /* Orientation matrix (x-vec, y-vec, z-vec) */
    f32     radius;             /* Trigger radius squared */
    f32     bonus_t[2];         /* Bonus time [first_lap, other_laps] */
    s16     track_cent;         /* Closest path center index */
    s16     logical_cp;         /* Logical checkpoint order (-1 = sentinel) */
} CheckPoint;
```

| Field | Description |
|-------|-------------|
| `pos[3]` | World XYZ position of checkpoint center |
| `uvs[3][3]` | 3x3 orientation matrix. The Z-vector (uvs[2]) is the crossing direction |
| `radius` | Squared radius for XZ plane proximity check |
| `bonus_t[0]` | Time bonus for first lap (in milliseconds, arcade uses ONE_SEC = 1000) |
| `bonus_t[1]` | Time bonus for subsequent laps |
| `track_cent` | Index into path array for closest centerline point |
| `logical_cp` | Order value for checkpoint sequencing. -1 marks end of array |

### TrackData

Per-track race configuration.

```c
typedef struct TrackData {
    u32     start_time[8];      /* Start time per difficulty (ms) */
    u32     end_time[8];        /* Target end time per difficulty (ms) */
    s16     loop_chkpnt;        /* First checkpoint in lap loop */
    s16     finish_line;        /* Finish line checkpoint index */
    s16     before_finish;      /* Checkpoint before finish line */
    s16     number_of_laps;     /* Laps in current race */
} TrackData;
```

| Field | Description |
|-------|-------------|
| `start_time[8]` | Initial race time for each of 8 difficulty levels |
| `end_time[8]` | Target remaining time at race end (used for bonus calculation) |
| `loop_chkpnt` | After passing last checkpoint, next checkpoint is this index |
| `finish_line` | Checkpoint index that represents the finish line |
| `before_finish` | Used to trigger finish line visual swap (checkered flag) |

### Track

Container for per-track checkpoint data.

```c
typedef struct Track {
    CheckPoint  *chk_point;     /* Pointer to checkpoint array */
    s16         num_checkpoints;/* Number of checkpoints on track */
    s16         direction;      /* Track direction: -1 or 1 */
} Track;
```

### CarData (Race-related fields)

Per-car state relevant to checkpoint system:

```c
typedef struct CarData {
    f32     dr_pos[3];          /* Dead reckoned position */
    f32     distance;           /* Distance from start (for ranking) */
    s8      place;              /* Race position (0 = 1st) */
    s8      place_locked;       /* 1 = finished, -1 = DNF, 0 = racing */
    s8      laps;               /* Current lap number */
    s8      checkpoint;         /* Current checkpoint index */
    u32     score;              /* Finish time for final ranking */
    /* ... other fields ... */
} CarData;
```

### Global Checkpoint State

```c
u32 number_checkpoints;         /* Checkpoint count for current track */
u32 last_checkpoint_time;       /* Timestamp of last checkpoint hit */
s16 global_laps;                /* Lead car's lap count */
s16 global_last_cp;             /* Lead car's last checkpoint */
s16 global_next_cp;             /* Lead car's next expected checkpoint */
s16 lap_loop_index;             /* First checkpoint in lap loop */
u32 time_to_be_given;           /* Total bonus time available */
u32 total_time_given;           /* Bonus time awarded so far */
```

---

## Checkpoint Detection

### Algorithm

The checkpoint detection uses a plane test with proximity culling:

1. **Proximity Check**: Calculate XZ distance to checkpoint center
   ```c
   cent_dist = diff[0] * diff[0] + diff[2] * diff[2];
   if (cent_dist < chkpnt[next_cp].radius) { ... }
   ```

2. **Plane Test**: Check if car has crossed the checkpoint plane
   ```c
   /* Car is past checkpoint if dot(car_pos - cp_pos, cp_z_vector) > 0 */
   if (vec_dot(diff, zvec) > 0.0f) {
       /* Passed checkpoint! */
   }
   ```

### CheckCPs Function

Called every frame to detect checkpoint crossings for all active cars.

```c
void CheckCPs(void) {
    for (index = 0; index < num_active_cars; index++) {
        /* Get next expected checkpoint for this car */
        next_cp = get_next_checkpoint(car_array[index].checkpoint);

        /* Get car position */
        obs_posn = car_array[index].dr_pos;

        /* Calculate vector from checkpoint to car */
        diff = obs_posn - chkpnt[next_cp].pos;
        zvec = chkpnt[next_cp].uvs[2];  /* Z-vector is crossing direction */

        /* XZ distance check (ignore Y for wider tolerance) */
        cent_dist = diff[0]*diff[0] + diff[2]*diff[2];

        if (cent_dist < chkpnt[next_cp].radius) {
            /* Within trigger radius - check if past plane */
            if (DotVector(diff, zvec) > 0.0) {
                car_array[index].checkpoint = next_cp;
                if (next_cp == track_data[trackno].finish_line) {
                    car_array[index].laps++;
                }
            }
        }
    }

    /* Update positions and global state */
    check_everyones_checkpoints();
}
```

### Arcade Comparison

The N64 implementation mirrors the arcade version closely:

| Arcade (checkpoint.c) | N64 (checkpoint.c) |
|-----------------------|-------------------|
| `CheckCPs()` at line 866 | `CheckCPs()` |
| `DotVector(diff, zvec)` | `vec_dot(diff, zvec)` |
| `game_car[index].dr_pos` | `car_array[index].dr_pos` |
| `model[index].next_cp_index` | `car_array[index].checkpoint` |

---

## Lap Counting Logic

### Checkpoint Sequence

Checkpoints are traversed in order. The `get_next_checkpoint()` function handles wrapping:

```c
u32 get_next_checkpoint(u32 cur_checkpoint) {
    cur_checkpoint++;

    /* Wrap to loop index after last checkpoint */
    if (cur_checkpoint >= number_checkpoints) {
        cur_checkpoint = lap_loop_index;
    }

    return cur_checkpoint;
}
```

### Lap Increment

A lap is completed when crossing the finish line checkpoint:

```c
void PassedCP(s32 car_index, s8 flag) {
    if (next_cp == trck->finish_line) {
        car_array[node].laps++;

        /* Update HUD */
        laps_remaining = trck->number_of_laps - car_array[node].laps;
        update_lap_counter(CP_UPDATE, laps_remaining);
    }
}
```

### Wrong Way Detection

The race.c implementation tracks wrong-way driving:

```c
void race_car_hit_checkpoint(s32 car_index, s32 checkpoint_id) {
    if (checkpoint_id == cp->next_cp) {
        /* Correct checkpoint - clear wrong way flag */
        cp->wrong_way = 0;
        cp->last_cp = checkpoint_id;
        cp->next_cp = (checkpoint_id + 1) % num_checkpoints;
    } else {
        /* Check if going backwards */
        s32 prev = (expected - 1 + num_checkpoints) % num_checkpoints;
        if (checkpoint_id == prev) {
            cp->wrong_way = 1;
        }
    }
}
```

---

## Race Timer Implementation

### Timer Architecture

The N64 uses two complementary timing systems:

1. **Frame-based timing** (`timer.c`, `race.c`):
   - Uses `frame_counter` incremented each frame at 60 FPS
   - Race time calculated as `frame_counter - start_time`

2. **Millisecond-based timing** (`timer.c`, arcade compatibility):
   - Uses `osGetCount()` for high-resolution timing
   - Converts CPU counter to milliseconds for arcade-compatible API

### Timer State

```c
static u32 timer_start_count;       /* osGetCount at race start */
static f32 timer_elapsed;           /* Elapsed time in seconds */
static u32 lap_start_times[MAX_CARS]; /* Lap start per car */
```

### Arcade-Compatible Timing

The arcade uses `IRQTIME` (milliseconds since boot). N64 emulates this:

```c
/* Arcade constant */
#define ONE_SEC     1000    /* Milliseconds per second */

/* N64 equivalent of arcade IRQTIME */
static u32 get_irqtime(void) {
    u32 count = osGetCount();
    return count / (COUNT_PER_SEC / 1000);  /* Convert to ms */
}

/* Get remaining countdown time */
s32 GetCountdownTime(void) {
    return (s32)arcade_play_time - (s32)GetElapsedTime();
}

/* Check if timer expired */
s32 TimeOut(void) {
    return GetCountdownTime() <= (ONE_SEC / 2);
}
```

### Time Formatting

```c
void race_format_time(u32 frames, char *buffer) {
    u32 total_secs = frames / RACE_FRAMES_PER_SEC;
    u32 mins = total_secs / 60;
    u32 secs = total_secs % 60;
    u32 hundredths = (frames % RACE_FRAMES_PER_SEC) * 100 / RACE_FRAMES_PER_SEC;

    /* Output: "M:SS.HH" or "MM:SS.HH" */
}
```

---

## Time Extension on Checkpoint

### Bonus Time Calculation

The arcade calculates bonus times dynamically based on difficulty and track:

```c
void InitCPS(void) {
    /* Calculate total time that will be awarded */
    total_time = /* sum of optimal checkpoint times */;
    start_time = trck->start_time[difficulty[trackno]];
    end_time = trck->end_time[difficulty[trackno]];

    time_to_be_given = (total_time + last_cp_time) - start_time + end_time;

    /* Scale all checkpoint times by fudge factor */
    fudge_factor = (F32)time_to_be_given / total_time;

    for (j = 0; j < number_checkpoints; j++) {
        chkpnt[j].bonus_t[0] *= fudge_factor;
        chkpnt[j].bonus_t[1] *= fudge_factor;
    }
}
```

### Awarding Bonus Time

Time is awarded when the lead human passes each checkpoint:

```c
void check_everyones_checkpoints(void) {
    for (i = 0; i < num_active_cars; i++) {
        slot = model[i].slot;

        /* Check if this human car advanced past expected checkpoint */
        if ((model[slot].drone_type == HUMAN) &&
            (cur_laps > next_lap ||
             (cur_laps == next_lap &&
              chkpnt[cur_lcp].logical_cp >= chkpnt[next_lcp].logical_cp))) {

            /* Determine bonus time based on lap */
            if (global_laps > 0) {
                bonus_t = chkpnt[next_lcp].bonus_t[1];  /* Subsequent lap */
            } else {
                bonus_t = chkpnt[next_lcp].bonus_t[0];  /* First lap */
            }

            /* Add time to race timer */
            play_time += bonus_t - total_time_box_time;
            total_time_given += bonus_t - total_time_box_time;
        }
    }
}
```

### N64 Implementation

```c
void race_car_hit_checkpoint(s32 car_index, s32 checkpoint_id) {
    if (checkpoint_id == cp->next_cp) {
        /* Add time bonus */
        gRace.time_remaining += gRace.config.checkpoint_bonus;
    }
}

void race_extend_time(s32 frames) {
    gRace.time_remaining += frames;
    gRace.race_flags |= RACE_FLAG_EXTENDED;
}
```

---

## Position/Ranking Calculation

### Distance-Based Ranking

Cars are ranked by their distance around the track:

```c
f32 calc_car_distance(s32 car_index) {
    f32 distance;
    s32 cp = car_array[car_index].checkpoint;
    s32 laps = car_array[car_index].laps;

    /* Base distance from laps completed */
    distance = (f32)laps * (f32)num_path_points;

    /* Add checkpoint progress within current lap */
    distance += (f32)path_dist_index[cp];

    return distance;
}
```

### Sorting Algorithm

The arcade uses selection sort for efficiency:

```c
void sort_race_positions(void) {
    /* Build list of cars still racing */
    num_left = 0;
    for (i = 0; i < num_active_cars; i++) {
        if (car_array[i].place_locked != 1) {
            place[num_left++] = i;
        }
    }

    /* Selection sort by distance (descending) */
    for (i = 0; i < num_left; i++) {
        high_index = i;
        for (j = i + 1; j < num_left; j++) {
            if (dist_tab[place[j]] > dist_tab[place[high_index]]) {
                high_index = j;
            }
        }
        /* Assign position (0 = first) */
        car_array[place[high_index]].place = i + (num_active_cars - num_left);

        /* Swap to sorted portion */
        swap(place[high_index], place[i]);
    }
}
```

### Finished Car Ranking

Finished cars are sorted by completion time:

```c
void sort_finished_positions(void) {
    /* Bubble sort by score (ascending - lowest time = first) */
    for (i = 0; i < num_locked - 1; i++) {
        for (j = 0; j < num_locked - i - 1; j++) {
            if (car_array[place[j]].score > car_array[place[j + 1]].score) {
                swap(place[j], place[j + 1]);
            }
        }
    }

    /* Handle ties - lower car index wins */
    for (i = 1; i < num_locked; i++) {
        if (car_array[place[i-1]].score == car_array[place[i]].score &&
            place[i-1] > place[i]) {
            swap(place[i], place[i-1]);
        }
    }
}
```

---

## Finish Line Handling

### Race Completion Detection

```c
void PassedCP(s32 car_index, s8 flag) {
    if (next_cp == trck->finish_line) {
        laps_remaining = trck->number_of_laps - car_array[node].laps;

        if (laps_remaining <= 0 && car_array[node].place_locked == 0) {
            /* Car finished the race! */
            car_array[node].place_locked = 1;
            end_game_flag = 1;
        }
    }
}
```

### Arcade Finish Time Interpolation

The arcade interpolates exact finish time based on velocity:

```c
/* From arcade checkpoint.c:1103-1110 */
SubVector(gc->dr_pos, chkpnt[m->last_cp_index].pos, rpos);
dist = BodyZ(rpos, chkpnt[m->last_cp_index].uvs);
vel = BodyZ(gc->dr_vel, chkpnt[m->last_cp_index].uvs);
/* Get msec since crossed finish line */
dtime = (vel > 1.0) ? (dist/vel)*1000.0 : 0;
game_car[node].gameover_time -= dtime;
```

### Visual Indication

The checkpoint before finish triggers the checkered flag:

```c
void PassedCP(MODELDAT *m, S8 flag) {
    if (m->last_cp_index == trck->before_finish) {
        if (trck->number_of_laps - m->laps == 1) {
            ChangeFinishLine();  /* Swap to checkered flag texture */
        }
    }
}
```

---

## Arcade Source Comparison

### Function Mapping

| Arcade Function | N64 Function | Location |
|-----------------|--------------|----------|
| `init_cp_data()` | `init_cp_data()` | checkpoint.c |
| `InitCPS()` | `InitCPS()` | checkpoint.c |
| `CheckCPs()` | `CheckCPs()` | checkpoint.c |
| `PassedCP()` | `PassedCP()` | checkpoint.c |
| `get_next_checkpoint()` | `get_next_checkpoint()` | checkpoint.c |
| `get_next_center()` | `get_next_center()` | checkpoint.c |
| `check_everyones_checkpoints()` | `check_everyones_checkpoints()` | checkpoint.c |
| `CarPassedCheckpoint()` | `CarPassedCheckpoint()` | checkpoint.c |
| `CarReportsGameOver()` | `CarReportsGameOver()` | checkpoint.c |
| `update_lap_counter()` | `update_lap_counter()` | checkpoint.c |
| `GetElapsedTime()` | `GetElapsedTime()` | timer.c |
| `GetCountdownTime()` | `GetCountdownTime()` | timer.c |
| `SetCountdownTimer()` | `SetCountdownTimer()` | timer.c |
| `TimeOut()` | `TimeOut()` | timer.c |

### Key Differences

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Timer resolution | Hardware IRQ (1ms) | osGetCount (~21ns) |
| Time unit | Milliseconds | Frames (60 FPS) or ms |
| Network sync | DIS PDUs | Not applicable |
| Position calculation | `find_maxpath_intervals()` | `calc_car_distance()` |
| HUD update | Direct text drawing | Abstracted through race.c |

### Arcade Constants

```c
/* From arcade checkpoint.c */
#define MAX_CPS         20      /* Max checkpoints per track */
#define ONE_SEC         1000    /* Milliseconds per second */
#define CHECKPT_TIME    /* Checkpoint display timeout */
```

### Network Messages (Arcade Only)

The arcade uses DIS protocol for multiplayer:

```c
/* Checkpoint crossing notification */
PutEventReportPDU(Event_CheckPoint, 4,
    Report_Node, gThisNode,
    Report_Channel, node,
    Report_CheckPoint, (m->laps<<16)+(m->last_cp_index<<8)+(m->next_cp_index),
    Report_Flags, flag);

/* Race completion notification */
PutEventReportPDU(Event_GameOver, 4,
    Report_Node, gThisNode,
    Report_Slot, node,
    Report_Score, GetElapsedTime(),
    Report_Flags, 0);
```

---

## Path System Integration

The checkpoint system integrates with the path/maxpath system for position calculation.

### Path Data Structures

```c
s16 path_dist_index[MAX_CHECKPOINTS];  /* Path index for each checkpoint */
u16 path_dist[MAX_PATH_POINTS];        /* Cumulative distance along path */
s16 path_index[MAX_PATH_POINTS];       /* Path point indices */
s16 path_to_maxpath[MAX_PATH_POINTS];  /* Mapping to maxpath indices */
s16 num_path_points;                   /* Total path points */
```

### Distance Calculation

The `path_dist_index` array maps checkpoints to path distances:

```c
f32 calc_car_distance(s32 car_index) {
    s32 cp = car_array[car_index].checkpoint;
    s32 laps = car_array[car_index].laps;

    distance = (f32)laps * (f32)num_path_points;
    distance += (f32)path_dist_index[cp];

    return distance;
}
```

---

## Files Reference

| File | Purpose |
|------|---------|
| `src/game/checkpoint.c` | N64 checkpoint detection and lap counting |
| `src/game/race.c` | N64 race state management and timing |
| `src/game/timer.c` | N64 timer utilities (frame and ms-based) |
| `include/game/checkpoint.h` | Checkpoint data structures and prototypes |
| `include/game/race.h` | Race state structures and prototypes |
| `include/game/structs.h` | CarData and GameStruct definitions |
| `reference/repos/rushtherock/game/checkpoint.c` | Original arcade implementation |
| `reference/repos/rushtherock/game/checkpoint.h` | Original arcade headers |

---

## Summary

The Rush 2049 N64 checkpoint system is a faithful port of the arcade implementation:

1. **Checkpoint Detection**: Uses identical plane-test algorithm with proximity culling
2. **Lap Counting**: Same checkpoint sequence wrapping logic
3. **Timer System**: Arcade-compatible millisecond API on top of N64 frame counting
4. **Time Extensions**: Per-checkpoint bonus time scaled by difficulty
5. **Position Ranking**: Distance-based sorting for active cars, time-based for finished
6. **Finish Handling**: Interpolated finish times, checkered flag swap before final checkpoint

The main adaptations are:
- N64 uses `osGetCount()` instead of hardware IRQ for timing
- Network PDU messages replaced with local function calls
- Frame-based timing added as alternative to millisecond timing
