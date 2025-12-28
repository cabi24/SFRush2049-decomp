# AI/Maxpath Navigation System Mapping

This document maps the arcade Rush The Rock AI/pathfinding system to the N64 Rush 2049 implementation.

## 1. Maxpath Algorithm Overview

The "maxpath" system is a waypoint-based AI navigation system used by drone (AI-controlled) cars. It consists of:

### Core Components

1. **Path Data Structure** - Pre-recorded waypoints along the track with position, speed, and hint data
2. **Path Following** - Drones follow waypoints while respecting speed and steering guidance
3. **Path Switching** - Multiple paths per track allow drones to take alternate routes
4. **Rubber-banding** - Speed adjustments to keep races competitive

### Algorithm Flow

```
Per Frame:
1. InitMaxPath()      - Load path data at race start
2. MaxPath()          - Main path processing (calls RecordMaxPath if recording)
3. MaxPathControls()  - Generate steering/throttle/brake inputs
   |-- MP_FindInterval()    - Find current position on path
   |-- MP_TargetSpeed()     - Calculate target speed
   |-- MP_TargetSteerPos()  - Calculate steering target
   |-- avoid_areas()        - Obstacle/traffic avoidance
   |-- AdjustSpeed()        - Apply throttle/brake
   |-- AdjustSteer()        - Apply steering
```

## 2. Data Structures

### MPHEADER - Path Header (32 bytes)
```c
typedef struct MPHEADER {
    S32 num_points;      // Number of waypoints in path
    S32 mpath_active;    // 1 if path is active, 0 if unused
    S8  priority;        // Path priority (0-10, higher = prefer this path)
    S8  dummy;           // Padding
    S16 lap_start;       // Waypoint index where lap starts
    S16 lap_end;         // Waypoint index where lap ends
    S16 num_in_lap;      // Number of waypoints in one lap
    S32 spare[2];        // Reserved
} MPHEADER;
```

### MPATH - Waypoint Data (24 bytes)
```c
typedef struct MPATH {
    F32 pos[3];          // World position (X, Y, Z)
    F32 speed;           // Target speed at this point (feet/sec)
    S32 hints;           // Behavior hints (see HINT_TYPES)
    S16 base_to_path;    // Link to corresponding point in alternate path
    S16 path_to_base;    // Link back to base path
} MPATH;
```

### MPCTL - Per-Car Path Control State
```c
typedef struct MPCTL {
    S32 mpi;             // Current path index (waypoint number)
    S16 mpath_index;     // Which path set (0-7) this car follows
    S16 default_path;    // Assigned default path for this drone
    S32 new_mpi;         // Target path index for resync (-1 if none)
    F32 xrel;            // Position relative to path (forward/back)
    F32 yrel;            // Position relative to path (left/right)
    F32 len;             // Distance to next waypoint
    F32 cyrel;           // Path curvature
    F32 tgtspd;          // Calculated target speed
    F32 tgtpos[3];       // Calculated target position
    S32 interval_time;   // Last time waypoint was updated
} MPCTL;
```

### MPWEIGHT - Path Selection Weights
```c
typedef struct MPWEIGHT {
    F32 distance_weight;   // Weight for proximity to path
    F32 direction_weight;  // Weight for velocity alignment
    F32 speed_weight;      // Weight for speed matching
    F32 own_path_weight;   // Bonus for staying on assigned path
    F32 priority_weight;   // Weight for path priority
    F32 hint_weight;       // Weight for following hints
} MPWEIGHT;
```

### HINT_TYPES - Behavior Hints
```c
typedef enum HINT_TYPES {
    STAY_WITHIN_WIDTH = 0, // Stay within track width
    GET_ON_MPATH = 1,      // Move toward path line
    STAY_ON_MPATH = 2,     // Follow path exactly
    STAY_PARALLEL = 3,     // Stay parallel to path (for traffic)
    RESET_PARALLEL = 4,    // Reset parallel tracking
    EASE_ON_MPATH = 5      // Gradually move toward path
} HINT_TYPES;
```

## 3. Function Mapping Table

### Core Path Functions

| Arcade Function | N64 Equivalent | ROM Address | Notes |
|-----------------|----------------|-------------|-------|
| `InitMaxPath()` | `ai_init()` (implied) | - | Path data loaded at track load |
| `MaxPath()` | `ai_input_generate()` | 0x8010D1B0 | Main per-frame AI update |
| `MaxPathControls()` | `ai_path_follow()` | 0x8010D680 | Path following logic |
| `MP_FindInterval()` | Internal to path_follow | - | Position calculation |
| `MP_TargetSpeed()` | `ai_control_speed()` | 0x8010DE58 | Speed target calculation |
| `MP_TargetSteerPos()` | Internal steer calc | - | 80-foot lookahead steering |
| `AdjustSpeed()` | `ai_control_speed()` | 0x8010DE58 | Throttle/brake output |
| `AdjustSteer()` | Internal to input_gen | - | Steering output |
| `avoid_areas()` | `ai_obstacle_avoid()` | 0x8010D85C | Traffic avoidance |

