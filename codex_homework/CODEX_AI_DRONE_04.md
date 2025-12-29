# Codex Homework: AI/Drone System - Part 4 of 6
## Collision Avoidance

**Estimated Time**: 2-3 hours
**Difficulty**: Medium
**Prerequisites**: CODEX_AI_DRONE_03 completed

---

## Objective

Implement collision avoidance so drones don't crash into each other or the player. The AI should steer around nearby cars and slow down when necessary.

---

## Background

The arcade `avoid_areas()` function checks distances to other cars and modifies steering/speed to avoid collisions. It considers:
- Distance to other cars
- Relative velocity
- Current hint type (some path segments allow more freedom)

---

## N64 Functions to Match

| N64 Function | Address | Size | Arcade Equivalent |
|--------------|---------|------|-------------------|
| `ai_avoid_obstacles()` | 0x80012532 | ~400 bytes | `avoid_areas()` |
| `ai_obstacle_avoid()` | 0x80041754 | ~200 bytes | Helper for avoid_areas |

---

## Tasks

### Task 1: Implement Basic avoid_areas()

**Arcade reference**: `maxpath.c:1601`

```c
/**
 * Collision avoidance with other cars
 * Modifies steering and speed to avoid nearby cars
 *
 * @param car_index Car to update
 */
void drone_avoid_areas(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];
    CarData *my_car = &car_array[car_index];
    CarData *other_car;
    f32 dx, dz;
    f32 dist_sq;
    f32 avoid_dist = 30.0f;     // Distance to start avoiding
    f32 min_dist = 10.0f;       // Minimum safe distance
    f32 steer_adjust = 0.0f;
    f32 speed_adjust = 1.0f;
    s32 i;

    for (i = 0; i < num_active_cars; i++) {
        if (i == car_index) continue;

        other_car = &car_array[i];

        // Calculate distance to other car
        dx = other_car->pos[0] - my_car->pos[0];
        dz = other_car->pos[2] - my_car->pos[2];
        dist_sq = dx*dx + dz*dz;

        // Skip if too far away
        if (dist_sq > avoid_dist * avoid_dist) continue;

        f32 dist = sqrtf(dist_sq);

        // Check if other car is ahead of us
        // Use dot product with our heading
        f32 heading_x = sinf(my_car->heading);
        f32 heading_z = cosf(my_car->heading);
        f32 forward_dist = dx * heading_x + dz * heading_z;

        if (forward_dist < -5.0f) {
            // Other car is behind us, ignore
            continue;
        }

        // Calculate lateral offset (cross product)
        f32 lateral_dist = dx * heading_z - dz * heading_x;

        // Determine avoidance direction
        if (dist < min_dist) {
            // Emergency avoidance - steer hard away
            if (lateral_dist > 0) {
                steer_adjust -= 0.5f;  // Steer left
            } else {
                steer_adjust += 0.5f;  // Steer right
            }
            speed_adjust = 0.7f;  // Slow down
        } else if (dist < avoid_dist) {
            // Gradual avoidance
            f32 urgency = 1.0f - (dist - min_dist) / (avoid_dist - min_dist);
            if (lateral_dist > 0) {
                steer_adjust -= 0.3f * urgency;
            } else {
                steer_adjust += 0.3f * urgency;
            }
            speed_adjust = 1.0f - 0.2f * urgency;
        }
    }

    // Apply adjustments
    ctl->steer_target += steer_adjust;
    if (ctl->steer_target < -1.0f) ctl->steer_target = -1.0f;
    if (ctl->steer_target > 1.0f) ctl->steer_target = 1.0f;

    ctl->throttle_target *= speed_adjust;
}
```

### Task 2: Add Hint-Based Avoidance

Different path hints affect how aggressive avoidance should be:

```c
/**
 * Get avoidance behavior for current path hint
 *
 * @param hint Current path hint type
 * @return Avoidance aggressiveness (0.0 to 1.0)
 */
f32 drone_get_avoid_strength(HintType hint) {
    switch (hint) {
        case STAY_ON_MPATH:
            // Strict path following - minimal avoidance
            return 0.3f;

        case STAY_PARALLEL:
            // Passing allowed - full avoidance
            return 1.0f;

        case GET_ON_MPATH:
        case EASE_ON_MPATH:
            // Returning to path - moderate avoidance
            return 0.6f;

        case STAY_WITHIN_WIDTH:
        default:
            // Normal - full avoidance
            return 1.0f;
    }
}
```

### Task 3: Implement Wall Avoidance (Optional)

If the car is approaching track boundaries:

```c
/**
 * Avoid track walls/boundaries
 * Uses track width data to steer away from edges
 *
 * @param car_index Car to update
 */
void drone_avoid_walls(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];

    // If yrel (lateral offset) is too large, we're near track edge
    f32 track_half_width = 20.0f;  // Approximate half-width
    f32 edge_margin = 5.0f;

    if (ctl->yrel > track_half_width - edge_margin) {
        // Near right edge - steer left
        ctl->steer_target -= 0.3f * (ctl->yrel - (track_half_width - edge_margin)) / edge_margin;
    } else if (ctl->yrel < -(track_half_width - edge_margin)) {
        // Near left edge - steer right
        ctl->steer_target += 0.3f * (-(track_half_width - edge_margin) - ctl->yrel) / edge_margin;
    }

    // Clamp steering
    if (ctl->steer_target < -1.0f) ctl->steer_target = -1.0f;
    if (ctl->steer_target > 1.0f) ctl->steer_target = 1.0f;
}
```

### Task 4: Integrate Avoidance into Main Loop

Update `drone_maxpath_controls()` to call avoidance:

```c
void drone_maxpath_controls(s32 car_index) {
    // ... existing code ...

    // Generate base inputs from path following
    drone_adjust_speed(car_index, target_speed, &throttle, &brake);
    wheel = drone_adjust_steer(car_index, target_pos);

    // Store in drone control
    ctl->throttle_target = throttle;
    ctl->brake_target = brake;
    ctl->steer_target = wheel;
    ctl->path_speed = target_speed;

    // Apply collision avoidance (modifies steer_target and throttle_target)
    drone_avoid_areas(car_index);

    // Apply wall avoidance
    drone_avoid_walls(car_index);
}
```

---

## Collision Detection Geometry

```
                Other Car
                    *
                   /|
                  / |
                 /  | lateral_dist
                /   |
               /    |
    My Car ---+--->
              ^
              forward_dist
```

- `forward_dist > 0` = other car is ahead
- `lateral_dist > 0` = other car is to our right

---

## Verification

1. Build passes:
```bash
make VERSION=us NON_MATCHING=1 -j20
```

2. Avoidance should:
   - Only activate for cars ahead
   - Steer away from the other car
   - Slow down when very close

---

## Files to Modify

| File | Changes |
|------|---------|
| `src/game/drone.c` | Add drone_avoid_areas(), drone_avoid_walls(), drone_get_avoid_strength() |
| `src/game/drone.c` | Update drone_maxpath_controls() to call avoidance |

---

## Arcade Source References

1. `reference/repos/rushtherock/game/maxpath.c:1601-1700` - avoid_areas()
2. `reference/repos/rushtherock/game/maxpath.h` - Hint types

---

## Success Criteria

- [ ] drone_avoid_areas() implemented
- [ ] drone_get_avoid_strength() implemented
- [ ] drone_avoid_walls() implemented (optional but recommended)
- [ ] Avoidance integrated into main control loop
- [ ] Build passes
- [ ] Git commit

---

## Next Homework

Proceed to **CODEX_AI_DRONE_05.md** for the rubber-band catchup system.
