# AI/Drone System Analysis - Rush 2049 N64 Decompilation

This document maps the arcade AI/drone system to the N64 decompilation, providing key insights for accurate function matching.

## Overview

The Rush arcade AI system consists of two main components:

1. **Drone Control** (`drones.c`) - High-level AI behavior, assignment, and speed regulation
2. **Path Following** (`maxpath.c`) - Pre-recorded path data and control for following racing lines

The N64 version adapts this system but with modifications for the console's constraints.

## Arcade Source Files

| File | Lines | Purpose |
|------|-------|---------|
| `reference/repos/rushtherock/game/drones.c` | ~950 | Main drone control, assignment, catchup |
| `reference/repos/rushtherock/game/maxpath.c` | ~3,730 | Path following, steering, speed control |
| `reference/repos/rushtherock/game/model.c` | ~190 (catchup) | Rubber-band catchup algorithm |
| `reference/repos/rushtherock/game/maxpath.h` | 113 | Data structures for path system |
| `reference/repos/rushtherock/game/modeldat.h` | ~600 | Car/model data structures |

## Drone State Machine

### High-Level Flow

```
InitDrones() -> DoDrones() [per-frame] -> EndDrones()
                    |
                    v
        +---------------------+
        | set_catchup() or    |
        | no_catchup()        |
        +---------------------+
                    |
                    v
        +---------------------+
        | MaxPath()           |
        | (path following)    |
        +---------------------+
                    |
                    v
        +---------------------+
        | assign_drones()     |
        | (speed regulation)  |
        +---------------------+
```

### Drone Types

From `modeldat.h`:
```c
// model[i].drone_type values:
#define HUMAN  0   // Human player
#define DRONE  1   // AI-controlled car
```

## Key Arcade Functions

### Initialization Functions

| Function | File | Purpose |
|----------|------|---------|
| `InitDrones()` | drones.c:227 | Initialize drone system, call InitMaxPath |
| `InitMaxPath(record)` | maxpath.c:421 | Load path data, set initial positions |
| `assign_default_paths()` | maxpath.c:513 | Assign path sets to drones |

### Per-Frame Update Functions

| Function | File | Purpose |
|----------|------|---------|
| `DoDrones()` | drones.c:253 | Main per-frame drone update |
| `set_catchup()` | model.c:121 | Calculate rubber-band catchup |
| `no_catchup()` | model.c:179 | Disable catchup (tournament mode) |
| `MaxPath()` | maxpath.c:657 | Record/playback path control |
| `assign_drones()` | drones.c:277 | Assign tasks, regulate speed |

### Path Following Functions

| Function | File | Purpose |
|----------|------|---------|
| `MaxPathControls()` | maxpath.c:745 | Generate steering/speed from path |
| `MP_FindInterval()` | maxpath.c:873 | Find current path segment |
| `MP_IntervalPos()` | maxpath.c:893 | Calculate position relative to path |
| `MP_TargetSpeed()` | maxpath.c:1414 | Calculate target speed from path data |
| `MP_TargetSteerPos()` | maxpath.c:1453 | Calculate target steering position |
| `AdjustSpeed()` | maxpath.c:1501 | Apply throttle/brake for target speed |
| `AdjustSteer()` | maxpath.c:1560 | Apply steering for target position |
| `avoid_areas()` | maxpath.c:1601 | Collision avoidance with other cars |

### Helper Functions

| Function | File | Purpose |
|----------|------|---------|
| `place_cars_in_order()` | maxpath.c:553 | Sort cars by race position |
| `linear_interp()` | drones.c:918 | Linear interpolation utility |
| `find_maxpath_dist()` | maxpath.c:2215 | Find path point at given distance |
| `sync_maxpath_to_last_checkpoint()` | maxpath.c:585 | Resync drone to checkpoint |

## Path Following Algorithm

### Path Data Structure

```c
typedef struct MPATH {
    F32 pos[3];         // World position (x, y, z)
    F32 speed;          // Target speed at this point
    S32 hints;          // Behavior hints (enum HINT_TYPES)
    S16 base_to_path;   // Link to base path
    S16 path_to_base;   // Link back to base
} MPATH;

typedef struct MPHEADER {
    S32 num_points;     // Number of path points
    S32 mpath_active;   // Is this path set active?
    S8  priority;       // Path priority (0-10)
    S16 lap_start;      // Index of lap start
    S16 lap_end;        // Index of lap end
    S16 num_in_lap;     // Points in one lap
} MPHEADER;
```

### Path Hint Types

```c
typedef enum HINT_TYPES {
    STAY_WITHIN_WIDTH,  // 0 - Stay within road width
    GET_ON_MPATH,       // 1 - Actively return to path
    STAY_ON_MPATH,      // 2 - Strictly follow path
    STAY_PARALLEL,      // 3 - Stay parallel (allow passing)
    RESET_PARALLEL,     // 4 - Reset parallel mode
    EASE_ON_MPATH,      // 5 - Gradually return to path
    NUM_HINT_TYPES      // 6
} HINT_TYPES;
```