### Drone Management Functions

| Arcade Function | N64 Equivalent | ROM Address | Notes |
|-----------------|----------------|-------------|-------|
| `InitDrones()` | `drone_init()` | - | Initialize drone array |
| `DoDrones()` | `drone_ai_update()` | 0x800ED2F0 | Per-drone update loop |
| `assign_drones()` | `ai_behavior_select()` | 0x8010E0FC | Assign behavior modes |
| `place_cars_in_order()` | Position calc in game.c | - | Sort cars by distance |
| `linear_interp()` | Inline calculations | - | Linear interpolation helper |

### Path Selection Functions

| Arcade Function | N64 Equivalent | ROM Address | Notes |
|-----------------|----------------|-------------|-------|
| `MP_IntervalPos()` | Internal to path_follow | - | Relative position calc |
| `mp_interval_pos()` | Internal | - | Same, external cars |
| `NextMaxPath()` | `ai_waypoint_next()` | 0x8010D3C0 | Advance waypoint |
| `PrevMaxPath()` | - | - | Reverse waypoint (unused) |
| `find_maxpath_dist()` | - | - | Find waypoint by distance |
| `link_maxpath_and_pathpoints()` | - | - | Build path cross-refs |

### Rubber-Banding Functions

| Arcade Function | N64 Equivalent | ROM Address | Notes |
|-----------------|----------------|-------------|-------|
| `set_catchup()` | `rubber_banding()` | 0x801079AC | Main catchup algorithm |
| `no_catchup()` | (difficulty=hard) | - | Disable rubber-banding |
| `avg_catchup()` | - | - | Solo player boost (unused) |
| - | `ai_rubber_band()` | 0x8010DCFC | Per-car speed adjustment |
| - | `ai_catch_up()` | 0x8010E8B4 | Aggressive catch-up mode |
| - | `ai_catch_up_logic()` | 0x801089CC | Detailed catch-up |

### N64-Specific AI Functions

| Function | ROM Address | Size | Description |
|----------|-------------|------|-------------|
| `ai_update_car()` | 0x8010C800 | 3276 bytes | Full car AI update |
| `ai_follow_path()` | 0x8010CB7C | 484 bytes | Simple path follow |
| `ai_avoid_obstacles()` | 0x8010CD00 | 1068 bytes | Obstacle detection |
| `ai_optimize_racing_line()` | 0x8010D100 | 1592 bytes | Racing line optimization |
| `ai_decide_strategy()` | 0x8010D700 | 2700 bytes | Strategy selection |
| `ai_attempt_overtake()` | 0x8010DF00 | 1084 bytes | Overtaking logic |
| `ai_defend_position()` | 0x8010E3A0 | 808 bytes | Defensive driving |
| `ai_recover_crash()` | 0x8010E6C0 | 1756 bytes | Crash recovery |
| `ai_difficulty_set()` | 0x8010DF90 | 364 bytes | Set AI difficulty |
| `ai_difficulty_adjust()` | 0x80107600 | 924 bytes | Dynamic difficulty |
| `ai_error_inject()` | 0x8010E72C | 392 bytes | Human-like errors |
| `entity_ai_pathfind()` | 0x800ECAD0 | 2388 bytes | General pathfinding |

### Track Node Functions

| Function | ROM Address | Description |
|----------|-------------|-------------|
| `track_get_node_pos()` | 0x80086A50+ | Get track node position |
| `path_follow()` | - | Generic path follow (extern) |

## 4. Rubber-Band Formula

The arcade catchup system uses these formulas:

### Per-Track Parameters
```c
// Catchup zone = distance (feet) where rubber-banding applies
const F32 cuzone[8] = { 400, 800, 800, 600, 900, 600, 800, 600 };

// Max catchup scale (percent speed boost when at max distance)
const F32 cuscale[8] = { 0.30, 0.20, 0.10, 0.20, 0.20, 0.20, 0.20, 0.25 };
// Track 1: up to 30% speed boost
// Track 3: only 10% boost (harder)
```

### Catchup Calculation
```c
void set_catchup(void) {
    // Find leader distance
    firstdist = game_car[leader].distance;

    for each human player:
        // Calculate lag distance
        lagdist = firstdist - game_car[i].distance;

        if (lagdist > cuzone[track])
            // Max catchup if outside zone
            target_catchup = max_catchup + 1.0;
        else
            // Linear interpolation within zone
            target_catchup = (lagdist * max_catchup / cuzone[track]) + 1.0;

        // Smooth transition (97% old + 3% new)
        model[i].catchup = 0.97 * model[i].catchup + 0.03 * target_catchup;
}
```

