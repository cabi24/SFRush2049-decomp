# AI Drone System Documentation

## Overview

The AI drone system in Rush 2049 controls computer-opponent cars that race against human players. The system is based on the arcade "Rush The Rock" source code and provides:

- **Path following** via the MaxPath waypoint system
- **Rubber-banding** (catchup) to keep races competitive
- **Difficulty scaling** based on player position and settings
- **Collision avoidance** with other cars and track walls
- **Multiple racing paths** for variety and passing opportunities

## Architecture

### Source Files

| Platform | File | Purpose |
|----------|------|---------|
| N64 | `src/game/drone.c` | Main drone AI logic |
| N64 | `include/game/drone.h` | Drone types and structures |
| N64 | `include/game/maxpath.h` | Path following structures |
| Arcade | `game/drones.c` | Arcade drone control |
| Arcade | `game/maxpath.c` | Arcade path following |
| Arcade | `game/maxpath.h` | Arcade path structures |
| Arcade | `game/model.c` | Catchup algorithm |

### Key Data Structures

#### DroneControl (N64)

```c
typedef struct DroneControl {
    s32     car_index;          /* Which car this controls */
    s32     drone_type;         /* HUMAN or DRONE */
    s32     personality;        /* Behavior style */
    s32     difficulty;         /* 0=hard, 7=easy */
    s32     target_car;         /* Car this drone is targeting */
    s32     weight_index;       /* Position in drone order */
    f32     drone_scale;        /* Speed scaling factor */
    f32     time_boost;         /* Time-based speed boost */
    f32     catchup_boost;      /* Catch-up speed boost */
    s32     mpath_index;        /* Current maxpath waypoint */
    s32     mpath_segment;      /* Current path segment */
    f32     xrel;               /* Forward position on segment */
    f32     yrel;               /* Lateral offset from path */
    f32     path_speed;         /* Target speed from path */
    f32     path_curvature;     /* Path curvature at current point */
    f32     steer_target;       /* Target steering angle (-1 to 1) */
    f32     throttle_target;    /* Target throttle (0 to 1) */
    f32     brake_target;       /* Target brake (0 to 1) */
    s8      we_control;         /* Do we control this car? */
    s8      is_active;          /* Is drone active? */
} DroneControl;
```

#### MPCTL (Arcade)

```c
typedef struct mpctltyp {
    S32 mpi;            /* index of current maxpath point */
    F32 xrel;           /* left/right offset to next point */
    F32 yrel;           /* forward/backward offset */
    F32 cyrel;          /* rotation to next point */
    F32 len;            /* distance to next point */
    F32 tgtspd;         /* interpolated speed for current spot */
    F32 tgtpos[3];      /* direction to steer to get to target */
    S32 interval_time;  /* time at current maxpath point */
    S16 new_mpi;        /* maxpath point to update next */
    S16 mpath_index;    /* current maxpath table being used */
    S16 default_path;   /* default path drone tries to stay on */
} MPCTL;
```

#### MaxPathPoint (Waypoint)

```c
typedef struct MaxPathPoint {
    f32     pos[3];             /* World position */
    f32     speed;              /* Suggested speed (mph) */
    s32     hints;              /* Hint flags */
    s16     base_to_path;       /* Index offset to alternate path */
    s16     path_to_base;       /* Index offset back to main path */
} MaxPathPoint;
```

## Difficulty Levels

### N64 Difficulty System

| Level | Name | Max Speed % | Aggression | Rubber-banding |
|-------|------|-------------|------------|----------------|
| 0 | Easy | 70% | 50 | Enabled |
| 1 | Medium | 85% | 70 | Enabled |
| 2 | Hard | 95% | 85 | Disabled |
| 3 | Expert | 100% | 100 | Disabled |

### Arcade Difficulty System

The arcade uses `drone_diff` (0-3):
- 0 = Easy
- 1 = Medium
- 2 = Hard
- 3 = Real Hard (Expert)

Per-track speed scales are defined in `tscale[trackno][drone_diff]`:

```c
F32 tscale[MAX_TRACKS][4] = {
    {.986, .993, 1.0, 1.06},  /* Track 1 */
    {.98,  .99, .996, 1.02},  /* Track 2 */
    {.985, .99, .997, 1.03},  /* Track 3 */
    {.985, .99, .997, 1.03},  /* Track 4 */
    /* ... */
};
```

### Dynamic Difficulty Assignment

During racing, drones are assigned difficulty based on human player positions:
- Difficulty 0 (hardest) assigned to drones near 1st place human
- Difficulty 7 (easiest) assigned to drones near last place human

```c
/* From arcade drones.c:357 */
game_car[drones[j++]].difficulty = game_car[humans[i]].place;
```

## Path Following (MaxPath System)

### Overview

The MaxPath system provides pre-recorded optimal racing lines that drones follow. Each track can have up to 8 different paths (MAX_MPATHS = 8), allowing drones to:
- Take different routes
- Pass each other
- Use shortcuts

### Waypoint Structure

Each path contains up to 2000 waypoints (MAXMPATH = 2000). Waypoints are recorded approximately every 20 feet along the racing line.

### Path Header

```c
typedef struct MaxPathHeader {
    s32     num_points;         /* Number of waypoints */
    s32     mpath_active;       /* Is path active? */
    s8      priority;           /* Path priority (higher = preferred) */
    s16     lap_start;          /* Index of lap start point */
    s16     lap_end;            /* Index of lap end point */
    s16     num_in_lap;         /* Points in one lap */
} MaxPathHeader;
```

### Hint Types

Waypoints include hints that modify drone behavior:

| Hint | Value | Behavior |
|------|-------|----------|
| STAY_WITHIN_WIDTH | 0 | Stay within track width, free to pass |
| GET_ON_MPATH | 1 | Steer toward the maxpath line |
| STAY_ON_MPATH | 2 | Stay exactly on maxpath |
| STAY_PARALLEL | 3 | Maintain parallel offset from path |
| RESET_PARALLEL | 4 | Reset parallel tracking |
| EASE_ON_MPATH | 5 | Gradually ease onto maxpath |

### Path Following Algorithm

1. **Find current segment**: Determine which path segment the car is on
2. **Calculate relative position**: Get xrel (forward) and yrel (lateral) offsets
3. **Interpolate target speed**: Blend speeds between current and next waypoint
4. **Calculate steering target**: Look ahead (80 feet) and compute steering angle
5. **Apply avoidance**: Adjust for other cars and track boundaries

```c
void drone_maxpath_controls(s32 car_index) {
    drone_find_interval(car_index);
    drone_interval_pos(car_index, &ctl->xrel, &ctl->yrel);

    target_speed = drone_target_speed(car_index);
    drone_target_steer_pos(car_index, target_pos);
    drone_adjust_speed(car_index, target_speed, &throttle, &brake);
    wheel = drone_adjust_steer(car_index, target_pos);

    drone_avoid_areas(car_index);
    drone_avoid_walls(car_index);
}
```

### Path Switching

Drones can switch between paths based on a weighted scoring system:

```c
typedef struct MaxPathWeight {
    f32     distance_weight;    /* Weight for distance to path */
    f32     direction_weight;   /* Weight for direction alignment */
    f32     speed_weight;       /* Weight for speed matching */
    f32     own_path_weight;    /* Weight for staying on own path */
    f32     priority_weight;    /* Weight for path priority */
    f32     hint_weight;        /* Weight for following hints */
} MaxPathWeight;
```

## Rubber-Banding / Catchup System

### Purpose

Rubber-banding keeps races competitive by:
- Speeding up trailing cars
- Slowing down leading cars
- Ensuring all players have a chance to win

### Catchup Constants (N64)

```c
#define CATCHUP_DISTANCE        300.0f  /* Distance for full catchup effect */
#define CATCHUP_MAX_BOOST       1.15f   /* Maximum speed boost (+15%) */
#define CATCHUP_MIN_BOOST       0.85f   /* Maximum slowdown (-15%) */
#define CATCHUP_SMOOTHING       0.03f   /* Smoothing factor per frame */
#define CATCHUP_ZONE_DEFAULT    500.0f  /* Distance zone for full effect */
#define CATCHUP_SOLO_BOOST      1.05f   /* Solo player boost */
```