### Control Flow

```
1. MP_FindInterval() - Find current path segment
   - Check if xrel >= len (moved to next segment)
   - Check if xrel < 0 (moved back a segment)

2. MP_IntervalPos() - Calculate relative position
   - xrel = forward distance along segment
   - yrel = lateral distance from path
   - cyrel = curvature to next point

3. MP_TargetSpeed() - Calculate speed
   - Interpolate between current and next point speeds
   - Scale down if off-path (offsc factor)
   - Minimum speed = 28 ft/s (~19 mph)

4. MP_TargetSteerPos() - Calculate steering target
   - Look ahead 80 feet (g_lookahd constant)
   - Find target position on path
   - Return target world coordinates

5. avoid_areas() - Collision avoidance
   - Check distance to other cars
   - Modify direction to avoid collisions
   - Apply hint-based behavior

6. AdjustSpeed() - Apply throttle/brake
   - Compare current speed to target
   - Apply acceleration/deceleration

7. AdjustSteer() - Apply steering
   - Convert target position to angle
   - Apply steering wheel position
```

## Rubber-Band Catchup System

### Algorithm (set_catchup)

```c
void set_catchup(void) {
    // Find leading human player
    for (i = 0; i < MAX_LINKS; i++) {
        if (model[i].in_game && model[i].drone_type == HUMAN) {
            if (game_car[i].distance > leader_distance)
                high_index = i;
        }
    }

    // Solo player gets compensating boost
    if (num_players == 1) {
        model[gThisNode].catchup = 1.0 + (cuscale[trackno] * cusolo);
        return;
    }

    // Multi-player: scale catchup by distance behind
    for (i = 0; i < MAX_LINKS; i++) {
        if (model[i].in_game && model[i].drone_type == HUMAN) {
            lagdist = firstdist - game_car[i].distance;
            if (lagdist > cuzone[trackno])
                target_catchup = max_catchup + 1;  // Full boost
            else
                target_catchup = lagdist * max_catchup / cuzone[trackno] + 1;

            // Smooth transition (3% per frame)
            model[i].catchup = 0.97 * model[i].catchup + 0.03 * target_catchup;
        }
    }
}
```

### Catchup Parameters

- `cuscale[trackno]` - Maximum catchup scale per track
- `cuzone[trackno]` - Distance zone for full catchup
- `cusolo` - Solo player boost factor
- `catchup` field in MODELDAT - Applied to engine torque

## Difficulty Scaling

### Drone Speed Regulation (assign_drones)

The arcade uses a sophisticated multi-tier speed regulation:

```c
// Difficulty scale (0=hard, 1=easy)
diff_scale = (F32)game_car[index].difficulty / (F32)(MAX_LINKS-1);

// Place scale (0=first, 1=last)
place_scale = (F32)my_place / (num_active_cars-1);

// Calculate min/max boost and brake values
max_boost = (1.0-1.1)*diff_scale+1.1 - place_scale * ((.15-.05)*diff_scale+.05);
min_boost = 1.0 - place_scale * ((.06-.02)*diff_scale+.02);
max_brake = 0.9 - place_scale * ((.6-.1)*diff_scale+.1);
min_brake = (.96-1.0)*diff_scale+1.0 - place_scale * ((.05-.02)*diff_scale+.02);
```

### Time Boost / Drone Scale

Two primary speed modifiers:
- `time_boost` - Applied to drone speed timer (faster/slower update)
- `drone_scale` - Applied to brake/deceleration

### Overtaking Logic

Drones track human targets and adjust:

```c
// If human in front of drone, speed up
if (delta_dist < -20) {
    if (delta_dist < -300 && okay_to_cheat)
        max_boost *= 1.2;  // Cheat boost when far behind
    time_scale = linear_interp(-300, -60, max_boost, min_boost, delta_dist);
}
// If drone ahead of human, slow down
else {
    scale = linear_interp(200, 60, max_brake, min_brake, delta_dist);
}
```

## N64 Function Mapping

Based on analysis of `src/game/game.c`:

### Direct Equivalents

| Arcade Function | N64 Function | Confidence |
|-----------------|--------------|------------|
| `InitDrones()` | (within `init_*` funcs) | Medium |
| `DoDrones()` | `drone_ai_update()` (9387) | High |
| `MaxPathControls()` | `ai_input_generate()` (41589) | High |
| `MP_FindInterval()` | `ai_waypoint_next()` (41680) | High |
| `MP_TargetSpeed()` | `ai_control_speed()` (12762) | High |
| `MP_TargetSteerPos()` | `ai_path_follow()` (41724) | High |
| `AdjustSteer()` | Within `ai_input_generate()` | High |
| `set_catchup()` | `ai_catch_up_logic()` (28265) | High |
| `avoid_areas()` | `ai_avoid_obstacles()` (12532) | High |
| `assign_drones()` | `ai_difficulty_adjust()` (28107) | Medium |
| `linear_interp()` | Likely inlined | Medium |

### N64-Specific AI Functions

