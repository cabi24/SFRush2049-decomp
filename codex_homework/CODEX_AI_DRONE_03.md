# Codex Homework: AI/Drone System - Part 3 of 6
## Steering & Speed Control

**Estimated Time**: 3-4 hours
**Difficulty**: Medium-Hard
**Prerequisites**: CODEX_AI_DRONE_02 completed

---

## Objective

Implement the steering and speed control functions that convert path data into actual car inputs (throttle, brake, steering wheel).

---

## Background

Once the AI knows where it is relative to the path (from Part 2), it needs to:
1. Calculate a target speed based on path data
2. Calculate a steering direction to reach the look-ahead point
3. Convert these into throttle/brake/wheel inputs

---

## N64 Functions to Match

| N64 Function | Address | Size | Arcade Equivalent |
|--------------|---------|------|-------------------|
| `ai_input_generate()` | 0x80041589 | ~400 bytes | `MaxPathControls()` |
| `ai_control_speed()` | 0x80012762 | ~300 bytes | `MP_TargetSpeed()` |
| `ai_path_follow()` | 0x80041724 | ~250 bytes | `MP_TargetSteerPos()` |

---

## Tasks

### Task 1: Implement MP_TargetSpeed()

**Arcade reference**: `maxpath.c:1414`

Calculate the target speed by interpolating between path points and adjusting for being off-path.

```c
/**
 * Calculate target speed from path data
 *
 * @param car_index Car index
 * @return Target speed in game units
 */
f32 drone_target_speed(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];
    MPATH *p0, *p1;
    f32 speed0, speed1;
    f32 t;
    f32 target_speed;
    f32 off_path_scale;

    // Get current and next path points
    p0 = &mpath_data[ctl->mpath_index][ctl->mpath_segment];
    p1 = &mpath_data[ctl->mpath_index][ctl->mpath_segment + 1];

    // Get speeds at each point
    speed0 = p0->speed;
    speed1 = p1->speed;

    // Calculate segment length for interpolation
    f32 dx = p1->pos[0] - p0->pos[0];
    f32 dz = p1->pos[2] - p0->pos[2];
    f32 seg_len = sqrtf(dx*dx + dz*dz);

    // Interpolation factor (0 at p0, 1 at p1)
    t = ctl->xrel / seg_len;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // Interpolate speed
    target_speed = speed0 + t * (speed1 - speed0);

    // Reduce speed if off path (arcade: offsc factor)
    // The further off path, the slower we should go
    f32 yrel_abs = ctl->yrel;
    if (yrel_abs < 0.0f) yrel_abs = -yrel_abs;

    if (yrel_abs > 5.0f) {
        // Scale down speed when off path
        off_path_scale = 1.0f - (yrel_abs - 5.0f) * 0.02f;
        if (off_path_scale < 0.5f) off_path_scale = 0.5f;
        target_speed *= off_path_scale;
    }

    // Minimum speed (arcade: 28 ft/s)
    if (target_speed < 28.0f) {
        target_speed = 28.0f;
    }

    return target_speed * PATH_SPEED_SCALE;
}
```

### Task 2: Implement MP_TargetSteerPos()

**Arcade reference**: `maxpath.c:1453`

Find the world position the AI should steer toward (look-ahead point).

```c
/**
 * Calculate steering target position
 * Uses look-ahead to find point on path to steer toward
 *
 * @param car_index Car index
 * @param target_pos Output: world position to steer toward [3]
 */
void drone_target_steer_pos(s32 car_index, f32 *target_pos) {
    DroneControl *ctl = &drone_ctl[car_index];

    // Look ahead on path (arcade: g_lookahd = 80 feet)
    drone_find_path_dist(car_index, PATH_LOOKAHEAD, target_pos);
}
```

### Task 3: Implement AdjustSpeed()

**Arcade reference**: `maxpath.c:1501`

Convert target speed into throttle/brake inputs.

```c
/**
 * Calculate throttle and brake to reach target speed
 *
 * @param car_index Car index
 * @param target_speed Desired speed
 * @param throttle_out Output: throttle value (0.0 to 1.0)
 * @param brake_out Output: brake value (0.0 to 1.0)
 */
void drone_adjust_speed(s32 car_index, f32 target_speed, f32 *throttle_out, f32 *brake_out) {
    CarData *car = &car_array[car_index];
    f32 current_speed = car->speed;
    f32 speed_diff = target_speed - current_speed;

    // Throttle and brake limits (arcade: 0x1000 = 4096)
    #define THROTTLE_MAX 1.0f
    #define BRAKE_MAX    1.0f

    if (speed_diff > 5.0f) {
        // Need to speed up - full throttle, no brake
        *throttle_out = THROTTLE_MAX;
        *brake_out = 0.0f;
    } else if (speed_diff > 0.0f) {
        // Slightly below target - proportional throttle
        *throttle_out = speed_diff / 5.0f;
        *brake_out = 0.0f;
    } else if (speed_diff > -10.0f) {
        // Slightly above target - coast
        *throttle_out = 0.0f;
        *brake_out = 0.0f;
    } else if (speed_diff > -30.0f) {
        // Need to slow down - light braking
        *throttle_out = 0.0f;
        *brake_out = (-speed_diff - 10.0f) / 20.0f;
    } else {
        // Way too fast - heavy braking
        *throttle_out = 0.0f;
        *brake_out = BRAKE_MAX;
    }

    // Clamp outputs
    if (*throttle_out < 0.0f) *throttle_out = 0.0f;
    if (*throttle_out > THROTTLE_MAX) *throttle_out = THROTTLE_MAX;
    if (*brake_out < 0.0f) *brake_out = 0.0f;
    if (*brake_out > BRAKE_MAX) *brake_out = BRAKE_MAX;
}
```