### N64 Rubber-Band Implementation
```c
void rubber_banding(void *race) {
    leaderDist = leader->trackDistance;

    for each AI:
        gap = leaderDist - ai->trackDistance;

        if (gap > 100.0f)
            // Far behind - speed boost (up to 30%)
            speedBoost = 1.0 + (gap - 100.0) * 0.002;
            speedBoost = min(speedBoost, 1.3);
        else if (gap < -50.0f)
            // Too far ahead - slow down (to 90%)
            speedBoost = 1.0 + gap * 0.001;
            speedBoost = max(speedBoost, 0.9);
        else
            speedBoost = 1.0;  // Close race

        ai->maxSpeed *= speedBoost;
}
```

## 5. Path Following Pseudocode

### Target Speed Calculation (MP_TargetSpeed)
```c
void MP_TargetSpeed(car, pathCtl) {
    // Get current and next waypoints
    curWP = path[pathCtl->mpi];
    nextWP = path[NextMaxPath(pathCtl->mpi)];

    // Interpolate speed between waypoints
    t = pathCtl->xrel / pathCtl->len;
    t = clamp(t, 0.0, 1.0);
    targetSpeed = curWP->speed + t * (nextWP->speed - curWP->speed);

    // Slow down if off racing line
    yrel = abs(pathCtl->yrel);
    if (yrel > 0.1) {
        if (yrel > 80.0)
            offScale = 0.8;  // 20% penalty at 80+ feet off
        else
            offScale = 1.0 - (yrel * yrel * 0.00003125);
        targetSpeed *= offScale;
    }

    // Minimum speed of ~19 mph
    targetSpeed = max(targetSpeed, 28.0);
}
```

### Target Steering Position (MP_TargetSteerPos)
```c
// Look ahead 80 feet on path
const F32 LOOKAHEAD = 80.0;

void MP_TargetSteerPos(car, pathCtl) {
    // Start from current position
    tgtDist = LOOKAHEAD + pathCtl->xrel;
    curWP = path[pathCtl->mpi];

    // Walk along path until we've gone 80 feet
    while (tgtDist > 0.0) {
        nextWP = path[NextMaxPath(curIdx)];
        dist = distance(curWP->pos, nextWP->pos);
        tgtDist -= dist;
        curWP = nextWP;
    }

    // Interpolate final target position
    t = (dist + tgtDist) / dist;
    targetPos = lerp(curWP->pos, nextWP->pos, t);
}
```

### Speed/Throttle Adjustment (AdjustSpeed)
```c
void AdjustSpeed(car, targetSpeed) {
    targetSpeed *= 1.05;  // 5% buffer

    if (front_tires_slipping) {
        brake = 0;
        throttle = 0;
    } else {
        currentSpeed = magnitude(car->velocity);
        deltaSpeed = targetSpeed - currentSpeed;

        if (deltaSpeed < 0)
            deltaSpeed *= 0.5;  // Less aggressive braking

        throttle += deltaSpeed * 750.0;

        if (throttle < -3000)
            brake -= (throttle + 3000) / 4;
        else
            brake = 0;
    }

    throttle = clamp(throttle, 0, 0x1000);
    brake = clamp(brake, 0, 0x1000);
}
```

### Steering Adjustment (AdjustSteer)
```c
void AdjustSteer(car, targetPos) {
    // Target position is in body-relative coordinates
    // [0] = forward/back, [1] = left/right

    // Amplify lateral component
    targetPos[1] *= 1.8;

    // Calculate angle to target
    angle = atan2(targetPos[0], targetPos[1]);
    angle = clamp(angle, 0, PI);

    // Convert to steering value (-0x1000 to 0x1000)
    wheel = 4096.0 - angle * 2607.6;

    car->steering = wheel;
}
```

## 6. Difficulty Effects on Drones

### Arcade Drone Parameters

| Parameter | Description |
|-----------|-------------|
| `drone_scale` | Speed multiplier (0.9-1.0 based on position) |
| `time_boost` | RPM/acceleration modifier (0.98-1.1) |
| `difficulty` | Per-drone difficulty (0=hard, 7=easy) |

### N64 Difficulty Settings