### Catchup Zones (Arcade)

Per-track catchup parameters:

| Track | Zone (feet) | Max Scale |
|-------|-------------|-----------|
| 1 | 400 | 30% |
| 2 | 800 | 20% |
| 3 | 800 | 10% |
| 4 | 600 | 20% |
| 5 | 900 | 20% |
| 6 | 600 | 20% |
| 7 | 800 | 20% |
| 8 | 600 | 25% |

### Catchup Algorithm

```c
void set_catchup(void) {
    /* Find leading human */
    for (i=0; i<MAX_LINKS; i++) {
        if (model[i].drone_type == HUMAN) {
            if (game_car[i].distance > game_car[high_index].distance)
                high_index = i;
        }
    }

    /* Solo player gets constant boost */
    if (num_players == 1) {
        model[gThisNode].catchup = 1.0 + (cuscale[trackno] * cusolo);
        return;
    }

    /* Calculate catchup for each human player */
    for (i=0; i<MAX_LINKS; i++) {
        if (model[i].drone_type == HUMAN) {
            lagdist = firstdist - game_car[i].distance;

            if (lagdist > cuzone[trackno])
                target_catchup = max_catchup + 1;
            else
                target_catchup = lagdist * max_catchup / cuzone[trackno] + 1;

            /* Smooth the transition */
            model[i].catchup = .97 * model[i].catchup + .03 * target_catchup;
        }
    }
}
```

### Disabling Catchup

Catchup can be disabled for:
- Tournament mode
- Hard/Expert difficulty settings
- MaxPath recording sessions

```c
void no_catchup(void) {
    for (i=0; i<MAX_LINKS; i++)
        if (model[i].we_control)
            model[i].catchup = 1.0;
}
```

## Drone State Machine

### Drone Types

| Type | Value | Description |
|------|-------|-------------|
| DRONE_TYPE_HUMAN | 0 | Player-controlled car |
| DRONE_TYPE_DRONE | 1 | AI-controlled car |

### Drone Personalities

| Personality | Value | Description |
|-------------|-------|-------------|
| DRONE_PERS_NORMAL | 0 | Standard racing behavior |
| DRONE_PERS_AGGRESSIVE | 1 | More aggressive passing |
| DRONE_PERS_PASSIVE | 2 | Hangs back, less competitive |

### Per-Frame Update Flow

```
drone_update_all()
    |
    +-- Check game state (PLAYGAME or COUNTDOWN)
    |
    +-- Update catchup system
    |       +-- drone_set_catchup() or drone_no_catchup()
    |
    +-- Update path tracking for humans
    |       +-- drone_do_maxpath()
    |
    +-- Assign drone behaviors
    |       +-- drone_assign_all()
    |
    +-- For each active drone:
            +-- drone_maxpath_controls()  -- Generate steering/throttle
            +-- drone_avoid_areas()       -- Avoid other cars
            +-- drone_avoid_walls()       -- Avoid track edges
            +-- Apply speed multiplier
            +-- drone_apply_inputs()      -- Write to car input
```

### Arcade Frame Update (DoDrones)

```c
void DoDrones(void) {
    if (!gUseCatchup)
        no_catchup();
    else
        set_catchup();

    MaxPath(&model[gThisNode]);

    assign_drones();
}
```

## Collision Avoidance

### Car Avoidance

The `drone_avoid_areas()` function detects nearby cars and adjusts steering/speed:

```c
void drone_avoid_areas(s32 car_index) {
    avoid_dist = 30.0f;   /* Detection range */
    min_dist = 10.0f;     /* Minimum safe distance */

    for (i = 0; i < num_active_cars; i++) {
        /* Skip self */
        if (i == car_index) continue;

        /* Check distance */
        dist_sq = dx * dx + dz * dz;
        if (dist_sq > avoid_dist * avoid_dist) continue;

        /* Skip cars behind us */
        forward_dist = dx * heading_x + dz * heading_z;
        if (forward_dist < -5.0f) continue;

        /* Calculate lateral offset */
        lateral_dist = dx * heading_z - dz * heading_x;

        /* Steer away from car */
        if (dist < min_dist) {
            if (lateral_dist > 0.0f)
                steer_adjust -= 0.5f * avoid_strength;
            else
                steer_adjust += 0.5f * avoid_strength;
            speed_adjust = 0.7f;
        }
    }
}
```

### Wall Avoidance

The `drone_avoid_walls()` function prevents drones from hitting track edges:

```c
void drone_avoid_walls(s32 car_index) {
    track_half_width = 20.0f;
    edge_margin = 5.0f;

    if (ctl->yrel > track_half_width - edge_margin) {
        /* Too far right, steer left */
        ctl->steer_target -= 0.3f * (ctl->yrel - (track_half_width - edge_margin)) / edge_margin;
    } else if (ctl->yrel < -(track_half_width - edge_margin)) {
        /* Too far left, steer right */
        ctl->steer_target += 0.3f * (-(track_half_width - edge_margin) - ctl->yrel) / edge_margin;
    }
}
```

### Avoidance Strength by Hint

```c
f32 drone_get_avoid_strength(HintType hint) {
    switch (hint) {
        case HINT_STAY_ON_MPATH:    return 0.3f;  /* Light avoidance */
        case HINT_STAY_PARALLEL:    return 1.0f;  /* Full avoidance */
        case HINT_GET_ON_MPATH:
        case HINT_EASE_ON_MPATH:    return 0.6f;  /* Medium avoidance */
        default:                     return 1.0f;
    }
}
```

## Speed and Steering Calculations

### Speed Control

```c
void drone_adjust_speed(s32 car_index, f32 target_speed, f32 *throttle_out, f32 *brake_out) {
    speed_diff = target_speed - current_speed;

    if (speed_diff > 5.0f) {
        *throttle_out = 1.0f;           /* Full throttle */
        *brake_out = 0.0f;
    } else if (speed_diff > 0.0f) {
        *throttle_out = speed_diff / 5.0f;  /* Proportional throttle */
        *brake_out = 0.0f;
    } else if (speed_diff > -10.0f) {
        *throttle_out = 0.0f;           /* Coast */
        *brake_out = 0.0f;
    } else if (speed_diff > -30.0f) {
        *throttle_out = 0.0f;
        *brake_out = (-speed_diff - 10.0f) / 20.0f;  /* Light braking */
    } else {
        *throttle_out = 0.0f;
        *brake_out = 1.0f;              /* Full braking */
    }
}
```

### Steering Control

```c
f32 drone_adjust_steer(s32 car_index, f32 *target_pos) {
    /* Calculate angle to target */
    dx = target_pos[0] - car->dr_pos[0];
    dz = target_pos[2] - car->dr_pos[2];
    angle_to_target = atan2f(dx, dz);

    /* Get angle difference from current heading */
    angle_diff = angle_to_target - car->yaw;

    /* Normalize to -PI to PI */
    while (angle_diff > 3.14159f) angle_diff -= 6.28318f;
    while (angle_diff < -3.14159f) angle_diff += 6.28318f;

    /* Convert to steering value (-1 to 1) */
    wheel = angle_diff / 0.5f;
    wheel = clamp(wheel, -1.0f, 1.0f);

    return wheel;
}
```

### Speed Multiplier Calculation

```c
f32 drone_get_speed_multiplier(s32 car_index) {
    multiplier = ctl->catchup_boost;
    multiplier = drone_scale_catchup_by_difficulty(car_index, multiplier);
    multiplier *= ctl->drone_scale;
    multiplier *= ctl->time_boost;

    return multiplier;
}
```

## Key Global Variables

### N64 Globals