### Task 4: Implement AdjustSteer()

**Arcade reference**: `maxpath.c:1560`

Convert target position into steering wheel input.

```c
/**
 * Calculate steering wheel position to reach target
 *
 * @param car_index Car index
 * @param target_pos World position to steer toward [3]
 * @return Steering wheel position (-1.0 to 1.0, left to right)
 */
f32 drone_adjust_steer(s32 car_index, f32 *target_pos) {
    CarData *car = &car_array[car_index];
    f32 dx, dz;
    f32 angle_to_target;
    f32 angle_diff;
    f32 wheel;

    // Vector from car to target
    dx = target_pos[0] - car->pos[0];
    dz = target_pos[2] - car->pos[2];

    // Angle to target (atan2 gives angle in radians)
    angle_to_target = atan2f(dx, dz);

    // Difference from car's current heading
    angle_diff = angle_to_target - car->heading;

    // Normalize to -PI to PI
    while (angle_diff > 3.14159f) angle_diff -= 6.28318f;
    while (angle_diff < -3.14159f) angle_diff += 6.28318f;

    // Convert angle to wheel position
    // Arcade formula: wheel = 4096.0 - angle * 2607.6
    // Normalized: wheel = (PI - angle) / PI * 2 - 1
    wheel = angle_diff / 0.5f;  // Scale factor for responsiveness

    // Clamp to valid range
    if (wheel < -1.0f) wheel = -1.0f;
    if (wheel > 1.0f) wheel = 1.0f;

    return wheel;
}
```

### Task 5: Implement MaxPathControls()

**Arcade reference**: `maxpath.c:745`

Main function that ties it all together.

```c
/**
 * Generate all AI inputs for a car
 * Main path-following control function
 *
 * @param car_index Car index
 */
void drone_maxpath_controls(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];
    f32 target_pos[3];
    f32 target_speed;
    f32 throttle, brake;
    f32 wheel;

    // Update path tracking
    drone_find_interval(car_index);
    drone_interval_pos(car_index, &ctl->xrel, &ctl->yrel);

    // Calculate target speed from path
    target_speed = drone_target_speed(car_index);

    // Calculate steering target
    drone_target_steer_pos(car_index, target_pos);

    // Generate inputs
    drone_adjust_speed(car_index, target_speed, &throttle, &brake);
    wheel = drone_adjust_steer(car_index, target_pos);

    // Store in drone control
    ctl->throttle_target = throttle;
    ctl->brake_target = brake;
    ctl->steer_target = wheel;
    ctl->path_speed = target_speed;
}
```

---

## DroneControl Updates

Add these fields to DroneControl:

```c
// Control outputs
f32 throttle_target;    // Target throttle (0.0 to 1.0)
f32 brake_target;       // Target brake (0.0 to 1.0)
f32 steer_target;       // Target steering (-1.0 to 1.0)
f32 path_speed;         // Current target speed from path
```

---

## Verification

1. Build passes:
```bash
make VERSION=us NON_MATCHING=1 -j20
```

2. Test the math:
   - Steering should be negative (left) when target is to the left
   - Throttle should be high when below target speed
   - Brake should only activate when significantly above target speed

---

## Files to Modify

| File | Changes |
|------|---------|
| `include/game/drone.h` | Add control output fields |
| `src/game/drone.c` | Implement all 5 functions above |

---

## Arcade Source References

1. `reference/repos/rushtherock/game/maxpath.c:745-850` - MaxPathControls
2. `reference/repos/rushtherock/game/maxpath.c:1414-1600` - TargetSpeed, TargetSteerPos, AdjustSpeed, AdjustSteer

---

## Math Notes

**Steering geometry**:
```
                Target
                  *
                 /
                /  angle_to_target
               /
    Car -------> heading
```

`angle_diff = angle_to_target - heading`

Positive angle_diff = turn right
Negative angle_diff = turn left

---

## Success Criteria

- [ ] drone_target_speed() implemented
- [ ] drone_target_steer_pos() implemented
- [ ] drone_adjust_speed() implemented
- [ ] drone_adjust_steer() implemented
- [ ] drone_maxpath_controls() ties them together
- [ ] Build passes
- [ ] Git commit

---

## Next Homework

Proceed to **CODEX_AI_DRONE_04.md** for collision avoidance.