```c
void ai_difficulty_set(car, difficulty) {
    switch (difficulty) {
        case 0:  // Easy
            reactionTime = 0.5;   // Slow reactions
            accuracy = 0.7;       // Sloppy steering
            aggression = 0.3;     // Passive
            maxSpeed = 0.85;      // 85% top speed
            break;

        case 1:  // Medium
            reactionTime = 0.3;
            accuracy = 0.85;
            aggression = 0.5;
            maxSpeed = 0.95;
            break;

        case 2:  // Hard
            reactionTime = 0.15;
            accuracy = 0.95;
            aggression = 0.7;
            maxSpeed = 1.0;
            break;

        case 3:  // Expert
            reactionTime = 0.05;  // Near-instant
            accuracy = 1.0;       // Perfect steering
            aggression = 0.9;     // Very aggressive
            maxSpeed = 1.05;      // 105% top speed
            break;
    }
}
```

## 7. Obstacle Avoidance (avoid_areas)

The arcade uses a sophisticated avoidance system:

```c
void avoid_areas(droneIndex) {
    // Get target from path
    pathTarget = mpctl[droneIndex].tgtpos;

    // Convert to body-relative coordinates
    rpos = worldToBody(pathTarget - car.position, car.orientation);

    // If going too slow, just aim for path
    if (car.speed < targetPathSpeed * 0.5) {
        rpos[1] *= 0.2;  // Reduce lateral correction
        return;
    }

    // If very far off path, just try to get back
    if (rpos[0] < abs(rpos[1] * 2)) {
        return;  // Use raw path direction
    }

    // Check each other car
    for each otherCar:
        relPos = worldToBody(otherCar.pos - car.pos, car.orientation);

        // Skip if too far (300 ft)
        if (distance(relPos) > 300)
            continue;

        // Check if human nearby (affects cheating)
        if (otherCar.isHuman && distance < 200)
            canCheat = false;

        // Skip if behind us
        if (relPos[0] < 0)
            continue;

        // If in front and close to our line
        if (abs(relPos[1]) < 8) {
            // Slow down based on proximity
            if (relPos[0] < 20)
                speedScale = 0.9;
            else if (relPos[0] < 100)
                speedScale = 1.0 - (100 - relPos[0]) / 80 * 0.1;
        }

    // Apply hint-based behavior
    switch (path[mpi].hint):
        case STAY_ON_MPATH:
            // Follow path exactly, slow for traffic
            break;
        case GET_ON_MPATH:
            // Gradually move toward path (15% path, 85% current)
            break;
        case EASE_ON_MPATH:
            // Less aggressive path return (40% lateral correction)
            break;
        case STAY_PARALLEL:
            // Track path changes, not absolute position
            break;
}
```

## 8. Key Global Variables

### Arcade Globals
| Variable | Description |
|----------|-------------|
| `gMaxPath` | Path data pointer (MPHEADER*) |
| `mpath` | Current path waypoints (MPATH*) |
| `mpctl[]` | Per-car path control (MPCTL[8]) |
| `gUseCatchup` | Enable/disable rubber-banding |
| `trackno` | Current track index (0-7) |
| `num_mpaths` | Number of active paths |
| `mpaths[]` | Active path indices |

### N64 Globals
| Address | Description |
|---------|-------------|
| `D_80143000` | Path data array |
| `D_80143200` | Waypoints per path |
| `D_80143400` | Lead car distance |
| `D_80143404` | Player distance |
| `D_80143500` | Random seed for AI errors |
| `D_80152744` | Number of active cars |
| `D_80152818` | Car state array base |
| `D_80159A08` | Current track ID |
| `D_80159B34` | Rubber-band mode (0=off, 1=heavy) |

## 9. Path Data Format

Paths are stored as arrays of MPATH structures with an MPHEADER prefix:

```
Path Data Layout (per path set):
+0x0000: MPHEADER (32 bytes)
+0x0020: MPATH[0] (24 bytes) - first waypoint
+0x0038: MPATH[1] (24 bytes)
...
+0x???? : MPATH[num_points-1]

Maximum: MAXMPATH = 2000 waypoints per path
Maximum: MAX_MPATHS = 8 alternate paths per track
```

### Path Recording

Paths are recorded during development:
1. Drive track at target speed
2. Every 20 feet, store position and current speed
3. Add hints at corners, obstacles, shortcuts
4. Link alternate paths to base path

## 10. N64-Specific Enhancements

The N64 version adds several AI features not in the arcade:

1. **Dynamic Difficulty** - Adjusts AI based on player performance
2. **Racing Line Optimization** - Calculates optimal corner entry/exit
3. **Strategic Behaviors** - Overtake, defend, recover modes
4. **Error Injection** - Random mistakes for realism
5. **Aggression System** - Variable blocking/passing behavior
6. **Crash Recovery** - Automated recovery from crashes/stuck

These additions likely compensate for:
- Fewer processing cycles on N64
- Need for longer single-player races
- Console game balancing requirements