| Variable | Type | Description |
|----------|------|-------------|
| `drone_ctl[]` | DroneControl[MAX_CARS] | Per-car drone control state |
| `num_drones` | s32 | Number of AI cars |
| `num_humans` | s32 | Number of human players |
| `use_catchup` | s32 | Catchup enabled flag |
| `gNumMPaths` | s16 | Number of paths on current track |
| `gMPathHeaders[]` | MaxPathHeader* | Path header pointers |
| `gMPathTables[]` | MaxPathPoint* | Path waypoint arrays |
| `gMPCtl[]` | MaxPathControl | Per-car path control state |

### Arcade Globals

| Variable | Type | Description |
|----------|------|-------------|
| `mpctl[]` | MPCTL | Maxpath control per car |
| `model[]` | MODELDAT | Full car state (includes drone_type, we_control) |
| `game_car[]` | CAR_DATA | Game-side car data |
| `gUseCatchup` | BOOL | Catchup enabled flag |
| `drone_diff` | S8 | Global difficulty (0-3) |
| `gMaxPath` | MPHEADER* | Current track path data |
| `mpath_tab[]` | MPATH* | Path waypoint arrays |
| `mpheader_tab[]` | MPHEADER* | Path header pointers |

## Function Mappings (N64 to Arcade)

| N64 Function | Arcade Function | Notes |
|--------------|-----------------|-------|
| `drone_init()` | `InitDrones()` | Initialize drone system |
| `drone_update_all()` | `DoDrones()` | Per-frame update |
| `drone_init_maxpath()` | `InitMaxPath()` | Initialize path system |
| `drone_assign_all()` | `assign_drones()` | Assign drone behaviors |
| `drone_set_catchup()` | `set_catchup()` | Enable rubber-banding |
| `drone_no_catchup()` | `no_catchup()` | Disable rubber-banding |
| `drone_maxpath_controls()` | `MaxPathControls()` | Generate AI inputs |
| `drone_do_maxpath()` | `MaxPath()` | Update path following |
| `drone_find_interval()` | `MP_FindInterval()` | Find current path segment |
| `drone_interval_pos()` | `mp_interval_pos()` | Calculate relative position |
| `drone_target_speed()` | `MP_TargetSpeed()` | Get target speed from path |
| `drone_target_steer_pos()` | `MP_TargetSteerPos()` | Get steering target |
| `drone_adjust_speed()` | `AdjustSpeed()` | Convert speed to throttle/brake |
| `drone_adjust_steer()` | `AdjustSteer()` | Convert position to steering |
| `drone_avoid_areas()` | `avoid_areas()` | Car collision avoidance |
| `drone_avoid_walls()` | (inline in avoid_areas) | Track boundary avoidance |
| `drone_assign_default_paths()` | `assign_default_paths()` | Assign paths to drones |
| `drone_place_in_order()` | `place_cars_in_order()` | Sort cars by race position |
| `drone_end()` | `EndDrones()` | Cleanup (empty) |
| `linear_interp()` | `linear_interp()` | Linear interpolation utility |

## Implementation Notes

### Differences from Arcade

1. **Simplified Catchup**: N64 uses per-drone catchup; arcade applies to all linked players
2. **Fixed Difficulty Levels**: N64 has 4 preset levels; arcade uses per-drone dynamic assignment
3. **No Network Play**: N64 drones don't need `we_control` network ownership checks
4. **Simpler Path Scoring**: N64 uses basic path assignment; arcade has weighted scoring

### Performance Considerations

- Path following uses look-ahead of 80 feet to smooth steering
- Catchup smoothing (0.97/0.03) prevents jarring speed changes
- Avoidance calculations skip cars beyond 30 feet

### Debug Features

The arcade code includes debug text output controlled by `SW_DEBUG` switches:
- `SW_DEBUG1`: MaxPath display mode
- `SW_DEBUG10`: Training/drone-view mode
- `SW_DEBUG12-14`: Path set selection

### Future Improvements

1. Implement personality-based behavior variations
2. Add more sophisticated path switching logic
3. Improve collision prediction for better avoidance
4. Add track-specific tuning parameters