| Function | Line | Purpose |
|----------|------|---------|
| `drone_ai_update()` | 9387 | Main drone AI update (3440 bytes) |
| `ai_input_generate()` | 41589 | Generate AI throttle/brake/steering |
| `ai_waypoint_next()` | 41680 | Advance to next waypoint |
| `ai_path_follow()` | 41724 | Follow path to waypoint |
| `ai_obstacle_avoid()` | 41754 | Avoid obstacles |
| `ai_overtake()` | 41805 | Overtaking behavior |
| `ai_defend()` | 41836 | Defensive driving |
| `ai_rubber_band()` | 41861 | Rubber-band catchup |
| `ai_difficulty_set()` | 41904 | Set difficulty level |
| `ai_behavior_select()` | 41958 | Select AI behavior mode |
| `ai_aggression_set()` | 42007 | Set aggression level |
| `ai_speed_limit_set()` | 42031 | Set speed limit |
| `ai_error_inject()` | 42050 | Inject driving errors (for realism) |
| `ai_catch_up()` | 42091 | Catch-up assistance |

### N64 AI Global Variables

```c
extern s32 opponents_enabled;     // Enable/disable opponents
extern s32 ai_max_speed_percent;  // AI max speed (70-100)
extern s32 ai_aggression_level;   // AI aggression (50-100)
extern s32 ai_rubberbanding;      // Enable rubber-banding (0-1)
```

### Difficulty Level Settings (N64)

From game.c line 34309-34326:
```c
switch (difficulty) {
    case 0:  // Easy
        ai_max_speed_percent = 70;
        ai_aggression_level = 50;
        ai_rubberbanding = 1;    // Heavy rubber-banding
        break;
    case 1:  // Medium
        ai_max_speed_percent = 85;
        ai_aggression_level = 70;
        ai_rubberbanding = 1;
        break;
    case 2:  // Hard
        ai_max_speed_percent = 95;
        ai_aggression_level = 85;
        ai_rubberbanding = 0;
        break;
    case 3:  // Expert
        ai_max_speed_percent = 100;
        ai_aggression_level = 100;
        ai_rubberbanding = 0;
        break;
}
```

## AI Tuning Parameters

### Arcade Tuning Constants

| Constant | Value | Purpose |
|----------|-------|---------|
| `g_lookahd` | 80.0 | Look-ahead distance for steering (feet) |
| `g_MPspdscale` | 1.0 | Global speed scale multiplier |
| `MAXMPATH` | 2000 | Maximum path points per set |
| `MAX_MPATHS` | 8 | Maximum number of path sets |
| `MAX_LINKS` | 8 | Maximum number of cars |

### Speed Control Constants

```c
// From AdjustSpeed():
if (lastthrottle[node] < -3000)
    lastbrake[node] -= (lastthrottle[node]+3000)/4;

// Throttle/brake limits
#define MAX_THROTTLE 0x1000  // 4096
#define MAX_BRAKE    0x1000  // 4096
```

### Steering Control

```c
// From AdjustSteer():
// Angle range: 0 to PI
// Wheel position: ((PI - angle) / PI * 2 - 1) * 0x1000
lastwheel[node] = 4096.0 - angle * 2607.6;
```

## Path Weight System

For dynamic path selection:

```c
typedef struct MPWEIGHT {
    F32 distance_weight;   // Weight for proximity to path
    F32 direction_weight;  // Weight for velocity alignment
    F32 speed_weight;      // Weight for speed match
    F32 own_path_weight;   // Bonus for assigned path
    F32 priority_weight;   // Weight for path priority
    F32 hint_weight;       // Weight for current hint type
} MPWEIGHT;
```

## Implementation Notes

### Key Differences: Arcade vs N64

1. **Path Data**: Arcade uses 8 path sets per track with up to 2000 points each. N64 likely uses simplified paths due to memory constraints.

2. **Update Rate**: Arcade runs at 60Hz with physics at higher rate. N64 may have variable update rate.

3. **Collision Avoidance**: Arcade has sophisticated multi-car conflict detection. N64 may be simplified.

4. **Rubber-Banding**: Both systems implement similar rubber-band catchup, but tuning differs.

5. **Difficulty**: N64 adds explicit difficulty levels with percentage-based speed limiting.

### Recommended Decompilation Order

1. Start with `ai_input_generate()` - Main AI control loop
2. Then `ai_path_follow()` - Core path following
3. Next `ai_rubber_band()` and `ai_catch_up()` - Catchup system
4. Finally `ai_difficulty_set()` - Difficulty scaling

### Debug/Testing

The arcade had extensive debug features:
- Path recording mode (SW_DEBUG10, SW_VIEW1, SW_DEBUG12-14)
- Visual path display with colored posts
- Real-time hint editing
- Drone speed/scale debugging output

Look for similar debug functions in N64 code (likely stripped but may have remnants).

## References

- Arcade source: `reference/repos/rushtherock/game/`
- N64 decompiled: `src/game/game.c`
- Path data structures: `reference/repos/rushtherock/game/maxpath.h`
- Car data structures: `reference/repos/rushtherock/game/modeldat.h`
